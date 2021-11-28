#include "expr_generator.h"
#include "code_generator.h"
#include "phy_expression.h"
#include "logical_operator.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Parser;
using namespace CatDB::Sql;

ExprGenerateCtx::ExprGenerateCtx()
{

}

ExprGenerateCtx::~ExprGenerateCtx()
{
    
}

u32 ExprGenerator::generate_exprs(ExprGenerateCtx &ctx, 
                                  const Vector<ExprStmt_s> &exprs, 
                                  Vector<Expression_s> &rt_exprs)
{
    u32 ret = SUCCESS;
    Expression_s rt_expr;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(generate_expr(ctx, exprs[i], rt_expr));
        rt_exprs.push_back(rt_expr);
    }
    return ret;
}

u32 ExprGenerator::generate_expr(ExprGenerateCtx &ctx, 
                                 const ExprStmt_s &expr, 
                                 Expression_s &rt_expr)
{
    u32 ret = SUCCESS;
	bool find = false;
	for (auto iter = ctx.access_expr_map.begin(); !find && iter != ctx.access_expr_map.end(); ++iter) {
		if (iter->first->same_as(expr)) {
			find = true;
			rt_expr = iter->second;
		}
	}
    if (!find) {
        CHECK(inner_generate_expr(ctx, expr, rt_expr));
    }
    return ret;
}

u32 ExprGenerator::inner_generate_expr(ExprGenerateCtx &ctx, 
                                       const ExprStmt_s &expr, 
                                       Expression_s &rt_expr)
{
    
	u32 ret = SUCCESS;;
    MY_ASSERT(expr);
	switch (expr->expr_type())
	{
	case CONST:
	{
        ConstStmt_s const_expr = expr;
        rt_expr = ConstExpression::make_const_expression(const_expr->value);
		break;
	}
	case COLUMN:
	{
		ColumnStmt_s col_expr = expr;
        ColumnDesc desc;
        desc.set_tid_cid(col_expr->table_id, col_expr->column_id);
        rt_expr = ColumnExpression::make_column_expression(desc);
		break;
	}
    case SET_EXPR:
    {
        SetExprStmt_s set_expr = expr;
        rt_expr = SetExpression::make_set_expression(set_expr->get_index());
        break;
    }
	case SUBQUERY:
	{
        SubQueryStmt_s subquery = expr;
        CHECK(generate_subquery_expr(ctx, subquery, rt_expr));
		break;
	}
	case EXPR_LIST:
	{
		ret = ERR_UNEXPECTED;
        LOG_ERR("List Expr can not be generate", K(expr), K(ret));
		break;
	}
	case AGG_EXPR:
	{
        AggrStmt_s aggr_expr = expr;
		Expression_s rt_aggr_expr;
        CHECK(generate_expr(ctx, aggr_expr->get_aggr_expr(), rt_aggr_expr));
        rt_expr = AggregateExpression::make_aggregate_expression(rt_aggr_expr,
                                                                 aggr_expr->aggr_func, 
                                                                 aggr_expr->distinct);
		break;
	}
	case OP_EXPR:
	{
        OpExprStmt_s op_expr = expr;
        OpExpression_s rt_op_expr = OpExpression::make_op_expression(op_expr->op_type);
        for (u32 i = 0; i < expr->params.size(); ++i) {
            CHECK(generate_expr(ctx, expr->params[i], rt_expr));
            rt_op_expr->param_exprs.push_back(rt_expr);
        }
        rt_expr = rt_op_expr;
		break;
	}
    case EXEC_PARAM:
    {
        ExecParamStmt_s exec_param = expr;
        auto iter = ctx.exec_param_map.find(exec_param);
        MY_ASSERT(iter != ctx.exec_param_map.end());
        rt_expr = ctx.exec_param_map[exec_param];
        break;
    }
	default:
        ret = ERR_UNEXPECTED;
		LOG_ERR("unknown expr", K(expr), K(ret));
	}
	return ret;
}

u32 ExprGenerator::generate_subquery_expr(ExprGenerateCtx &ctx, 
                                          const SubQueryStmt_s &expr, 
                                          Expression_s &rt_expr)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr);
    auto iter = ctx.subplan_map.find(expr);
    MY_ASSERT(iter != ctx.subplan_map.end());
    Vector<ExecParamExpression_s> rt_exec_params;
    CHECK(generate_exec_params(ctx, expr->exec_params, rt_exec_params));
    PhyOperator_s phy_op;
    CHECK(CodeGenerator::generate_phy_plan(ctx, ctx.subplan_map[expr], phy_op));
    ctx.phy_subplans.push_back(phy_op);
    SubplanExpression_s subplan_expr = SubplanExpression::make_subplan_expression(phy_op);
    subplan_expr->exec_params = rt_exec_params;
    subplan_expr->output_one_row = expr->output_one_row;
    rt_expr = subplan_expr;
    return ret;
}


u32 ExprGenerator::generate_exec_params(ExprGenerateCtx &ctx, 
                                        Vector<ExecParamStmt_s> &exprs, 
                                        Vector<ExecParamExpression_s> &rt_exprs)
{
    u32 ret = SUCCESS;
    Expression_s rt_expr;
    for (u32 i = 0; i < exprs.size(); ++i) {
        ExecParamStmt_s &exec_param = exprs[i];
        CHECK(generate_expr(ctx, exec_param->get_ref_expr(), rt_expr));
        rt_expr = ExecParamExpression::make_exec_param_expression(rt_expr);
        rt_exprs.push_back(rt_expr);
        ctx.exec_param_map[exec_param] = rt_expr;
    }
    return ret;
}
