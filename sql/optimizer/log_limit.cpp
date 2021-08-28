#include "log_limit.h"

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

void LogLimit::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "LIMIT";
    if (offset > 0) {
        info.expr_info += "offset:" + std::to_string(offset);
    }
    info.expr_info += "limit:" + std::to_string(limit_value);
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}