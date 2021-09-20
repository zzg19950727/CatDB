#include "log_table_scan.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogTableScan::make_table_scan(BasicTableStmt_s &table)
{
    return LogicalOperator_s(new LogTableScan(table));
}

u32  LogTableScan::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_produce, access_exprs);
    CHECK(LogicalOperator::allocate_expr_pre());
    return ret;
}

void LogTableScan::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "TABLE SCAN";
    info.name = table_item->alias_name;
    info.expr_info += "access";
    print_exprs(access_exprs, info.expr_info);
    plan_info.push_back(info);
}