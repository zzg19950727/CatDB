#include "transform_utils.h"
#include "obj_cast_util.h"
#include "transformer.h"
#include "select_stmt.h"
#include "table_stmt.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "session_info.h"
#include "obj_number.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;
using namespace CatDB::Common;

u32 TransformUtils::deep_copy_stmt(const SelectStmt_s &old_stmt, 
                                    SelectStmt_s &new_stmt,
                                    u32 flag)
{
    u32 ret = SUCCESS;
    CHECK(old_stmt->deep_copy(new_stmt, flag));
    //update table ids
    Vector<u32> old_table_ids;
    Vector<u32> new_table_ids;
    Vector<TableStmt_s> table_items;
    CHECK(new_stmt->get_table_items(table_items));
    CHECK(new_stmt->collect_special_exprs());
    Vector<ExprStmt_s> &columns = new_stmt->get_column_exprs();
    for (u32 i = 0; i < table_items.size(); ++i) {
        TableStmt_s &table = table_items[i];
        old_table_ids.push_back(table->table_id);
        u32 new_table_id = QUERY_CTX->generate_table_id();
        new_table_ids.push_back(new_table_id);
        table->table_id = new_table_id;
    }
    u32 index = 0;
    bool find = false;
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnStmt_s column = columns[i];
        find = ExprUtils::find_item(old_table_ids, column->table_id, &index);
        MY_ASSERT(find, index >= 0, index < new_table_ids.size());
        column->table_id = new_table_ids[index];
    }
    //update stmt id
    CHECK(new_stmt->reset_stmt_id(QUERY_CTX->generate_stmt_id()));
    CHECK(new_stmt->formalize());
    return ret;
}

u32 TransformUtils::create_view_with_table_item(const TableStmt_s &table,
                                                SelectStmt_s &view)
{
    u32 ret = SUCCESS;
    view = SelectStmt::make_select_stmt();
    CHECK(view->reset_stmt_id(QUERY_CTX->generate_stmt_id()));
    view->from_stmts.push_back(table);
    return ret;
}

u32 TransformUtils::create_table_with_view(SelectStmt_s &view,
                                            ViewTableStmt_s &table)
{
    u32 ret = SUCCESS;
    table = ViewTableStmt::make_view_table(view);
    table->table_id = QUERY_CTX->generate_table_id();
    table->alias_name = QUERY_CTX->generate_view_name();
    return ret;
}

u32 TransformUtils::create_select_item_for_view_table(ViewTableStmt_s &view_table,
                                                      Vector<ExprStmt_s> &new_select_list,
                                                      Vector<ExprStmt_s> &new_column_list)
{
    u32 ret = SUCCESS;
    MY_ASSERT(view_table, view_table->ref_query);
    SelectStmt_s &view = view_table->ref_query;
    u32 start_column_id = view->select_expr_list.size();
    for (u32 i = 0; i < new_select_list.size(); ++i) {
        new_select_list[i]->alias_name = "C" + std::to_string(i+1);
        ColumnStmt_s column = ColumnStmt::make_column_stmt(view_table->alias_name, 
                                                           new_select_list[i]->alias_name);
        column->table_id = view_table->table_id;
        column->column_id = start_column_id + i;
        column->res_type = new_select_list[i]->res_type;
        new_column_list.push_back(column);
    }
    append(view->select_expr_list, new_select_list);
    return ret;
}

u32 TransformUtils::create_column_for_view_table(ViewTableStmt_s &view_table,
                                                 Vector<ExprStmt_s> &new_column_list)
{
    u32 ret = SUCCESS;
    MY_ASSERT(view_table, view_table->ref_query);
    SelectStmt_s &view = view_table->ref_query;
    for (u32 i = 0; i < view->select_expr_list.size(); ++i) {
        ColumnStmt_s column = ColumnStmt::make_column_stmt(view_table->alias_name, 
                                                           view->select_expr_list[i]->alias_name);
        column->table_id = view_table->table_id;
        column->column_id = i;
        column->res_type = view->select_expr_list[i]->res_type;
        new_column_list.push_back(column);
    }
    return ret;
}

u32 TransformUtils::create_set_stmt(const SelectStmt_s &left_query,
                                       const SelectStmt_s &right_query,
                                       SetOpType set_op,
                                       SelectStmt_s &set_stmt)
{
    u32 ret = SUCCESS;
    MY_ASSERT(left_query->select_expr_list.size() == right_query->select_expr_list.size());
    set_stmt = SetStmt::make_set_stmt(left_query, right_query, set_op);
    CHECK(set_stmt->reset_stmt_id(QUERY_CTX->generate_stmt_id()));
    for (u32 i = 0; i < left_query->select_expr_list.size(); ++i) {
        SetExprStmt_s expr = SetExprStmt::make_set_expr(set_op, i);
        expr->alias_name = left_query->select_expr_list[i]->alias_name;
        set_stmt->select_expr_list.push_back(expr);
    }
    SetStmt_s stmt = set_stmt;
    CHECK(ObjCastUtil::deduce_set_expr_type(stmt));
    return ret;
}

u32 TransformUtils::is_simple_stmt(SelectStmt_s &stmt, bool &is_simple)
{
    u32 ret = SUCCESS;
    is_simple = false;
    if (stmt->is_set_stmt() ||
        stmt->has_group_by() || 
        stmt->has_order_by() || 
        stmt->has_distinct() ||
        stmt->has_window_func() ||
        stmt->has_limit()) {
        is_simple = false;
    } else if (1 != stmt->from_stmts.size()) {
        is_simple = false;
    } else if (stmt->from_stmts[0]->is_joined_table()) {
        is_simple = false;
    } else {
        is_simple = true;
    }
    return ret;
}

u32 TransformUtils::get_view_table_columns(DMLStmt_s &stmt, 
                                            ViewTableStmt_s &view_table, 
                                            Vector<ExprStmt_s> &columns, 
                                            Vector<ExprStmt_s> &select_exprs)
{
    u32 ret = SUCCESS;
    CHECK(stmt->get_column_exprs(view_table->table_id, columns));
    SelectStmt_s view = view_table->ref_query;
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnStmt_s col = columns[i];
        MY_ASSERT(col->column_id < view->select_expr_list.size());
        select_exprs.push_back(view->select_expr_list[col->column_id]);
    }
    return ret;
}

u32 TransformUtils::build_joined_table(Vector<TableStmt_s> &table_items,
                                        TableStmt_s &joined_table)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_items.size() > 0);
    joined_table = table_items[0];
    for (u32 i = 1; i < table_items.size(); ++i) {
        JoinedTableStmt_s table = JoinedTableStmt::make_joined_table(joined_table, table_items[i], Inner);
        joined_table = table;
    }
    return ret;
}

u32 TransformUtils::has_correlated_expr(Vector<ExprStmt_s> &exprs,
                                        SubQueryStmt_s &subquery_expr,
                                        bool &is_valid)
{
    u32 ret = SUCCESS;
    bool has = false;
    is_valid = false;
    if (subquery_expr->exec_params.empty()) {
        return ret;
    }
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(has_correlated_expr(exprs[i], 
                                subquery_expr, 
                                has));
        is_valid |= has;
    }
    return ret;
}

u32 TransformUtils::has_correlated_expr(ExprStmt_s &expr,
                                        SubQueryStmt_s &subquery_expr,
                                        bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (subquery_expr->exec_params.empty()) {
        return ret;
    } else if (expr->has_flag(IS_EXEC_PARAM)) {
        Vector<ExprStmt_s> exec_params;
        append(exec_params, subquery_expr->exec_params);
        is_valid = ExprUtils::find_equal_expr(exec_params, expr);
    } else if (expr->has_flag(HAS_EXEC_PARAM)) {
        CHECK(has_correlated_expr(expr->params, 
                                  subquery_expr, 
                                  is_valid));
    }
    return ret;
}

u32 TransformUtils::check_stmt_correlated(DMLStmt_s stmt, 
                                          SubQueryStmt_s &subquery_expr, 
                                          bool &is_correlated)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> all_exprs;
    is_correlated = false;
    if (subquery_expr->exec_params.empty()) {
        return ret;
    }
    CHECK(stmt->get_stmt_exprs(all_exprs));
    CHECK(has_correlated_expr(all_exprs, 
                          subquery_expr, 
                          is_correlated));
    if (is_correlated) {
        return ret;
    }
    Vector<SelectStmt_s> child_stmts;
    CHECK(stmt->get_child_stmts(child_stmts));
    for (u32 i = 0; !is_correlated && i < child_stmts.size(); ++i) {
        CHECK(check_stmt_correlated(child_stmts[i], 
                                    subquery_expr, 
                                    is_correlated));
    }
    return ret;
}

u32 TransformUtils::create_dummy_select_expr(SelectStmt_s &stmt)
{
    u32 ret = SUCCESS;
    ExprStmt_s const_expr;
    CHECK(ExprUtils::make_int_expr(const_expr, 1));
    stmt->select_expr_list.clear();
    stmt->select_expr_list.push_back(const_expr);
    if (stmt->is_set_stmt()) {
        SetStmt_s set_stmt = stmt;
        CHECK(create_dummy_select_expr(set_stmt->left_query));
        CHECK(create_dummy_select_expr(set_stmt->right_query));
    }
    return ret;
}

u32 TransformUtils::is_dummy_select_expr(SelectStmt_s &stmt, bool &is_dummy)
{
    u32 ret = SUCCESS;
    is_dummy = false;
    if (stmt->select_expr_list.size() != 1) {
        return ret;
    }
    is_dummy = stmt->select_expr_list[0]->has_flag(IS_CONST);
    return ret;
}

u32 TransformUtils::get_table_ids(TableStmt_s &table, Vector<u32> &table_ids)
{
    u32 ret = SUCCESS;
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        get_table_ids(joined_table->left_table, table_ids);
        get_table_ids(joined_table->right_table, table_ids);
    } else {
        table_ids.push_back(table->table_id);
    }
    return ret;
}