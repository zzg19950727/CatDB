#include "log_insert.h"
#include "expr_stmt.h"
#include "opt_est_cost.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogInsert::make_insert(LogicalOperator_s &child, BasicTableStmt_s &insert_table)
{
    LogInsert *insert = new LogInsert(child);
    insert->table_item = insert_table;
    return LogicalOperator_s(insert);
}

u32 LogInsert::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = child()->get_output_rows();
    return ret;
}

u32 LogInsert::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_insert(output_rows);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
}

void LogInsert::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "INSERT";
    info.name = table_item->alias_name;
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}