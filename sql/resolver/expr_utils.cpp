#include "expr_utils.h"
#include "dml_stmt.h"
#include "expr_stmt.h"
#include "error.h"
#include "log.h"

using namespace::CatDB::Parser;

u32 ExprUtils::get_column_exprs(Vector<ExprStmt_s> &exprs, u32 table_id, Vector<ColumnStmt_s> &columns)
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

u32 ExprUtils::get_column_exprs(ExprStmt_s& expr, u32 table_id, Vector<ColumnStmt_s> &columns)
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

bool ExprUtils::find_equal_expr(const Vector<ExprStmt_s> &exprs, const ExprStmt_s& expr, u32 *index)
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
