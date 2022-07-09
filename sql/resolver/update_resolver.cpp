#include "update_resolver.h"
#include "obj_cast_util.h"
#include "update_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "error.h"
using namespace CatDB::Parser;
using namespace CatDB::Common;

UpdateResolver::UpdateResolver(UpdateStmt_s &stmt, ResolveCtx &resolve_ctx)
    :DMLResolver(stmt, resolve_ctx),
    update_stmt(stmt)
{
    
}

UpdateResolver::~UpdateResolver()
{

}
u32 UpdateResolver::resolve_stmt()
{
    u32 ret = SUCCESS;
    CHECK(resolve_stmt_hint());
    CHECK(resolve_from_stmt());
    CHECK(resolve_where_stmt());
    MY_ASSERT(update_stmt->table);
    BasicTableStmt_s &update_table_item = update_stmt->table;
    const String &table_name = update_table_item->table_name;
    bool find = false;
    for (u32 i = 0; !find && i < resolve_ctx.cur_tables.size(); ++i) {
        if (resolve_ctx.cur_tables[i]->alias_name == table_name &&
            resolve_ctx.cur_tables[i]->is_basic_and_not_dual_table()) {
            find = true;
            CHECK(ExprUtils::make_row_id_expr(table_name,
                                              resolve_ctx.cur_tables[i]->table_id,
                                              update_stmt->row_id_col));
            update_stmt->table = resolve_ctx.cur_tables[i];
        }
    }
    MY_ASSERT(find);
    CHECK(set_relation_for_assign_exprs());
    CHECK(resolve_exprs(update_stmt->update_assign_stmt, resolve_ctx));
    CHECK(check_assign_exprs());
    return ret;
}

u32 UpdateResolver::set_relation_for_assign_exprs()
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < update_stmt->update_assign_stmt.size(); ++i) {
        ExprStmt_s assign_expr = update_stmt->update_assign_stmt[i];
        ColumnStmt_s column_expr;
        MY_ASSERT(assign_expr, OP_EXPR == assign_expr->expr_type());
        OpExprStmt_s expr = assign_expr;
        MY_ASSERT(OP_ASSIGN == expr->op_type, 2 == expr->params.size());
        MY_ASSERT(COLUMN == expr->params[0]->expr_type());
        column_expr = expr->params[0];
        column_expr->table = update_stmt->table->alias_name;   
    }
    return ret;
}

u32 UpdateResolver::check_assign_exprs()
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < update_stmt->update_assign_stmt.size(); ++i) {
        ExprStmt_s value_expr;
        ColumnStmt_s column_expr;
        CHECK(check_assign_expr(update_stmt->update_assign_stmt[i],
                                column_expr,
                                value_expr));                    
    }
    return ret;
}

u32 UpdateResolver::check_assign_expr(ExprStmt_s &assign_expr,
                                      ColumnStmt_s &column_expr,
                                      ExprStmt_s &value_expr)
{
    u32 ret = SUCCESS;
    bool need_cast = false;
    MY_ASSERT(assign_expr, OP_EXPR == assign_expr->expr_type());
    OpExprStmt_s expr = assign_expr;
    MY_ASSERT(OP_ASSIGN == expr->op_type, 2 == expr->params.size());
    MY_ASSERT(COLUMN == expr->params[0]->expr_type());
    column_expr = expr->params[0];
    MY_ASSERT(column_expr->table_id == update_stmt->table->table_id);
    value_expr = expr->params[1];
    CHECK(ObjCastUtil::check_need_cast(value_expr->res_type, column_expr->res_type, need_cast));
    CHECK(ObjCastUtil::add_cast(value_expr, column_expr->res_type, value_expr));
    expr->params[1] = value_expr;
    return ret;
}