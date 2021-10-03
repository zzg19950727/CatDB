#include "log_set.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogSet::make_set_op(const LogicalOperator_s& left,
                                    const LogicalOperator_s& right,
                                    SetOpType set_type)
{
    LogSet* set = new LogSet(left, right);
    set->set_type = set_type;
    return LogicalOperator_s(set);
}

u32 LogSet::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = left_child()->get_output_rows()
                + right_child()->get_output_rows();
    return ret;
}

u32 LogSet::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = 0.0;
    switch(set_type) {
		case UNION_ALL:
			op_cost = EstCostUtil::cost_union_all(left_child()->get_output_rows(),
                                                  right_child()->get_output_rows());
            break;
		case UNION:
			op_cost = EstCostUtil::cost_hash_union(left_child()->get_output_rows(),
                                                    right_child()->get_output_rows());
            break;
		case INTERSECT:
			op_cost = EstCostUtil::cost_hash_intersec(left_child()->get_output_rows(),
                                                        right_child()->get_output_rows());
            break;
		case EXCEPT:
			op_cost = EstCostUtil::cost_hash_except(left_child()->get_output_rows(),
                                                    right_child()->get_output_rows());
            break;
	}
    cost = op_cost;
    cost += left_child()->get_cost();
    cost += right_child()->get_cost();
    return ret;
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