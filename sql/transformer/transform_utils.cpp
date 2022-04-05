#include "transform_utils.h"
#include "transformer.h"
#include "select_stmt.h"
#include "table_stmt.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "query_ctx.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

u32 TransformUtils::deep_copy_stmt(const SelectStmt_s &old_stmt, 
                                    SelectStmt_s &new_stmt, 
                                    QueryCtx_s &ctx,
                                    u32 flag)
{
    u32 ret = SUCCESS;
    MY_ASSERT(ctx);
    CHECK(old_stmt->deep_copy(new_stmt, ctx, flag));
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
        u32 new_table_id = ctx->generate_table_id();
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
    CHECK(new_stmt->reset_stmt_id(ctx->generate_stmt_id()));
    CHECK(new_stmt->formalize());
    return ret;
}

u32 TransformUtils::create_view_with_table_item(const TableStmt_s &table,
                                                SelectStmt_s &view,
                                                TransformCtx_s &ctx)
{
    u32 ret = SUCCESS;
    MY_ASSERT(ctx, ctx->query_ctx);
    view = SelectStmt::make_select_stmt();
    CHECK(view->reset_stmt_id(ctx->query_ctx->generate_stmt_id()));
    view->from_stmts.push_back(table);
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
                                       SelectStmt_s &set_stmt,
                                       TransformCtx_s &ctx)
{
    u32 ret = SUCCESS;
    MY_ASSERT(ctx, ctx->query_ctx);
    MY_ASSERT(left_query->select_expr_list.size() == right_query->select_expr_list.size());
    set_stmt = SetStmt::make_set_stmt(left_query, right_query, set_op);
    CHECK(set_stmt->reset_stmt_id(ctx->query_ctx->generate_stmt_id()));
    for (u32 i = 0; i < left_query->select_expr_list.size(); ++i) {
        SetExprStmt_s expr = SetExprStmt::make_set_expr(set_op, i);
        expr->res_type = left_query->select_expr_list[i]->res_type;
        expr->alias_name = left_query->select_expr_list[i]->alias_name;
        set_stmt->select_expr_list.push_back(expr);
    }
    return ret;
}