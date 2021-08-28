#include "log_dual_table.h"
#include "log_table_scan.h"
#include "log_subquery_evaluate.h"
#include "log_view.h"
#include "dml_plan.h"
#include "dml_stmt.h"
#include "expr_utils.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

DMLPlan::DMLPlan()
{

}

DMLPlan::~DMLPlan()
{

}

u32 DMLPlan::build_plan()
{
    u32 ret = SUCCESS;
    CHECK(generate_plan_tree());
    MY_ASSERT(root_operator);
    CHECK(root_operator->visit_plan());
    return ret;
}

u32 DMLPlan::pushdown_quals()
{
    return SUCCESS;
}

u32 DMLPlan::generate_join_operator(LogicalOperator_s &left,
                                LogicalOperator_s &right,
                                JoinType join_type,
                                Vector<ExprStmt_s> &join_condition,
                                LogJoin::JoinAlgo join_algo,
                                LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    LogJoin_s join = LogJoin::make_join(left, right, join_type, join_algo);
    join->set_query_ctx(query_ctx);
    join->other_join_condition = join_condition;
    op = join;
    return ret;
}

u32 DMLPlan::generate_join_order_with_joined_table(JoinedTableStmt_s table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt != NULL);
    LogicalOperator_s left_op, right_op;
    CHECK(generate_join_order_with_table_item(table_stmt->left_table, left_op));
    CHECK(generate_join_order_with_table_item(table_stmt->right_table, right_op));
    CHECK(generate_join_operator(left_op, 
                                 right_op,
                                 table_stmt->join_type,
                                 table_stmt->join_condition,
                                 LogJoin::NL_JOIN,
                                 op));
    return ret;
}

u32 DMLPlan::generate_join_order_with_basic_table(BasicTableStmt_s table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt != NULL);
    if (table_stmt->is_dual) {
        op = LogDualTable::make_dual_table(table_stmt);
    } else {
        op = LogTableScan::make_table_scan(table_stmt);
        CHECK(set_table_access_columns(op));
    }
    op->set_query_ctx(query_ctx);
    return ret;
}

u32 DMLPlan::generate_join_order_with_view_table(ViewTableStmt_s table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt != NULL);
    CHECK(generate_sub_select_plan_tree(table_stmt->ref_query, op));
    op = LogView::make_view(table_stmt, op);
    op->set_query_ctx(query_ctx);
    CHECK(set_table_access_columns(op));
    return ret;
}

u32 DMLPlan::generate_join_order_with_table_item(TableStmt_s& table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    if (table_stmt->is_basic_table()) {
        CHECK(generate_join_order_with_basic_table(table_stmt, op));
    } else if (table_stmt->is_joined_table()) {
        CHECK(generate_join_order_with_joined_table(table_stmt, op));
    } else if (table_stmt->is_view_table()) {
        CHECK(generate_join_order_with_view_table(table_stmt, op));
    } else {
        LOG_ERR("unknow table type", K(table_stmt));
        ret = ERROR_LEX_STMT;
    }
    return ret;
}

u32 DMLPlan::generate_join_order()
{
    u32 ret = SUCCESS;
    MY_ASSERT(lex_stmt, lex_stmt->stmt_type() !=  Stmt::DoCMD)
    DMLStmt_s stmt = lex_stmt;
    Vector<ExprStmt_s> dummy_join_condition;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        LogicalOperator_s op;
        CHECK(generate_join_order_with_table_item(stmt->from_stmts[i], op));
        if (!root_operator) {
            root_operator = op;
        } else {
            CHECK(generate_join_operator(root_operator,
                                        op,
                                        Inner,
                                        dummy_join_condition,
                                        LogJoin::NL_JOIN,
                                        root_operator));
        }
    }
    if (root_operator) {
        root_operator->filters = stmt->where_stmt;
    }
    return ret;
}

u32 DMLPlan::generate_sub_select_plan_tree(SelectStmt_s &sub_select, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
	Plan_s sub_plan = Plan::make_plan(sub_select);
    sub_plan->set_query_ctx(query_ctx);
	CHECK(sub_plan->generate_plan_tree());
    op = sub_plan->get_root_operator();
	return ret;
}

u32 DMLPlan::set_table_access_columns(LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> columns;
    DMLStmt_s stmt = lex_stmt;
    u32 table_id = INVALID_ID;
    MY_ASSERT(stmt, op, LogicalOperator::LOG_TABLE_SCAN == op->type() || LogicalOperator::LOG_VIEW == op->type());
    if (LogicalOperator::LOG_TABLE_SCAN == op->type()) {
        LogTableScan_s table_scan = op;
        table_id = table_scan->table_item->table_id;
    } else if (LogicalOperator::LOG_VIEW == op->type()) {
        LogView_s view = op;
        table_id = view->table_item->table_id;
    }
    stmt->get_column_exprs(table_id, columns);
    append(op->access_exprs, columns);
    return ret;
}

u32 DMLPlan::generate_subplan()
{
    u32 ret = SUCCESS;
    DMLStmt_s stmt = lex_stmt;
    Vector<SubQueryStmt_s> &subquery_exprs = stmt->get_subquery_exprs();
    if (subquery_exprs.empty()) {
        return ret;
    }
    MY_ASSERT(root_operator);
    LogSubQueryEvaluate_s subquery_evaluate = LogSubQueryEvaluate::make_subquery_evaluate(root_operator);
    subquery_evaluate->set_query_ctx(query_ctx);
    for (u32 i = 0; i < subquery_exprs.size(); ++i) {
        SubQueryStmt_s &subquery = subquery_exprs[i];
        subquery->set_subquery_id(i);
        LogicalOperator_s subplan;
        CHECK(generate_sub_select_plan_tree(subquery->query_stmt, subplan));
        subquery_evaluate->add_subplan(subquery, subplan);
    }
    root_operator = subquery_evaluate;
    return ret;
}