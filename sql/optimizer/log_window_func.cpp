#include "log_window_func.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

LogicalOperator_s LogWindowFunc::make_window_func(const LogicalOperator_s& child,
												  Vector<ExprStmt_s> &win_items,
                                                  Vector<ExprStmt_s> &partition_by_exprs)
{
    LogWindowFunc *window_func = new LogWindowFunc(child);
    window_func->win_items = win_items;
    window_func->partition_by_exprs = partition_by_exprs;
    return LogicalOperator_s(window_func);
}

u32 LogWindowFunc::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = child()->get_output_rows();
    set_output_rows(output_rows);
    return ret;
}

u32 LogWindowFunc::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_win_func(child()->get_output_rows(), win_items);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

u32  LogWindowFunc::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_produce, win_items);
    for (u32 i = 0; i < win_items.size(); ++i) {
        WinExprStmt_s win_item = win_items[i];
        if (win_item->has_win_func_expr()) {
            expr_ctx.expr_consume.push_back(win_item->get_win_func_expr());
        }
        win_item->get_win_part_by_exprs(expr_ctx.expr_consume);
        win_item->get_win_order_by_exprs(expr_ctx.expr_consume);
    }
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogWindowFunc::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "WINDOW FUNCTION";
    print_exprs(win_items, "win_func_exprs", info);
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}