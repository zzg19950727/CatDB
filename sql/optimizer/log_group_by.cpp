#include "log_group_by.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogGroupBy::make_group_by(const LogicalOperator_s& child,
											Vector<ExprStmt_s> &group_by_exprs, 
											Vector<ExprStmt_s> &agg_items)
{
    LogGroupBy *group_by = new LogGroupBy(child);
    group_by->group_by_exprs = group_by_exprs;
    group_by->agg_items = agg_items;
    return LogicalOperator_s(group_by);
}

u32  LogGroupBy::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_consume, group_by_exprs);
    append(expr_ctx.expr_consume, agg_items);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogGroupBy::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "GROUP BY";
    info.expr_info += "group_by_exprs";
    print_exprs(group_by_exprs, info.expr_info);
    if (!agg_items.empty()) {
        info.expr_info += "aggr_exprs";
        print_exprs(agg_items, info.expr_info);
    }
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}