#include "log_dual_table.h"
#include "table_stmt.h"
#include "expr_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogDualTable::make_dual_table(BasicTableStmt_s &table)
{
    return LogicalOperator_s(new LogDualTable(table));
}

u32 LogDualTable::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = 1.0;
    set_output_rows(output_rows);
    return ret;
}

u32 LogDualTable::est_cost()
{
    u32 ret = SUCCESS;
    cost = 1.0;
    return ret;
}

void LogDualTable::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "DUAL TABLE";
    info.name = table_item->alias_name;
    plan_info.push_back(info);
}