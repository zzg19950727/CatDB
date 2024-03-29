#include "log_join.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "opt_est_sel.h"
#include "opt_est_cost.h"
#include "error.h"
#include "log.h"

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
    if (LeftAnti == join_type || RightAnti == join_type) {
        output_rows = left_child()->get_output_rows() * right_child()->get_output_rows() * (1.0 - selectivity);
    } else {
        output_rows = left_child()->get_output_rows() * right_child()->get_output_rows() * selectivity;
    }
    if (LeftAnti == join_type || LeftSemi == join_type) {
        if (output_rows > left_child()->get_output_rows()) {
            output_rows = left_child()->get_output_rows();
        }
    } else if (RightAnti == join_type || RightSemi == join_type) {
        if (output_rows > right_child()->get_output_rows()) {
            output_rows = right_child()->get_output_rows();
        }
    } else if (LeftOuter == join_type) {
        if (output_rows < left_child()->get_output_rows()) {
            output_rows = left_child()->get_output_rows();
        }
    } else if (RightOuter == join_type) {
        if (output_rows < right_child()->get_output_rows()) {
            output_rows = right_child()->get_output_rows();
        }
    } else if (FullOuter == join_type) {
        if (output_rows < left_child()->get_output_rows()) {
            output_rows = left_child()->get_output_rows();
        }
        if (output_rows < right_child()->get_output_rows()) {
            output_rows = right_child()->get_output_rows();
        }
    }
    set_output_rows(output_rows);
    CHECK(est_left_distinct_rows());
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
	if (equal_join_condition.empty() && other_join_condition.empty()) {
		;//info.op += "CROSS ";
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

u32 LogJoin::est_left_distinct_rows()
{
    u32 ret = SUCCESS;
    if (HASH_JOIN != join_algo) {
        return ret;
    }
    Vector<ExprStmt_s> hash_exprs;
    LogicalOperator_s &left_child = childs[0];
    LogicalOperator_s &right_child = childs[1];
    for (u32 i = 0; i < equal_join_condition.size(); ++i) {
        ExprStmt_s &expr = equal_join_condition[i];
        MY_ASSERT(OP_EXPR == expr->expr_type());
        OpExprStmt_s op_expr = expr;
        MY_ASSERT(op_expr->params.size() == 2);
        ExprStmt_s &left_expr = op_expr->params[0];
        ExprStmt_s &right_expr = op_expr->params[1];
        if (left_expr->table_ids.is_subset(left_child->table_ids) && 
            right_expr->table_ids.is_subset(right_child->table_ids)) {
            hash_exprs.push_back(left_expr);
        } else if (left_expr->table_ids.is_subset(right_child->table_ids) && 
                   right_expr->table_ids.is_subset(left_child->table_ids)) {
            hash_exprs.push_back(right_expr);        
        }
    }
    CHECK(EstSelUtil::calc_distinct_count(est_info, hash_exprs, left_distinct_rows));
    return ret;
}