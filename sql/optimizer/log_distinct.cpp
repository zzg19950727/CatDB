#include "log_distinct.h"
#include "expr_stmt.h"
#include "opt_est_sel.h"
#include "opt_est_cost.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogDistinct::make_distinct(LogicalOperator_s &child)
{
    LogDistinct *distinct = new LogDistinct(child);
    return LogicalOperator_s(distinct);
}

u32 LogDistinct::est_row_count()
{
    u32 ret = SUCCESS;
    CHECK(EstSelUtil::calc_distinct_count(est_info, distinct_exprs, output_rows));
    if (output_rows > child()->get_output_rows()) {
        output_rows = child()->get_output_rows();
    }
    return ret;
}

u32 LogDistinct::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_distinct(child()->get_output_rows(),
                                                distinct_exprs);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

void LogDistinct::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "DISTINCT";
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}