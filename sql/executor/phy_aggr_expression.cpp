#include "phy_aggr_expression.h"
#include "phy_operator.h"
#include "obj_number.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

AggrExprCtx_s AggrExprCtx::make_aggr_ctx()
{
    return AggrExprCtx_s(new AggrExprCtx);
}

u32 AggrExprCtx::init(ExecCtx_s &exec_ctx, AggregateExpression_s &aggr_expr)
{
    if (aggr_expr->is_distinct) {
        hash_table = HashTable::make_hash_table();
        hash_table->set_exec_ctx(exec_ctx);
        hash_table->set_hash_expr(aggr_expr->expr);
	    hash_table->set_probe_expr(aggr_expr->expr);
    }
    return SUCCESS;
}

u32 AggrExprCtx::reset()
{
    if (hash_table) {
        hash_table->reset();
    }
    value.reset();
    row_count = 0;
    return SUCCESS;
}

AggregateExpression::AggregateExpression(const Expression_s& expr, AggrType op, bool is_distinct)
	:expr(expr),
	is_distinct(is_distinct),
	op(op)
{
}

AggregateExpression::~AggregateExpression()
{
}

Expression_s AggregateExpression::make_aggregate_expression(const Expression_s& expr, AggrType op, bool is_distinct)
{
	return Expression_s(new AggregateExpression(expr, op, is_distinct));
}

u32 AggregateExpression::get_result(ExecCtx_s &ctx)
{
    return OPERATION_NOT_SUPPORT;
}

u32 AggregateExpression::get_result(ExecCtx_s &ctx, AggrExprCtx_s &aggr_ctx)
{
	u32 ret = SUCCESS;
	if (op != COUNT && 0 == aggr_ctx->row_count) {
        ctx->output_result = Object::make_null_object();
	} else if (op == COUNT) {
		ctx->output_result = Number::make_int_object(aggr_ctx->row_count);
	} else if (op == AVG) {
		Number_s sum = aggr_ctx->value;
		Number_s count = Number::make_float_object(aggr_ctx->row_count);
		Number_s res;
		CHECK(Number::num_div(sum, count, res));
		ctx->output_result = res;
	} else {
        ctx->output_result = aggr_ctx->value;
    }
	return ret;
}

ExprType AggregateExpression::get_type() const
{
	return AGG_EXPR;
}

u32 AggregateExpression::add_row(ExecCtx_s &ctx, AggrExprCtx_s &aggr_ctx)
{
	u32 ret = SUCCESS;
	Row_s &row = ctx->input_rows[0];
	if (is_distinct && op != MIN && op != MAX) {
		bool hit = false;
		CHECK(aggr_ctx->hash_table->probe(row, hit));
		if (!hit) {
			row = Row::deep_copy(row);
			CHECK(aggr_ctx->hash_table->build_without_check(row));
		} else {
			return ret;
		}
	}
	CHECK(expr->get_result(ctx));
	switch (op)
	{
	case AVG:
	case SUM:
		return sum(ctx->output_result, aggr_ctx);
	case COUNT:
		return count(ctx->output_result, aggr_ctx);
	case MIN:
		return min(ctx->output_result, aggr_ctx);
	case MAX:
		return max(ctx->output_result, aggr_ctx);
	default:
		return UNKNOWN_AGG_FUNC;
	}
	return ret;
}

u32 AggregateExpression::sum(Object_s &result, AggrExprCtx_s &aggr_ctx)
{
	u32 ret = SUCCESS;
    if (0 == aggr_ctx->row_count) {
        aggr_ctx->value = Number::make_object(result);
    } else {
        Number_s sum_result = aggr_ctx->value;
        CHECK(sum_result->accumulate(result));
    }
	++aggr_ctx->row_count;
	return ret;
}

u32 AggregateExpression::count(Object_s &result, AggrExprCtx_s &aggr_ctx)
{
	u32 ret = SUCCESS;
	if (!result->is_null()) {
		++aggr_ctx->row_count;
	}
	return ret;
}

u32 AggregateExpression::max(Object_s &result, AggrExprCtx_s &aggr_ctx)
{
	u32 ret = SUCCESS;
	if (result->is_null()) {
		//do nothing
	} else if (0 == aggr_ctx->row_count) {
		aggr_ctx->value = result;
		++aggr_ctx->row_count;
	} else {
		int res = 0;
		CHECK(aggr_ctx->value->compare(result, res));
		if (res == CMP_RES_LT) {
			aggr_ctx->value = result;
		}
		++aggr_ctx->row_count;
	}
	return ret;
}

u32 AggregateExpression::min(Object_s &result, AggrExprCtx_s &aggr_ctx)
{
	u32 ret = SUCCESS;
	if (result->is_null()) {
		//do nothing
	} else if (0 == aggr_ctx->row_count) {
		aggr_ctx->value = result;
		++aggr_ctx->row_count;
	} else {
		int res = 0;
		CHECK(aggr_ctx->value->compare(result, res));
		if (res == CMP_RES_GT) {
			aggr_ctx->value = result;
		}
		++aggr_ctx->row_count;
	}
	return ret;
}

AggregateExpCalculator::AggregateExpCalculator(const Vector<Expression_s>& agg_funcs)
{
	append(this->agg_funcs, agg_funcs);
}

u32 AggregateExpCalculator::reset_agg_func()
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < aggr_ctxs.size(); ++i) {
		CHECK(aggr_ctxs[i]->reset());
	}
	return ret;
}

u32 AggregateExpCalculator::init_agg_func(ExecCtx_s &exec_ctx)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s &agg_func = agg_funcs[i];
		AggrExprCtx_s aggr_ctx = AggrExprCtx::make_aggr_ctx();
		CHECK(aggr_ctx->init(exec_ctx, agg_func));
		aggr_ctxs.push_back(aggr_ctx);
	}
	return ret;
}

u32 AggregateExpCalculator::add_row_to_agg_func(ExecCtx_s &exec_ctx, Row_s & row)
{
	u32 ret = SUCCESS;
	exec_ctx->set_input_rows(row);
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s &agg_func = agg_funcs[i];
		CHECK(agg_func->add_row(exec_ctx, aggr_ctxs[i]));
	}
	return ret;
}

u32 AggregateExpCalculator::calc_aggr_func(ExecCtx_s &exec_ctx, Row_s & row)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s &agg_func = agg_funcs[i];
		CHECK(agg_func->get_result(exec_ctx, aggr_ctxs[i]));
		CHECK(row->set_cell(i, exec_ctx->output_result));
	}
	return ret;
}