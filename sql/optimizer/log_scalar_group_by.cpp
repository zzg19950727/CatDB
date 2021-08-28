#include "log_scalar_group_by.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogScalarGroupBy::make_scalar_group_by(const LogicalOperator_s& child,
														Vector<ExprStmt_s> &agg_items)
{
    LogScalarGroupBy *scalar_group_by = new LogScalarGroupBy(child);
    scalar_group_by->agg_items = agg_items;
    return LogicalOperator_s(scalar_group_by);
}

u32  LogScalarGroupBy::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_produce, agg_items);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogScalarGroupBy::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "SCALAR GROUP BY";
    info.expr_info += "aggr_exprs:";
    print_exprs(agg_items, info.expr_info);
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}