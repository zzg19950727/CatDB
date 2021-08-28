#include "log_join.h"
#include "expr_stmt.h"

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
    info.expr_info += "equal_join_conditions:";
    print_exprs(equal_join_condition, info.expr_info);
    info.expr_info += "other_join_conditions:";
    print_exprs(other_join_condition, info.expr_info);
    plan_info.push_back(info);
    left_child()->print_plan(depth + 1, plan_info);
    right_child()->print_plan(depth + 1, plan_info);
}