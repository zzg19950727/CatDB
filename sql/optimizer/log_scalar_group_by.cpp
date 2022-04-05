#include "log_scalar_group_by.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogScalarGroupBy::make_scalar_group_by(const LogicalOperator_s& child,
														Vector<ExprStmt_s> &agg_items)
{
    LogScalarGroupBy *scalar_group_by = new LogScalarGroupBy(child);
    scalar_group_by->agg_items = agg_items;
    return LogicalOperator_s(scalar_group_by);
}

u32 LogScalarGroupBy::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = 1.0;
    set_output_rows(output_rows);
    return ret;
}

u32 LogScalarGroupBy::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_scalar_group(child()->get_output_rows(), agg_items);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
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
    print_exprs(agg_items, "aggr_exprs", info);
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}