#include "log_set.h"
#include "select_stmt.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogSet::make_set_op(const LogicalOperator_s& left,
                                    const LogicalOperator_s& right,
                                    SetOpType set_type)
{
    LogSet* set = new LogSet(left, right);
    set->set_type = set_type;
    return LogicalOperator_s(set);
}

void LogSet::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    info.op = SetOpTypeString[set_type];
    print_basic_info(depth, info);
    plan_info.push_back(info);
    left_child()->print_plan(depth + 1, plan_info);
    right_child()->print_plan(depth + 1, plan_info);
}