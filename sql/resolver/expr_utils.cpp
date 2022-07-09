#include "expr_utils.h"
#include "dml_stmt.h"
#include "expr_stmt.h"
#include "obj_number.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;

OperationType ExprUtils::remove_op_any(OperationType type)
{
    static OperationType map[] = {OP_EQ,
                                OP_NE,
                                OP_GE,
                                OP_GT,
                                OP_LE,
                                OP_LT};
    if (OP_EQ_ANY <= type && type <= OP_LT_ANY) {
        type = map[type-OP_EQ_ANY];
    }
    return type;
}

OperationType ExprUtils::remove_op_all(OperationType type)
{
    static OperationType map[] = {OP_EQ,
                                OP_NE,
                                OP_GE,
                                OP_GT,
                                OP_LE,
                                OP_LT};
    if (OP_EQ_ALL <= type && type <= OP_LT_ALL) {
        type = map[type-OP_EQ_ALL];
    }
    return type;
}

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
        if (exprs[i] == expr ||
            exprs[i]->same_as(expr)) {
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
                               u32 flag)
{
    u32 ret = SUCCESS;
    ExprStmt_s expr;
    for (u32 i = 0; i < old_exprs.size(); ++i) {
        CHECK(deep_copy_expr(old_exprs[i], expr, flag));
        new_exprs.push_back(expr);
    }
    return ret;
}

u32 ExprUtils::deep_copy_expr(const ExprStmt_s &old_expr, 
                              ExprStmt_s &new_expr,
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
        CHECK(old_expr->deep_copy(new_expr, flag));
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
    row_id->set_row_id();
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

u32 ExprUtils::make_or_expr(Vector<ExprStmt_s> &old_exprs, ExprStmt_s &new_expr)
{
    u32 ret = SUCCESS;
    MY_ASSERT(2 <= old_exprs.size());
    ExprStmt_s l_expr = old_exprs[0];
    for (u32 i = 1; i < old_exprs.size(); ++i) {
        new_expr = OpExprStmt::make_op_expr_stmt(OP_OR);
        new_expr->params.push_back(l_expr);
        new_expr->params.push_back(old_exprs[i]);
        l_expr = new_expr;
    }
    CHECK(new_expr->formalize());
    return ret;
}

u32 ExprUtils::make_and_expr(Vector<ExprStmt_s> &old_exprs, ExprStmt_s &new_expr)
{
    u32 ret = SUCCESS;
    MY_ASSERT(2 <= old_exprs.size());
    ExprStmt_s l_expr = old_exprs[0];
    for (u32 i = 1; i < old_exprs.size(); ++i) {
        new_expr = OpExprStmt::make_op_expr_stmt(OP_AND);
        new_expr->params.push_back(l_expr);
        new_expr->params.push_back(old_exprs[i]);
        l_expr = new_expr;
    }
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

u32 ExprUtils::make_bool_expr(ExprStmt_s &bool_expr, bool value)
{
    u32 ret = SUCCESS;
    bool_expr = OpExprStmt::make_op_expr_stmt(OP_IS_NULL);
    Object_s num = Number::make_int_object(0);
    if (value) {
        num->set_null();
    }
    ExprStmt_s const_expr = ConstStmt::make_const_stmt(num);
	bool_expr->params.push_back(const_expr);
    CHECK(bool_expr->formalize());
    return ret;
}

u32 ExprUtils::make_int_expr(ExprStmt_s &int_expr, int value)
{
    u32 ret = SUCCESS;
    Object_s num = Number::make_int_object(value);
    int_expr = ConstStmt::make_const_stmt(num);
    CHECK(int_expr->formalize());
    return ret;
}

u32 ExprUtils::extract_aggr_exprs(Vector<ExprStmt_s> &exprs, Vector<ExprStmt_s> &aggr_exprs)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(extract_aggr_exprs(exprs[i], aggr_exprs));
    }
    return ret;
}

u32 ExprUtils::extract_aggr_exprs(ExprStmt_s &expr, Vector<ExprStmt_s> &aggr_exprs)
{
    u32 ret = SUCCESS;
    if (!expr->has_flag(HAS_AGG)) {
        return ret;
    } else if (expr->has_flag(IS_AGG)) {
        aggr_exprs.push_back(expr);
    } else {
        CHECK(extract_aggr_exprs(expr->params, aggr_exprs));
    }
    return ret;
}

u32 ExprUtils::extract_subquery_exprs(Vector<ExprStmt_s> &exprs, 
                                      Vector<SubQueryStmt_s> &subquery_exprs,
                                      bool ignore_aggr)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(extract_subquery_exprs(exprs[i], subquery_exprs));
    }
    return ret;
}

u32 ExprUtils::extract_subquery_exprs(ExprStmt_s &expr, 
                                      Vector<SubQueryStmt_s> &subquery_exprs,
                                      bool ignore_aggr)
{
    u32 ret = SUCCESS;
    if (!expr->has_flag(HAS_SUBQUERY)) {
        return ret;
    } else if (expr->has_flag(IS_SUBQUERY)) {
        subquery_exprs.push_back(expr);
    } else if (ignore_aggr && expr->has_flag(IS_AGG)) {
        //
    } else {
        CHECK(extract_subquery_exprs(expr->params, subquery_exprs));
    }
    return ret;
}

u32 ExprUtils::extract_subquery_exprs(Vector<ExprStmt_s> &exprs, 
                                      Vector<ExprStmt_s> &subquery_exprs,
                                      Vector<ExprStmt_s> &none_subquery_exprs)
{
    u32 ret =  SUCCESS;
    for (u32 i = 0; i < exprs.size(); ++i) {
        if (exprs[i]->has_flag(HAS_SUBQUERY)) {
            subquery_exprs.push_back(exprs[i]);
        } else {
            none_subquery_exprs.push_back(exprs[i]);
        }
    }
    return ret;
}

u32 ExprUtils::is_calculable_expr(ExprStmt_s &expr, bool &is_calculable)
{
    u32 ret = SUCCESS;
    is_calculable = !expr->has_flag(HAS_COLUMN) &&
                    !expr->has_flag(HAS_SET_EXPR) &&
                    !expr->has_flag(HAS_AGG) &&
                    !expr->has_flag(HAS_SUBQUERY) &&
                    !expr->has_flag(HAS_EXEC_PARAM) &&
                    !expr->has_flag(HAS_WINFUNC);
    return ret;
}

u32 ExprUtils::expr_intersect(const Vector<ExprStmt_s> &lhs,
                              const Vector<ExprStmt_s> &rhs,
                              Vector<ExprStmt_s> &res)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> new_exprs;
    for (u32 i = 0; i < lhs.size(); ++i) {
        if (find_equal_expr(rhs, lhs[i])) {
            new_exprs.push_back(lhs[i]);
        }
    }
    res = new_exprs;
    return ret;
}

u32 ExprUtils::remove_equal_exprs(Vector<ExprStmt_s> &src,
                                  const Vector<ExprStmt_s> &dst)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> new_exprs;
    for (u32 i = 0; i < src.size(); ++i) {
        if (!find_equal_expr(dst, src[i])) {
            new_exprs.push_back(src[i]);
        }
    }
    src = new_exprs;
    return ret;
}