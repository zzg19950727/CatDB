#include "dml_stmt.h"
#include "expr_stmt.h"

using namespace CatDB::Parser;

DMLStmt::DMLStmt()
    :is_explain(false)
{

}

DMLStmt::~DMLStmt()
{

}

u32 DMLStmt::formalize()
{
    u32 ret = SUCCESS;
    table_ids.clear();
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->formalize());
        table_ids.add_members(from_stmts[i]->table_ids);
    }
    for (u32 i = 0; i < where_stmt.size(); ++i) {
        CHECK(where_stmt[i]->formalize());
    }
    collect_special_exprs();
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
    if (ExprStmt::Column == expr->expr_type()) {
        column_exprs.push_back(expr);
    } else if (ExprStmt::Aggregate == expr->expr_type()) {
        aggr_exprs.push_back(expr);
    } else if (ExprStmt::SubQuery == expr->expr_type()) {
        subquery_exprs.push_back(expr);
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
        if (column_exprs[i]->table_ids.has_member(table_id)) {
            columns.push_back(column_exprs[i]);
        }
    }
    return ret;
}