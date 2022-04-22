#include "transform_full_outer_join.h"
#include "transform_utils.h"
#include "select_stmt.h"
#include "query_ctx.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformFullOuterJoin::TransformFullOuterJoin()
    :TransformRule(POST_ORDER)
{

}

TransformFullOuterJoin::~TransformFullOuterJoin()
{

}

bool TransformFullOuterJoin::need_rewrite(DMLStmt_s stmt) const
{
    return true;
}

u32 TransformFullOuterJoin::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        CHECK(transform_one_table(stmt, stmt->from_stmts[i]));
    }
    return ret;
}

u32 TransformFullOuterJoin::transform_one_table(DMLStmt_s &stmt, TableStmt_s &table)
{
    u32 ret = SUCCESS;
    if (table->is_joined_table()) {
        bool is_valid;
        JoinedTableStmt_s joined_table = table;
        CHECK(transform_one_table(stmt, joined_table->left_table));
        CHECK(transform_one_table(stmt, joined_table->right_table));
        if (transform_happened()) {
            CHECK(table->formalize());
        }
        CHECK(check_table_need_transform(joined_table, is_valid));
        if (is_valid) {
            CHECK(do_transform(stmt, table));
            set_transform_happened();
        }
    }
    return ret;
}

u32 TransformFullOuterJoin::check_table_need_transform(JoinedTableStmt_s &joined_table, bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (FullOuter == joined_table->join_type) {
        bool has_equal_cond = false;
        is_valid = true;
        for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
            CHECK(check_join_condition(joined_table->join_condition[i], 
                                       joined_table->left_table->table_ids,
                                       joined_table->right_table->table_ids,
                                       has_equal_cond));
            if (has_equal_cond) {
                is_valid = false;
                break;
            }
        }
    }
    return ret;
}

u32 TransformFullOuterJoin::check_join_condition(ExprStmt_s &expr, 
                                                 BitSet &left_table_ids,
                                                 BitSet &right_table_ids,
                                                 bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (OP_EXPR == expr->expr_type()) {
        OpExprStmt_s op_expr = expr;
        if (OP_EQ == op_expr->op_type) {
            MY_ASSERT(op_expr->params.size() == 2);
            if (op_expr->params[0]->table_ids.is_empty() ||
                op_expr->params[1]->table_ids.is_empty()) {
                //do nothing    
            } else if (op_expr->params[0]->table_ids.is_subset(left_table_ids) &&
                       op_expr->params[1]->table_ids.is_subset(right_table_ids)) {
                is_valid = true;           
            } else if (op_expr->params[1]->table_ids.is_subset(left_table_ids) &&
                       op_expr->params[0]->table_ids.is_subset(right_table_ids)) {
                is_valid = true;           
            }
        }
    }
    return ret;
}

u32 TransformFullOuterJoin::do_transform(DMLStmt_s &stmt, TableStmt_s &table)
{
    u32 ret = SUCCESS;
    Vector<TableStmt_s> table_items;
    Vector<ExprStmt_s> columns;
    MY_ASSERT(table->is_joined_table());
    JoinedTableStmt_s joined_table = table;
    CHECK(joined_table->get_table_items(table_items));
    for (u32 i = 0; i < table_items.size(); ++i) {
        CHECK(stmt->get_column_exprs(table_items[i]->table_id, columns));
    }
    SelectStmt_s left_join_view;
    SelectStmt_s right_anti_view;
    CHECK(TransformUtils::create_view_with_table_item(joined_table,
                                                      left_join_view, 
                                                      ctx));
    left_join_view->select_expr_list = columns;
    CHECK(TransformUtils::deep_copy_stmt(left_join_view, 
                                         right_anti_view, 
                                         ctx->query_ctx));
    MY_ASSERT(left_join_view->from_stmts.size() == 1);
    MY_ASSERT(right_anti_view->from_stmts.size() == 1);
    MY_ASSERT(left_join_view->from_stmts[0]->is_joined_table());
    MY_ASSERT(right_anti_view->from_stmts[0]->is_joined_table());
    JoinedTableStmt_s left_join = left_join_view->from_stmts[0];
    JoinedTableStmt_s right_anti = right_anti_view->from_stmts[0];
    left_join->join_type = LeftOuter;
    right_anti->join_type = RightAnti;
    CHECK(output_null_for_anti(right_anti_view));
    SelectStmt_s set_stmt;
    CHECK(TransformUtils::create_set_stmt(left_join_view, 
                                          right_anti_view, 
                                          UNION_ALL, 
                                          set_stmt, 
                                          ctx));
    table = ViewTableStmt::make_view_table(set_stmt);
    table->table_id = ctx->query_ctx->generate_table_id();
    table->alias_name = ctx->query_ctx->generate_view_name();
    ViewTableStmt_s view_table = table;
    Vector<ExprStmt_s> new_column_list;
    CHECK(TransformUtils::create_column_for_view_table(view_table, new_column_list));
    CHECK(stmt->replace_stmt_exprs(columns, new_column_list));
    return ret;
}

u32 TransformFullOuterJoin::output_null_for_anti(SelectStmt_s &view)
{
    u32 ret = SUCCESS;
    MY_ASSERT(view, view->from_stmts.size() == 1);
    MY_ASSERT(view->from_stmts[0]->is_joined_table());
    JoinedTableStmt_s right_anti = view->from_stmts[0];
    BitSet &left_table_ids = right_anti->left_table->table_ids;
    for (u32 i = 0; i < view->select_expr_list.size(); ++i) {
        MY_ASSERT(COLUMN == view->select_expr_list[i]->expr_type());
        ColumnStmt_s column = view->select_expr_list[i];
        if (left_table_ids.has_member(column->table_id)) {
            ExprStmt_s null_value = ConstStmt::make_const_stmt(Common::Object::make_null_object());
            CHECK(null_value->formalize());
            view->select_expr_list[i] = null_value;
        }
    }
    return ret;
}