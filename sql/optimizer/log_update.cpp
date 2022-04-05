#include "log_update.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "opt_est_info.h"
#include "opt_est_cost.h"
#include "error.h"
#include "log.h"

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

u32 LogUpdate::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = child()->get_output_rows();
    set_output_rows(output_rows);
    return ret;
}

u32 LogUpdate::est_cost()
{
    u32 ret = SUCCESS;
    TableEstInfo_s table_statis;
    CHECK(est_info->get_table_statis(table_item->table_id, table_statis));
    double op_cost = EstCostUtil::cost_update(table_statis->table_rows, 
                                              table_statis->block_count, 
                                              output_rows);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

u32 LogUpdate::allocate_expr_pre()
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
    print_exprs(assign_exprs, "assignment", info);
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}