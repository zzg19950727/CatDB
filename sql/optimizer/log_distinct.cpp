#include "log_distinct.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogDistinct::make_distinct(LogicalOperator_s &child)
{
    LogDistinct *distinct = new LogDistinct(child);
    return LogicalOperator_s(distinct);
}

void LogDistinct::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "DISTINCT";
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}