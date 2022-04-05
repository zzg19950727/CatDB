#include "log_delete.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "opt_est_cost.h"
#include "opt_est_info.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogDelete::make_delete(LogicalOperator_s &child,
                                        BasicTableStmt_s &delete_table,
                                        ColumnStmt_s& row_id,
                                        bool is_delete_all)
{
    LogDelete *log_delete = new LogDelete(child);
    log_delete->table_item = delete_table;
    log_delete->row_id = row_id;
    log_delete->is_delete_all = is_delete_all;
    return LogicalOperator_s(log_delete);
}

u32 LogDelete::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = child()->get_output_rows();
    set_output_rows(output_rows);
    return ret;
}

u32 LogDelete::est_cost()
{
    u32 ret = SUCCESS;
    TableEstInfo_s table_statis;
    CHECK(est_info->get_table_statis(table_item->table_id, table_statis));
    double op_cost = EstCostUtil::cost_delete(table_statis->table_rows, 
                                              table_statis->block_count, 
                                              output_rows, 
                                              is_delete_all);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

u32  LogDelete::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    expr_ctx.expr_consume.push_back(row_id);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogDelete::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "DELETE";
    info.name = table_item->alias_name;
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}