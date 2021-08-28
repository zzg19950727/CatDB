#include "log_delete.h"
#include "expr_stmt.h"

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