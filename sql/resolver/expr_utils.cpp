#include "expr_utils.h"
#include "dml_stmt.h"
#include "expr_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;

u32 ExprUtils::get_column_exprs(Vector<ExprStmt_s> &exprs, 
                                u32 table_id, 
                                Vector<ColumnStmt_s> &columns)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < exprs.size(); ++i) {
        MY_ASSERT(exprs[i]);
        if (!exprs[i]->has_flag(HAS_COLUMN)) {
            //do nothing
        } else if (INVALID_ID != table_id && 
            !exprs[i]->get_table_ids().has_member(table_id)) {
            //do nothing
        } else {
            CHECK(get_column_exprs(exprs[i], table_id, columns));
        }
    }
    return ret;
}

u32 ExprUtils::get_column_exprs(ExprStmt_s& expr, 
                                u32 table_id, 
                                Vector<ColumnStmt_s> &columns)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr);
    if (expr->has_flag(IS_COLUMN)) {
        if (INVALID_ID == table_id) {
            columns.push_back(expr);
        } else {
            ColumnStmt_s col = expr;
            if (col->table_id == table_id) {
                columns.push_back(col);
            }
        }
    } else {
        for (u32 i = 0; i < expr->get_params().size(); ++i) {
            MY_ASSERT(expr->get_params()[i]);
            if (!expr->get_params()[i]->has_flag(HAS_COLUMN)) {
                //do nothing
            } else if (INVALID_ID != table_id && 
                !expr->get_params()[i]->get_table_ids().has_member(table_id)) {
                //do nothing
            } else {
                CHECK(get_column_exprs(expr->get_params()[i], table_id, columns));
            }
        }
    }
    return ret;
}

bool ExprUtils::find_equal_expr(const Vector<ExprStmt_s> &exprs, 
                                const ExprStmt_s& expr, 
                                u32 *index)
{
	bool find = false;
	for (u32 i = 0; !find && i < exprs.size(); ++i) {
		if (exprs[i]->same_as(expr)) {
			find = true;
			if (index != nullptr) {
				*index = i;
			}
		}
	}
	return find;
}

u32 ExprUtils::replace_expr(const Vector<ExprStmt_s> &old_exprs, 
                            const Vector<ExprStmt_s> &new_exprs, 
                            ExprStmt_s& expr)
{
    u32 ret = SUCCESS;
    u32 index = 0;
    if (find_equal_expr(old_exprs, expr, &index)) {
        MY_ASSERT(index >=0 && index < new_exprs.size());
        expr = new_exprs[index];
    } else if (!expr->params.empty()) {
        CHECK(replace_exprs(old_exprs, new_exprs, expr->params));
    }
    return ret;
}

u32 ExprUtils::replace_exprs(const Vector<ExprStmt_s> &old_exprs, 
                            const Vector<ExprStmt_s> &new_exprs, 
                            Vector<ExprStmt_s> &exprs)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(replace_expr(old_exprs, new_exprs, exprs[i]));
    }
    return ret;
}

u32 ExprUtils::deep_copy_exprs(const Vector<ExprStmt_s> &old_exprs, 
                               Vector<ExprStmt_s> &new_exprs,
                               QueryCtx_s &ctx,
                               u32 flag)
{
    u32 ret = SUCCESS;
    ExprStmt_s expr;
    for (u32 i = 0; i < old_exprs.size(); ++i) {
        CHECK(deep_copy_expr(old_exprs[i], expr, ctx, flag));
        new_exprs.push_back(expr);
    }
    return ret;
}

u32 ExprUtils::deep_copy_expr(const ExprStmt_s &old_expr, 
                              ExprStmt_s &new_expr,
                              QueryCtx_s &ctx,
                              u32 flag)
{
    u32 ret = SUCCESS;
    if (old_expr->has_flag(IS_COLUMN) &&
       !ENABLE_COPY_SHARE(flag)) {
        new_expr = old_expr;
    } else if (old_expr->has_flag(IS_EXEC_PARAM) ||
               CONST == old_expr->expr_type()) {
        new_expr = old_expr;
    } else {
        CHECK(old_expr->deep_copy(new_expr, ctx, flag));
    }
    return ret;
}

u32 ExprUtils::make_row_id_expr(const String& table_name, 
                                u32 table_id, 
                                ColumnStmt_s &row_id)
{
    u32 ret = SUCCESS;
    row_id = ColumnStmt::make_column_stmt(table_name, "ROWID");
    row_id->table_id = table_id;
    row_id->column_id = ROWID_COLUMN_ID;
    CHECK(row_id->formalize());
    return ret;
}

u32 ExprUtils::is_null_reject_expr(ExprStmt_s &expr, bool &reject_null)
{
    u32 ret = SUCCESS;
    if (expr->has_flag(IS_COLUMN)) {
        reject_null = false;
    } else {
        reject_null = true;
    }
    return ret;
}

u32 ExprUtils::make_is_null_expr(ExprStmt_s &old_expr, ExprStmt_s &new_expr)
{
    u32 ret = SUCCESS;
    new_expr = OpExprStmt::make_op_expr_stmt(OP_IS_NULL);
	new_expr->params.push_back(old_expr); 
    CHECK(new_expr->formalize());
    return ret;
}

u32 ExprUtils::make_null_expr(ExprStmt_s &null_expr)
{
    u32 ret = SUCCESS;
    Object_s value = Object::make_null_object();
    null_expr = ConstStmt::make_const_stmt(value);
    CHECK(null_expr->formalize());
    return ret;
}