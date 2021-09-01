#include "log_update.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogUpdate::make_update(LogicalOperator_s &child,
                                        BasicTableStmt_s &update_table,
                                        Vector<ExprStmt_s> &assign_exprs,
                                        Vector<ColumnStmt_s> &column_exprs,
                                        Vector<ExprStmt_s> &value_exprs,
                                        ColumnStmt_s &row_id)
{
    LogUpdate *update = new LogUpdate(child);
    update->table_item = update_table;
    update->assign_exprs = assign_exprs;
    update->column_exprs = column_exprs;
    update->value_exprs = value_exprs;
    update->row_id = row_id;
    return LogicalOperator_s(update);
}

u32  LogUpdate::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_consume, value_exprs);
    expr_ctx.expr_consume.push_back(row_id);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogUpdate::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "UPDATE";
    info.name = table_item->alias_name;
    info.expr_info += "assignment:";
    print_exprs(assign_exprs, info.expr_info);
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}