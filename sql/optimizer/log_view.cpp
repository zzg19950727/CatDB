#include "log_view.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "select_stmt.h"
#include "opt_est_sel.h"
#include "opt_est_cost.h"
#include "opt_est_info.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogView::make_view(ViewTableStmt_s &table, LogicalOperator_s &query_root)
{
    return LogicalOperator_s(new LogView(query_root, table));
}

u32 LogView::est_row_count()
{
    u32 ret = SUCCESS;
    TableEstInfo_s table_statis = TableEstInfo::make_table_est_info();
    table_statis->table_rows = child()->get_output_rows();
    table_statis->table_id = table_item->table_id;
    table_statis->block_count = 0.0;
    Vector<ExprStmt_s> &select_list = table_item->ref_query->select_expr_list;
    for (u32 i = 0; i < select_list.size(); ++i) {
        ColumnEstInfo_s column_statis = ColumnEstInfo::make_column_est_info();
        CHECK(EstSelUtil::calc_distinct_count(child()->est_info, select_list[i], column_statis->ori_ndv));
        column_statis->column_id = i;
        column_statis->null_count = 0;
        column_statis->min_value = 0;
        column_statis->max_value = 10000;
        table_statis->columns_statis[i] = column_statis;
    }
    CHECK(est_info->add_view_statis(table_item->table_id, table_statis));
    double sel = 1.0;
    CHECK(EstSelUtil::calc_selectivity(est_info, filters, sel));
    output_rows = child()->get_output_rows() * sel;
    set_output_rows(output_rows);
    CHECK(est_info->set_table_output_rows(table_item->table_id, output_rows));
    return ret;
}

u32 LogView::est_cost()
{
    u32 ret = SUCCESS;
    double op_cost = EstCostUtil::cost_filters(child()->get_output_rows(), filters);
    cost = op_cost;
    cost += child()->get_cost();
    return ret;
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
    }
    plan_info.push_back(info);
    child()->print_plan(depth + 1, plan_info);
}