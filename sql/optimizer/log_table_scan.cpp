#include "log_table_scan.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "opt_est_sel.h"
#include "opt_est_cost.h"
#include "opt_est_info.h"
#include "error.h"
#include "log.h"

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

u32 LogTableScan::est_row_count()
{
    u32 ret = SUCCESS;
    double selectivity = 1.0;
    TableEstInfo_s table_statis;
    CHECK(est_info->init_table_statis(table_item->ref_table_id, table_item->table_id));
    CHECK(est_info->get_table_statis(table_item->table_id, table_statis));
    CHECK(EstSelUtil::calc_selectivity(est_info, filters, selectivity));
    output_rows = table_statis->table_rows * selectivity;
    set_output_rows(output_rows);
    CHECK(est_info->set_table_output_rows(table_item->table_id, output_rows));
    return ret;
}

u32 LogTableScan::est_cost()
{
    u32 ret = SUCCESS;
    TableEstInfo_s table_statis;
    CHECK(est_info->get_table_statis(table_item->table_id, table_statis));
    cost = EstCostUtil::cost_table_scan(table_statis->table_rows,
                                        table_statis->block_count,
                                        access_exprs);
    cost += EstCostUtil::cost_filters(table_statis->table_rows, filters);
    return ret;
}

void LogTableScan::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op = "TABLE SCAN";
    info.name = table_item->alias_name;
    if (!access_exprs.empty()) {
        auto compare_func = [](const ExprStmt_s& lhs, const ExprStmt_s& rhs) 
        { 
            if (COLUMN == lhs->expr_type() && COLUMN == rhs->expr_type()) {
                ColumnStmt_s l_col = lhs;
                ColumnStmt_s r_col = rhs;
                return l_col->column_id < r_col->column_id;
            } else {
                return true;
            }
        };
        std::sort(access_exprs.begin(), 
			      access_exprs.end(), 
			      compare_func);
        print_exprs(access_exprs, "access", info);
        print_exprs(access_exprs, "access", info);
    }
    plan_info.push_back(info);
}
