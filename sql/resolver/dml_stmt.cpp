#include "dml_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "select_stmt.h"

using namespace CatDB::Parser;


DMLStmt::DMLStmt()
    :ref_count(1), 
    is_explain(false)
{

}

DMLStmt::~DMLStmt()
{

}

u32 DMLStmt::formalize()
{
    u32 ret = SUCCESS;
    table_ids.clear();
    if (!stmt_hint.has_qb_name()) {
        stmt_hint.generate_qb_name(stmt_id);
    }
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->formalize());
        table_ids.add_members(from_stmts[i]->table_ids);
    }
    for (u32 i = 0; i < where_stmt.size(); ++i) {
        CHECK(where_stmt[i]->formalize());
    }
    CHECK(collect_special_exprs());
    return ret;
}

u32 DMLStmt::get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
    u32 ret = SUCCESS;
    CHECK(inner_get_stmt_exprs(exprs));
    return ret;
}

u32 DMLStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
    u32 ret = SUCCESS;
    append(exprs, where_stmt);
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->get_table_exprs(exprs));
    }
    return ret;
}

u32 DMLStmt::replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                const Vector<ExprStmt_s> &new_exprs,
                                const bool recurse_child_stmt)
{
    u32 ret = SUCCESS;
    CHECK(inner_replace_stmt_exprs(old_exprs, new_exprs));
    if (recurse_child_stmt) {
        Vector<SelectStmt_s> child_stms;
        CHECK(get_child_stmts(child_stms));
        for (u32 i = 0; i < child_stms.size(); ++i) {
            CHECK(child_stms[i]->replace_stmt_exprs(old_exprs, 
                                                    new_exprs, 
                                                    recurse_child_stmt));
        }
    }
    return ret;
}

u32 DMLStmt::inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   	  const Vector<ExprStmt_s> &new_exprs)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->replace_exprs(old_exprs, new_exprs));
    }
    CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, where_stmt));
    return ret;
}

u32 DMLStmt::collect_special_exprs()
{
    u32 ret = SUCCESS;
    column_exprs.clear();
    aggr_exprs.clear();
    subquery_exprs.clear();
    Vector<ExprStmt_s> all_exprs;
    CHECK(get_stmt_exprs(all_exprs));
    for (u32 i = 0; i < all_exprs.size(); ++i) {
        CHECK(collect_special_exprs(all_exprs[i]));
    }
    return ret;
}

u32 DMLStmt::collect_special_exprs(ExprStmt_s& expr)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr);
    if (COLUMN == expr->expr_type()) {
        column_exprs.push_back(expr);
    } else if (AGG_EXPR == expr->expr_type()) {
        aggr_exprs.push_back(expr);
    } else if (SUBQUERY == expr->expr_type()) {
        subquery_exprs.push_back(expr);
    } else if (WIN_EXPR == expr->expr_type()) {
        win_func_exprs.push_back(expr);
    }
    for (u32 i = 0; i < expr->get_params().size(); ++i) {
        CHECK(collect_special_exprs(expr->get_params()[i]));
    }
    return ret;
}

u32 DMLStmt::get_table_items(Vector<TableStmt_s> &tables)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        if (!from_stmts[i]->is_joined_table()) {
            tables.push_back(from_stmts[i]);
        } else {
            JoinedTableStmt_s joined_table = from_stmts[i];
            CHECK(joined_table->get_table_items(tables));
        }
    }
    return ret;
}

u32 DMLStmt::get_column_exprs(u32 table_id, Vector<ExprStmt_s> &columns)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < column_exprs.size(); ++i) {
        ColumnStmt_s col = column_exprs[i];
        if (table_id == col->table_id) {
			if (!ExprUtils::find_equal_expr(columns, column_exprs[i])) {
            	columns.push_back(column_exprs[i]);
			}
        }
    }
    return ret;
}

u32 DMLStmt::get_child_stmts(Vector<SelectStmt_s> &child_stms)
{
    u32 ret = SUCCESS;
    if (SetOperation == stmt_type()) {
        SetStmt *set_stmt = dynamic_cast<SetStmt*>(this);
        child_stms.push_back(set_stmt->left_query);
        child_stms.push_back(set_stmt->right_query);
    }
    Vector<TableStmt_s> table_items;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        TableStmt_s table = from_stmts[i];
        if (table->is_joined_table()) {
            JoinedTableStmt_s joined_table = table;
            CHECK(joined_table->get_table_items(table_items));
        } else if (table->is_view_table()) {
            table_items.push_back(table);
        }
    }
    for (u32 i = 0; i < table_items.size(); ++i) {
        if (table_items[i]->is_view_table()) {
            ViewTableStmt_s view_table = table_items[i];
            child_stms.push_back(view_table->ref_query);
        }
    }
    Vector<SubQueryStmt_s> &subquery_exprs = get_subquery_exprs();
    for (u32 i = 0; i < subquery_exprs.size(); ++i) {
        child_stms.push_back(subquery_exprs[i]->query_stmt);
    }
    return ret;
}

u32 DMLStmt::reset_stmt_id(u32 stmt_id)
{
    u32 ret = SUCCESS;
    this->stmt_id = stmt_id;
    stmt_hint.generate_qb_name(stmt_id);
    return ret;
}

bool DMLStmt::find_table_id(const String &table_name, u32 &table_id)
{
    bool find = false;
    Vector<TableStmt_s> table_items;
    get_table_items(table_items);
    for (u32 i = 0; i < table_items.size(); ++i) {
        if (table_items[i]->alias_name == table_name) {
            if (find) {
                return false;
            } else {
                table_id = table_items[i]->table_id;
                find = true;
            }
        }
    }
    return find;
}

u32 DMLStmt::inner_deep_copy(DMLStmt_s stmt, QueryCtx_s &ctx, u32 flag)const
{
    u32 ret = SUCCESS;
    MY_ASSERT(stmt);
    TableStmt_s copy_table;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->deep_copy(copy_table, ctx, flag));
        stmt->from_stmts.push_back(copy_table);
    }
    CHECK(ExprUtils::deep_copy_exprs(where_stmt, stmt->where_stmt, ctx, flag));
    //CHECK(stmt_hint.deep_copy(stmt->stmt_hint));
    stmt->stmt_id = stmt_id;
    stmt->is_explain = is_explain;
    return ret;
}