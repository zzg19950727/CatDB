#include "phy_win_expression.h"
#include "phy_operator.h"
#include "obj_number.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

WinFuncExprCtx_s WinFuncExprCtx::make_win_ctx()
{
    return WinFuncExprCtx_s(new WinFuncExprCtx);
}

u32 WinFuncExprCtx::init(ExecCtx_s &exec_ctx, WindowFuncExpression_s &win_func_expr)
{
    if (win_func_expr->is_distinct) {
        hash_table = HashTable::make_hash_table();
        hash_table->set_exec_ctx(exec_ctx);
        hash_table->set_hash_expr(win_func_expr->expr);
	    hash_table->set_probe_expr(win_func_expr->expr);
    }
    return SUCCESS;
}

u32 WinFuncExprCtx::reset()
{
    if (hash_table) {
        hash_table->reset();
    }
    value.reset();
    row_count = 0;
	rank = 1;
	order_by_values.clear();
    return SUCCESS;
}

WindowFuncExpression::WindowFuncExpression(const Expression_s& expr, WinType op, bool is_distinct)
	:expr(expr),
	is_distinct(is_distinct),
	op(op)
{
}

WindowFuncExpression::~WindowFuncExpression()
{
}

Expression_s WindowFuncExpression::make_window_func_expression(const Expression_s& expr, WinType op, bool is_distinct)
{
	return Expression_s(new WindowFuncExpression(expr, op, is_distinct));
}

u32 WindowFuncExpression::get_result(ExecCtx_s &ctx)
{
    return OPERATION_NOT_SUPPORT;
}

u32 WindowFuncExpression::get_result(ExecCtx_s &ctx, WinFuncExprCtx_s &win_ctx)
{
	u32 ret = SUCCESS;
	if (op == WIN_COUNT) {
		ctx->output_result = Number::make_int_object(win_ctx->row_count);
	} else if (op == WIN_AVG) {
		Number_s sum = win_ctx->value;
		Number_s count = Number::make_float_object(win_ctx->row_count);
		Number_s res;
		CHECK(Number::num_div(sum, count, res));
		ctx->output_result = res;
	} else if (WIN_SUM == op ||
			   WIN_MIN == op ||
			   WIN_MAX == op) {
        ctx->output_result = win_ctx->value;
    } else if (WIN_RANK == op) {
		Vector<Object_s> order_by_values;
		++win_ctx->row_count;
		CHECK(calc_order_by_exprs(ctx, order_by_values));
		if (win_ctx->order_by_values.empty()) {
			win_ctx->order_by_values = order_by_values;
			ctx->output_result = Number::make_int_object(win_ctx->rank);
		} else {
			bool is_equal = false;
			CHECK(compare_order_by_values(order_by_values, 
										  win_ctx->order_by_values, 
										  is_equal));
			if (is_equal) {
				ctx->output_result = Number::make_int_object(win_ctx->rank);
			} else {
				win_ctx->rank = win_ctx->row_count;
				win_ctx->order_by_values = order_by_values;
				ctx->output_result = Number::make_int_object(win_ctx->rank);
			}
		}
	} else if (WIN_DENSE_RANK == op) {
		Vector<Object_s> order_by_values;
		++win_ctx->row_count;
		CHECK(calc_order_by_exprs(ctx, order_by_values));
		if (win_ctx->order_by_values.empty()) {
			win_ctx->order_by_values = order_by_values;
			ctx->output_result = Number::make_int_object(win_ctx->rank);
		} else {
			bool is_equal = false;
			CHECK(compare_order_by_values(order_by_values, 
										  win_ctx->order_by_values, 
										  is_equal));
			if (is_equal) {
				ctx->output_result = Number::make_int_object(win_ctx->rank);
			} else {
				++win_ctx->rank;
				win_ctx->order_by_values = order_by_values;
				ctx->output_result = Number::make_int_object(win_ctx->rank);
			}
		}
	} else if (WIN_ROW_NUMBER == op) {
		++win_ctx->row_count;
		ctx->output_result = Number::make_int_object(win_ctx->row_count);
	}
	return ret;
}

ExprType WindowFuncExpression::get_type() const
{
	return WIN_EXPR;
}

u32 WindowFuncExpression::add_row(ExecCtx_s &ctx, WinFuncExprCtx_s &win_ctx)
{
	u32 ret = SUCCESS;
	Row_s &row = ctx->input_rows[0];
	if (is_distinct && op != WIN_MIN && op != WIN_MAX) {
		bool hit = false;
		CHECK(win_ctx->hash_table->probe(row, hit));
		if (!hit) {
			CHECK(win_ctx->hash_table->build_without_check(row));
		} else {
			return ret;
		}
	}
    if (expr) {
	    CHECK(expr->get_result(ctx));
    }
	switch (op)
	{
	case WIN_AVG:
	case WIN_SUM:
		return sum(ctx->output_result, win_ctx);
	case WIN_COUNT:
		return count(ctx->output_result, win_ctx);
	case WIN_MIN:
		return min(ctx->output_result, win_ctx);
	case WIN_MAX:
		return max(ctx->output_result, win_ctx);
    case WIN_RANK:
    case WIN_DENSE_RANK:
    case WIN_ROW_NUMBER:
        return ret;
	default:
		return UNKNOWN_WIN_FUNC;
	}
	return ret;
}

u32 WindowFuncExpression::calc_order_by_exprs(ExecCtx_s &ctx, Vector<Object_s> &order_by_values)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < order_by_exprs.size(); ++i) {
		CHECK(order_by_exprs[i]->get_result(ctx));
		order_by_values.push_back(ctx->output_result);
	}
	return ret;
}
u32 WindowFuncExpression::compare_order_by_values(const Vector<Object_s> &l_values, 
												  const Vector<Object_s> &r_values, 
												  bool &is_equal)
{
	u32 ret = SUCCESS;
	is_equal = true;
	if (l_values.size() != r_values.size()) {
		ret = ERR_UNEXPECTED;
		return ret;
	}
	int res = 0;
	for (u32 i = 0; i < l_values.size(); ++i) {
		CHECK(l_values[i]->compare(r_values[i], res));
		if (CMP_RES_EQ != res) {
			is_equal = false;
			break;
		}
	}
	return ret;
}

u32 WindowFuncExpression::sum(Object_s &result, WinFuncExprCtx_s &win_ctx)
{
	u32 ret = SUCCESS;
    if (0 == win_ctx->row_count) {
        win_ctx->value = Number::make_object(result);
    } else if (result->is_null()) {
		//do nothind
	} else {
        Number_s sum_result = win_ctx->value;
        CHECK(sum_result->accumulate(result));
    }
	++win_ctx->row_count;
	return ret;
}

u32 WindowFuncExpression::count(Object_s &result, WinFuncExprCtx_s &win_ctx)
{
	u32 ret = SUCCESS;
	if (!result->is_null()) {
		++win_ctx->row_count;
	}
	return ret;
}

u32 WindowFuncExpression::max(Object_s &result, WinFuncExprCtx_s &win_ctx)
{
	u32 ret = SUCCESS;
	if (result->is_null()) {
		//do nothing
	} else if (0 == win_ctx->row_count) {
		win_ctx->value = result;
		++win_ctx->row_count;
	} else {
		int res = 0;
		CHECK(win_ctx->value->compare(result, res));
		if (res == CMP_RES_LT) {
			win_ctx->value = result;
		}
		++win_ctx->row_count;
	}
	return ret;
}

u32 WindowFuncExpression::min(Object_s &result, WinFuncExprCtx_s &win_ctx)
{
	u32 ret = SUCCESS;
	if (result->is_null()) {
		//do nothing
	} else if (0 == win_ctx->row_count) {
		win_ctx->value = result;
		++win_ctx->row_count;
	} else {
		int res = 0;
		CHECK(win_ctx->value->compare(result, res));
		if (res == CMP_RES_GT) {
			win_ctx->value = result;
		}
		++win_ctx->row_count;
	}
	return ret;
}

WindowFuncCalculator::WindowFuncCalculator(const Vector<Expression_s>& win_funcs)
{
	append(this->win_funcs, win_funcs);
}

u32 WindowFuncCalculator::reset_win_func()
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < win_ctxs.size(); ++i) {
		CHECK(win_ctxs[i]->reset());
	}
	return ret;
}

u32 WindowFuncCalculator::init_win_func(ExecCtx_s &exec_ctx)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < win_funcs.size(); ++i) {
		WindowFuncExpression_s &win_func = win_funcs[i];
		WinFuncExprCtx_s win_ctx = WinFuncExprCtx::make_win_ctx();
		CHECK(win_ctx->init(exec_ctx, win_func));
		win_ctxs.push_back(win_ctx);
	}
	return ret;
}

u32 WindowFuncCalculator::add_row_to_win_func(ExecCtx_s &exec_ctx, Row_s & row)
{
	u32 ret = SUCCESS;
	exec_ctx->set_input_rows(row);
	for (u32 i = 0; i < win_funcs.size(); ++i) {
		WindowFuncExpression_s &win_func = win_funcs[i];
		CHECK(win_func->add_row(exec_ctx, win_ctxs[i]));
	}
	return ret;
}

u32 WindowFuncCalculator::calc_win_func(ExecCtx_s &exec_ctx, Row_s & row)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < win_funcs.size(); ++i) {
		WindowFuncExpression_s &win_func = win_funcs[i];
		CHECK(win_func->get_result(exec_ctx, win_ctxs[i]));
		CHECK(row->set_cell(i, exec_ctx->output_result));
	}
	return ret;
}