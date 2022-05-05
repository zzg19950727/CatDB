#include "log_sort.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"
#include "opt_est_sel.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

LogicalOperator_s LogSort::make_sort(LogicalOperator_s &child,
									Vector<ExprStmt_s> &sort_keys,
                                    u32 top_n)
{
    LogSort *sort = new LogSort(child);
    append(sort->sort_keys, sort_keys);
    sort->top_n = top_n;
    return LogicalOperator_s(sort);
}

u32 LogSort::est_row_count()
{
    u32 ret = SUCCESS;
    if (top_n > 0) {
        if (top_n < child()->get_output_rows()) {
            output_rows = top_n;
        } else {
            output_rows = child()->get_output_rows();
        }
    } else {
        output_rows = child()->get_output_rows();
    }
    set_output_rows(output_rows);
    return ret;
}

u32 LogSort::est_cost()
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> sort_exprs;
    for (u32 i = 0; i < sort_keys.size(); ++i) {
        sort_exprs.push_back(sort_keys[i]->get_order_by_expr());
    }
    double op_cost = 0.0;
    if (!partition_keys.empty()) {
        double ndv = 0.0;
        append(sort_exprs, partition_keys);
        CHECK(EstSelUtil::calc_distinct_count(est_info, partition_keys, ndv));
        double row_per_group = child()->get_output_rows() / ndv;
        op_cost += EstCostUtil::cost_sort(row_per_group, sort_exprs);
        op_cost *= ndv;
    } else if (!sort_exprs.empty()) {
        op_cost += EstCostUtil::cost_sort(child()->get_output_rows(), sort_exprs);
    }
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

void LogSort::set_partition_keys(const Vector<ExprStmt_s>& exprs)
{
    partition_keys = exprs;
}

u32 LogSort::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < sort_keys.size(); ++i) {
        expr_ctx.expr_consume.push_back(sort_keys[i]->get_order_by_expr());
    }
    append(expr_ctx.expr_consume, partition_keys);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogSort::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    ExprInfo expr_info;
    expr_info.title = "sort_keys";
    for (u32 i = 0; i < sort_keys.size(); ++i) {
        String sort_key = sort_keys[i]->to_string();
        expr_info.exprs.push_back(sort_key);
    }
    info.expr_infos.push_back(expr_info);
    if (!partition_keys.empty()) {
        info.op = "PARTITION SORT";
        print_exprs(partition_keys, "partition_keys", info);
    } else if (top_n == 0) {
        info.op = "SORT";
    } else {
        info.op = "TOP_N SORT";
        ExprInfo expr_info;
        expr_info.title = "top_n";
        expr_info.exprs.push_back(std::to_string(top_n));
        info.expr_infos.push_back(expr_info);
    }
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}
