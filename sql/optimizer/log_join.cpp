#include "log_join.h"
#include "expr_stmt.h"
#include "opt_est_sel.h"
#include "opt_est_cost.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

LogicalOperator_s LogJoin::make_join(const LogicalOperator_s& left,
                                    const LogicalOperator_s& right,
                                    JoinType join_type,
                                    JoinAlgo join_algo)
{
    LogJoin *join = new LogJoin(left, right);
    join->join_type = join_type;
    join->join_algo = join_algo;
    return LogicalOperator_s(join);
}

u32 LogJoin::est_row_count()
{
    u32 ret = SUCCESS;
    double selectivity = 1.0;
    double sel = 1.0;
    CHECK(EstSelUtil::calc_selectivity(est_info, equal_join_condition, sel));
    selectivity *= sel;
    CHECK(EstSelUtil::calc_selectivity(est_info, other_join_condition, sel));
    selectivity *= sel;
    CHECK(EstSelUtil::calc_selectivity(est_info, filters, sel));
    selectivity *= sel;
    output_rows = left_child()->get_output_rows() * right_child()->get_output_rows() * selectivity;
    return ret;
}

u32 LogJoin::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = 0.0;
    if (HASH_JOIN == join_algo) {
        op_cost = EstCostUtil::cost_hash_join(left_child()->get_output_rows(),
                                              right_child()->get_output_rows(),
                                              output_rows,
                                              equal_join_condition,
                                              other_join_condition);
    } else {
        op_cost = EstCostUtil::cost_nestloop_join(left_child()->get_output_rows(),
                                                right_child()->get_output_rows(),
                                                right_child()->get_cost(),
                                                equal_join_condition,
                                                other_join_condition);
    }
    op_cost += EstCostUtil::cost_filters(output_rows, filters);
    cost = op_cost;
    cost += left_child()->get_cost() + right_child()->get_cost();
    return ret;
}

u32  LogJoin::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_consume, equal_join_condition);
    append(expr_ctx.expr_consume, other_join_condition);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogJoin::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    if (HASH_JOIN == join_algo) {
        info.op = "HASH ";
    } else {
        info.op = "NESTED LOOP ";
    }
    info.op += JoinTypeString[join_type] + String(" JOIN");
    if (!equal_join_condition.empty()) {
        print_exprs(equal_join_condition, "equal_join_conditions", info);
    }
    if (!other_join_condition.empty()) {
        print_exprs(other_join_condition, "other_join_conditions", info);
    }
    plan_info.push_back(info);
    left_child()->print_plan(depth + 1, plan_info);
    right_child()->print_plan(depth + 1, plan_info);
}