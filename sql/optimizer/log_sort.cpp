#include "log_sort.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

LogicalOperator_s LogSort::make_sort(LogicalOperator_s &child,
									Vector<OrderStmt_s> &sort_keys,
                                    u32 top_n)
{
    LogSort *sort = new LogSort(child);
    sort->sort_keys = sort_keys;
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
    return ret;
}

u32 LogSort::est_cost()
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> sort_exprs;
    for (u32 i = 0; i < sort_keys.size(); ++i) {
        sort_exprs.push_back(sort_keys[i]->order_expr);
    }
    double op_cost = EstCostUtil::cost_sort(child()->get_output_rows(), sort_exprs);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

u32 LogSort::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < sort_keys.size(); ++i) {
        expr_ctx.expr_consume.push_back(sort_keys[i]->order_expr);
    }
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
        String sort_key = sort_keys[i]->order_expr->to_string();
        if (sort_keys[i]->asc) {
            sort_key += " ASC";
        } else {
            sort_key += " DESC";
        }
        expr_info.exprs.push_back(sort_key);
    }
    info.expr_infos.push_back(expr_info);
    if (top_n == 0) {
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
