#include "transform_merge_view.h"
#include "transform_utils.h"
#include "select_stmt.h"
#include "query_ctx.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformMergeView::TransformMergeView()
    :TransformRule(POST_ORDER, MERGE)
{

}

TransformMergeView::~TransformMergeView()
{

}

u32 TransformMergeView::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    TransfomHelper helper;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        helper.reset();
        CHECK(transform_one_table(stmt, stmt->from_stmts[i], helper));
    }
    return ret;
}

u32 TransformMergeView::transform_one_table(DMLStmt_s &stmt, 
                                            TableStmt_s &table, 
                                            TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        helper.reset();
        if (RightOuter == joined_table->join_type || 
            FullOuter == joined_table->join_type) {
            helper.is_outer_right_view = true;
        }
        CHECK(transform_one_table(stmt, 
                                  joined_table->left_table, 
                                  helper));
        helper.reset();
        if (LeftOuter == joined_table->join_type || 
            FullOuter == joined_table->join_type) {
            helper.is_outer_right_view = true;
        }
        CHECK(transform_one_table(stmt, 
                                  joined_table->right_table, 
                                  helper));
    } else if (table->is_view_table()) {
        bool is_valid = false;
        CHECK(check_table_need_transform(stmt,
                                         table, 
                                         helper, 
                                         is_valid));
        if (is_valid) {
            ViewTableStmt_s view_table = table;
            SelectStmt_s view = view_table->ref_query;
            bool is_disable = false;
            CHECK(check_hint_disable(view, is_disable));
            if (!is_disable) {
                CHECK(do_transform(stmt, table, helper));
                CHECK(generate_outline(view));
                set_transform_happened();
            }
        }
    }
    return ret;
}

u32 TransformMergeView::check_table_need_transform(DMLStmt_s &stmt, 
                                                   TableStmt_s &table, 
                                                   TransfomHelper &helper, 
                                                   bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    ViewTableStmt_s view_table = table;
    SelectStmt_s view = view_table->ref_query;
    if (view->is_set_stmt() ||
        view->has_group_by() || 
        view->has_order_by() || 
        view->has_distinct() ||
        view->has_window_func() ||
        view->has_limit()) {
        if (!stmt->is_select_stmt() ||
            view->is_set_stmt() ||
            stmt->where_stmt.size() > 0) {
            is_valid = false;
            return ret;
        }
        SelectStmt_s sel_stmt = stmt;
        CHECK(TransformUtils::is_simple_stmt(sel_stmt, is_valid));
        helper.is_query_pushdown = true;
    } else if (helper.is_outer_right_view) {
        DMLStmt_s dml_stmt = view;
        CHECK(find_not_null_row_id(dml_stmt, helper.row_id_expr, is_valid));
    } else {
        is_valid = true;
    }
    return ret;
}

u32 TransformMergeView::find_not_null_row_id(DMLStmt_s &stmt,
                                             ExprStmt_s &row_id,
                                             bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    ColumnStmt_s row_id_col;
    //find basic from table
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        if (stmt->from_stmts[i]->is_basic_table()) {
            TableStmt_s &basic_table = stmt->from_stmts[i];
            CHECK(ExprUtils::make_row_id_expr(basic_table->alias_name,
                                              basic_table->table_id,
                                              row_id_col));
            is_valid = true;
            row_id = row_id_col;
            return ret;
        } else if (stmt->from_stmts[i]->is_joined_table()) {
            TableStmt_s table = stmt->from_stmts[i];
            while(table->is_joined_table()) {
                JoinedTableStmt_s joined_table = table;
                if (LeftOuter == joined_table->join_type) {
                    table = joined_table->left_table;
                } else if (RightOuter == joined_table->join_type) {
                    table = joined_table->right_table;
                } else if (FullOuter == joined_table->join_type) {
                    break;
                }
            }
            if (table->is_basic_table()) {
                CHECK(ExprUtils::make_row_id_expr(table->alias_name,
                                                table->table_id,
                                                row_id_col));
                is_valid = true;
                row_id = row_id_col;
                return ret;
            }
        }
    }
    return ret;
}

u32 TransformMergeView::do_transform(DMLStmt_s &stmt, 
                                     TableStmt_s &table, 
                                     TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    ViewTableStmt_s view_table = table;
    SelectStmt_s view = view_table->ref_query;
    Vector<ExprStmt_s> select_exprs;
    Vector<ExprStmt_s> new_select_exprs;
    Vector<ExprStmt_s> columns;
    TableStmt_s new_table;
    CHECK(TransformUtils::get_view_table_columns(stmt, 
                                                 view_table, 
                                                 columns, 
                                                 select_exprs));
    CHECK(TransformUtils::build_joined_table(view->from_stmts, new_table));
    append(new_table->table_filter, view->where_stmt);
    if (helper.is_query_pushdown) {
        SelectStmt_s parent_stmt = stmt;
        append(parent_stmt->group_exprs, view->group_exprs);
		append(parent_stmt->having_stmt, view->having_stmt);
		append(parent_stmt->order_exprs, view->order_exprs);
		parent_stmt->limit_stmt = view->limit_stmt;
        parent_stmt->is_distinct = view->is_distinct;
    }
    if (helper.is_outer_right_view) {
        CHECK(try_add_case_when(select_exprs, 
                                helper.row_id_expr, 
                                new_select_exprs));
    } else {
        new_select_exprs = select_exprs;
    }
    CHECK(stmt->replace_stmt_exprs(columns, new_select_exprs));
    table = new_table;
    set_transform_happened();
    return ret;
}

u32 TransformMergeView::try_add_case_when(Vector<ExprStmt_s> &old_exprs,
                                          ExprStmt_s &not_null_expr,
                                          Vector<ExprStmt_s> &new_exprs)
{
    u32 ret = SUCCESS;
    bool is_reject_null = false;
    ExprStmt_s new_expr;
    ExprStmt_s is_null_expr;
    ExprStmt_s null_expr;
    CHECK(ExprUtils::make_is_null_expr(not_null_expr, is_null_expr));
    CHECK(ExprUtils::make_null_expr(null_expr));
    for (u32 i = 0; i < old_exprs.size(); ++i) {
        CHECK(ExprUtils::is_null_reject_expr(old_exprs[i], is_reject_null));
        if (!is_reject_null) {
            new_exprs.push_back(old_exprs[i]);
            continue;
        }
        new_expr = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
	    new_expr->params.push_back(is_null_expr); 
        new_expr->params.push_back(null_expr); 
        new_expr->params.push_back(old_exprs[i]); 
        new_exprs.push_back(new_expr);
    }
    return ret;
}

u32 TransformMergeView::check_hint_disable(SelectStmt_s& view, bool &is_disable)
{
    u32 ret = SUCCESS;
    is_disable = false;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    is_disable = query_hint.enable_no_merge(view->get_qb_name());
    return ret;
}

u32 TransformMergeView::generate_outline(SelectStmt_s& view)
{
    u32 ret = SUCCESS;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    CHECK(query_hint.generate_transform_outline(view->get_qb_name(), control_hint));
    return ret;
}