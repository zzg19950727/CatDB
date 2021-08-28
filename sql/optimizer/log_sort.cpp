#include "log_sort.h"
#include "select_stmt.h"
#include "expr_stmt.h"

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

u32  LogSort::allocate_expr_pre()
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
    info.expr_info += "sort keys:";
    for (u32 i = 0; i < sort_keys.size(); ++i) {
        if (0 == i) {
            info.expr_info += sort_keys[i]->order_expr->to_string();
        } else {
            info.expr_info += "," + sort_keys[i]->order_expr->to_string();
        }
        if (sort_keys[i]->asc) {
            info.expr_info += " ASC";
        } else {
            info.expr_info += " DESC";
        }
    }
    if (top_n == 0) {
        info.op = "SORT";
    } else {
        info.op = "TOP_N SORT";
        info.expr_info += "top_n:" + std::to_string(top_n) + "\n";
    }
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}