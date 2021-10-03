
#include "log_limit.h"
#include "opt_est_cost.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogLimit::make_limit(LogicalOperator_s &child,
                                        u32 offset,
                                        u32 limit_value)
{
    LogLimit *limit = new LogLimit(child);
    limit->offset = offset;
    limit->limit_value = limit_value;
    return LogicalOperator_s(limit);
}

u32 LogLimit::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = child()->get_output_rows();
    output_rows = output_rows > (limit_value + offset) ? (limit_value + offset) : output_rows;
    return ret;
}

u32 LogLimit::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_limit(output_rows);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

void LogLimit::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "LIMIT";
    if (offset > 0) {
        info.expr_info += "offset(" + std::to_string(offset) + ")";
    }
    info.expr_info += "limit(" + std::to_string(limit_value) + ")\n";
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}
