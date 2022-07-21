#include "log_group_by.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"
#include "opt_est_sel.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

LogicalOperator_s LogGroupBy::make_group_by(const LogicalOperator_s& child,
											Vector<ExprStmt_s> &group_by_exprs, 
											Vector<ExprStmt_s> &agg_items)
{
    LogGroupBy *group_by = new LogGroupBy(child);
    group_by->group_by_exprs = group_by_exprs;
    group_by->agg_items = agg_items;
    return LogicalOperator_s(group_by);
}

u32 LogGroupBy::est_row_count()
{
    u32 ret = SUCCESS;
    double ndv = 0.0;
    CHECK(EstSelUtil::calc_distinct_count(est_info, group_by_exprs, ndv));
    ndv = ndv > child()->get_output_rows() ? child()->get_output_rows() : ndv;
    distinct_rows = ndv;
    double sel = 0.0;
    CHECK(EstSelUtil::calc_selectivity(est_info, filters, sel));
    output_rows = ndv * sel;
    set_output_rows(output_rows);
    return ret;
}

u32 LogGroupBy::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_hash_group_by(child()->get_output_rows(), 
                                                     output_rows, 
                                                     group_by_exprs, 
                                                     agg_items);
    op_cost += EstCostUtil::cost_filters(output_rows, filters);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

u32  LogGroupBy::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_consume, group_by_exprs);
    append(expr_ctx.expr_produce, agg_items);
    for (u32 i = 0; i < agg_items.size(); ++i) {
        AggrStmt_s agg_item = agg_items[i];
        expr_ctx.expr_consume.push_back(agg_item->get_aggr_expr());
    }
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogGroupBy::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "HASH GROUP BY";
    print_exprs(group_by_exprs, "group_by_exprs", info);
    if (!agg_items.empty()) {
        print_exprs(agg_items, "aggr_exprs", info);
    }
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}