#include "log_view.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogView::make_view(ViewTableStmt_s &table, LogicalOperator_s &query_root)
{
    return LogicalOperator_s(new LogView(query_root, table));
}

u32 LogView::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_produce, access_exprs);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogView::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "VIEW";
    info.name = table_item->alias_name;
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}