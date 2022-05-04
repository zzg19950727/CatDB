#include "phy_expression.h"
#include "expression_calc.h"
#include "phy_subquery_evaluate.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

extern OpFuncType op_funcs[OP_MAX];

u32 CatDB::Sql::expr_filter(Vector<Expression_s> &filters, ExecCtx_s &exec_ctx)
{
	u32 ret = SUCCESS;
	bool valid = true;
	for (u32 i = 0; valid && i < filters.size(); ++i) {
		CHECK(filters[i]->get_result(exec_ctx));
		valid = exec_ctx->bool_result;
	}
	exec_ctx->bool_result = valid;
	return ret;
}

Expression::Expression()
{
}

Expression::~Expression()
{
}

ConstExpression::ConstExpression(const Object_s& object)
	:const_object(object)
{
}

Expression_s ConstExpression::make_const_expression(const Object_s & object)
{
	return Expression_s(new ConstExpression(object));
}

u32 ConstExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	ctx->output_result = const_object;
	return ret;
}

ExprType ConstExpression::get_type() const
{
	return CONST;
}

ExecParamExpression::ExecParamExpression(u32 param_index)
	:param_index(param_index)
{

}

Expression_s ExecParamExpression::make_exec_param_expression(u32 param_index)
{
	return Expression_s(new ExecParamExpression(param_index));
}

u32 ExecParamExpression::set_value(ExecCtx_s &ctx, Object_s &value)
{
	u32 ret = SUCCESS;
	ctx->param_store->set_value(param_index, value);
	return ret;
}

u32 ExecParamExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	ctx->output_result = ctx->param_store->get_value(param_index);
	return ret;
}

ExprType ExecParamExpression::get_type() const
{
	return EXEC_PARAM;
}

ColumnExpression::ColumnExpression(u32 op_id, u32 column_id)
	:op_id(op_id),
	column_id(column_id)
{
}

Expression_s ColumnExpression::make_column_expression(u32 op_id, u32 column_id)
{
	return Expression_s(new ColumnExpression(op_id, column_id));
}

u32 ColumnExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	bool find = false;
	for (u32 i = 0; i < ctx->input_rows.size(); ++i) {
		if (op_id == ctx->input_rows[i]->get_op_id()) {
			CHECK(ctx->input_rows[i]->get_cell(column_id, ctx->output_result));
			find = true;
			break;
		}
	}
	if (!find) {
		ret = ERR_UNEXPECTED;
		LOG_ERR("unexpect column value", K(op_id), K(column_id), K(ctx->input_rows));
	}
	return ret;
}

ExprType ColumnExpression::get_type() const
{
	return COLUMN;
}

SetExpression::SetExpression(u32 idx)
	:index(idx)
{

}

Expression_s SetExpression::make_set_expression(u32 idx)
{
	SetExpression *set_expr = new SetExpression(idx);
	return Expression_s(set_expr);
}

u32 SetExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	CHECK(ctx->input_rows[0]->get_cell(index, ctx->output_result));
	return ret;
}

ExprType SetExpression::get_type()const
{
	return SET_EXPR;
}

ListExpression::ListExpression()
{

}

ListExpression::~ListExpression()
{

}

Expression_s ListExpression::make_list_expression()
{
	return Expression_s(new ListExpression);
}

u32 ListExpression::get_result(ExecCtx_s &ctx)
{
	return SUCCESS;
}

ExprType ListExpression::get_type()const
{
	return EXPR_LIST;
}

void ListExpression::add_param_expr(Expression_s& expr)
{
	param_exprs.push_back(expr);
}

OpExpression::OpExpression(OperationType op)
	:op_type(op)
{

}

OpExpression::~OpExpression()
{
}

Expression_s OpExpression::make_op_expression(OperationType op)
{
	return Expression_s(new OpExpression(op));
}

u32 OpExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	CHECK(op_funcs[op_type](param_exprs, ctx));
	return ret;
}

ExprType OpExpression::get_type() const
{
	return OP_EXPR;
}

void OpExpression::add_param_expr(Expression_s& expr)
{
	param_exprs.push_back(expr);
}

SubplanExpression::SubplanExpression(u32 subplan_id)
	:subplan_id(subplan_id)
{
}

Expression_s SubplanExpression::make_subplan_expression(u32 subplan_id)
{
	SubplanExpression* expr = new SubplanExpression(subplan_id);
	return Expression_s(expr);
}

u32 SubplanExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	CHECK(set_exec_param(ctx));
	Row_s row;
	u32 row_count = 0;
	PhySubqueryEvaluate *subquery_evaluate = static_cast<PhySubqueryEvaluate*>(ctx->cur_op);
	ret = subquery_evaluate->get_subplan_next_row(subplan_id, row);
	if (FAIL(ret)) {
		ctx->output_result = Object::make_null_object();
	}
	ret = subquery_evaluate->get_subplan_next_row(subplan_id, row);
	if (SUCC(ret)) {
		ret = MORE_THAN_ONE_ROW;
	} else {
		ret = SUCCESS;
	}
	return ret;
}

ExprType SubplanExpression::get_type() const
{
	return SUBQUERY;
}

u32 SubplanExpression::set_exec_param(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
	PhySubqueryEvaluate *subquery_evaluate = static_cast<PhySubqueryEvaluate*>(ctx->cur_op);
	CHECK(subquery_evaluate->reset_subplan(subplan_id));
	for (u32 i = 0; i < exec_params.size(); ++i) {
		ExecParamExpression_s &exec_param = exec_params[i].first;
		Expression_s &ref_expr = exec_params[i].second;
		CHECK(ref_expr->get_result(ctx));
		CHECK(exec_param->set_value(ctx, ctx->output_result));
	}
	return ret;
}

u32 SubplanExpression::get_next_result(ExecCtx_s &ctx, Object_s &res)
{
	u32 ret = SUCCESS;
	Row_s row;
	PhySubqueryEvaluate *subquery_evaluate = static_cast<PhySubqueryEvaluate*>(ctx->cur_op);
	CHECK(subquery_evaluate->get_subplan_next_row(subplan_id, row));
	CHECK(row->get_cell(0, res));
	return ret;
}

CastExpression::CastExpression(OperationType op)
			:OpExpression(op)
{

}

CastExpression::~CastExpression()
{

}

Expression_s CastExpression::make_cast_expression(OperationType op)
{
	return Expression_s(new CastExpression(op));
}

void CastExpression::set_dst_type(const Common::DataType& type)
{
	dst_type = type;
}

u32 CastExpression::get_result(ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(param_exprs[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(lhs->cast_to(dst_type, ctx->output_result));
    if (lhs->is_null()) {
        ctx->output_result->set_null();
    }
    return ret;
}