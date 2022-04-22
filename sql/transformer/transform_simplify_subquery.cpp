#include "transform_simplify_subquery.h"
#include "transform_utils.h"
#include "select_stmt.h"
#include "query_ctx.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "dml_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformSimplifySubquery::TransformSimplifySubquery()
    :TransformRule(PRE_ORDER, SIMPLIFY_SQ)
{

}

TransformSimplifySubquery::~TransformSimplifySubquery()
{

}

u32 TransformSimplifySubquery::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    ExprStmt_s old_expr;
    Vector<ExprStmt_s> all_exprs;
    Vector<ExprStmt_s> old_exprs;
    Vector<ExprStmt_s> new_exprs;
    CHECK(stmt->get_stmt_exprs(all_exprs));
    for (u32 i = 0; i < all_exprs.size(); ++i) {
        old_expr = all_exprs[i];
        CHECK(transform_one_expr(all_exprs[i]));
        if (old_expr != all_exprs[i]) {
            old_exprs.push_back(old_expr);
            new_exprs.push_back(all_exprs[i]);
        }
    }
    if (!old_exprs.empty()) {
        CHECK(stmt->replace_stmt_exprs(old_exprs, new_exprs));
    }
    return ret;
}

u32 TransformSimplifySubquery::transform_one_expr(ExprStmt_s &expr)
{
    u32 ret = SUCCESS;
    CHECK(inner_transform_one_expr(expr));
    for (u32 i = 0; i < expr->params.size(); ++i) {
        CHECK(transform_one_expr(expr->params[i]));
    }
    return ret;
}

u32 TransformSimplifySubquery::inner_transform_one_expr(ExprStmt_s &expr)
{
    u32 ret = SUCCESS;
    bool is_disable = false;
    bool happened = false;
    bool is_empty = false;
    bool output_one_row = false;
    SelectStmt_s subquery;
    if (expr->has_flag(IS_OP_EXPR)) {
        OpExprStmt_s op_expr = expr;
        if (OP_EQ_ANY <= op_expr->op_type && OP_LT_ANY >= op_expr->op_type) {
            MY_ASSERT(2 == op_expr->params.size());
            MY_ASSERT(op_expr->params[1]->has_flag(IS_SUBQUERY));
            SubQueryStmt_s subquery_expr = op_expr->params[1];
            subquery = subquery_expr->query_stmt;
            CHECK(check_subquery_status(subquery,
                                        is_empty, 
                                        output_one_row));
            CHECK(check_hint_disable(subquery, is_disable));
            if (is_disable) {
                return ret;
            }
            if (output_one_row) {
                op_expr->op_type = ExprUtils::remove_op_any(op_expr->op_type);
                happened = true;
            } else if (is_empty) {
                CHECK(ExprUtils::make_bool_expr(expr, false));
                happened = true;
            } else {
                CHECK(simplify_subquery(subquery,
                                        happened,
                                        op_expr->op_type));
            }
        } else if (OP_EQ_ALL <= op_expr->op_type && OP_LT_ALL >= op_expr->op_type) {
            MY_ASSERT(2 == op_expr->params.size());
            MY_ASSERT(op_expr->params[1]->has_flag(IS_SUBQUERY));
            SubQueryStmt_s subquery_expr = op_expr->params[1];
            subquery = subquery_expr->query_stmt;
            CHECK(check_subquery_status(subquery,
                                        is_empty, 
                                        output_one_row));
            CHECK(check_hint_disable(subquery, is_disable));
            if (is_disable) {
                return ret;
            }
            if (output_one_row) {
                op_expr->op_type = ExprUtils::remove_op_all(op_expr->op_type);
                happened = true;
            } else if (is_empty) {
                CHECK(ExprUtils::make_bool_expr(expr, true));
                happened = true;
            } else {
                CHECK(simplify_subquery(subquery,
                                        happened,
                                        op_expr->op_type));
            }
        } else if (OP_IN == op_expr->op_type) {
            MY_ASSERT(2 == op_expr->params.size());
            MY_ASSERT(op_expr->params[1]->has_flag(IS_SUBQUERY));
            SubQueryStmt_s subquery_expr = op_expr->params[1];
            subquery = subquery_expr->query_stmt;
            CHECK(check_subquery_status(subquery,
                                        is_empty, 
                                        output_one_row));
            CHECK(check_hint_disable(subquery, is_disable));
            if (is_disable) {
                return ret;
            }
            if (output_one_row) {
                op_expr->op_type = OP_EQ;
                happened = true;
            } else if (is_empty) {
                CHECK(ExprUtils::make_bool_expr(expr, false));
                happened = true;
            } else {
                CHECK(simplify_subquery(subquery,
                                        happened,
                                        op_expr->op_type));
            }
        } else if (OP_NOT_IN == op_expr->op_type) {
            MY_ASSERT(2 == op_expr->params.size());
            MY_ASSERT(op_expr->params[1]->has_flag(IS_SUBQUERY));
            SubQueryStmt_s subquery_expr = op_expr->params[1];
            subquery = subquery_expr->query_stmt;
            CHECK(check_subquery_status(subquery,
                                        is_empty, 
                                        output_one_row));
            CHECK(check_hint_disable(subquery, is_disable));
            if (is_disable) {
                return ret;
            }
            if (is_empty) {
                CHECK(ExprUtils::make_bool_expr(expr, true));
                happened = true;
            } else {
                CHECK(simplify_subquery(subquery,
                                        happened,
                                        op_expr->op_type));
            }
        } else if (OP_EXISTS == op_expr->op_type) {
            MY_ASSERT(1 == op_expr->params.size());
            MY_ASSERT(op_expr->params[0]->has_flag(IS_SUBQUERY));
            SubQueryStmt_s subquery_expr = op_expr->params[0];
            subquery = subquery_expr->query_stmt;
            CHECK(check_subquery_status(subquery,
                                        is_empty, 
                                        output_one_row));
            CHECK(check_hint_disable(subquery, is_disable));
            if (is_disable) {
                return ret;
            }
            if (output_one_row) {
                CHECK(ExprUtils::make_bool_expr(expr, true));
                happened = true;
            } else if (is_empty) {
                CHECK(ExprUtils::make_bool_expr(expr, false));
                happened = true;
            } else {
                CHECK(simplify_subquery(subquery,
                                        happened,
                                        op_expr->op_type));
            }
        } else if (OP_NOT_EXISTS == op_expr->op_type) {
            MY_ASSERT(1 == op_expr->params.size());
            MY_ASSERT(op_expr->params[0]->has_flag(IS_SUBQUERY));
            SubQueryStmt_s subquery_expr = op_expr->params[0];
            subquery = subquery_expr->query_stmt;
            CHECK(check_subquery_status(subquery,
                                        is_empty, 
                                        output_one_row));
            CHECK(check_hint_disable(subquery, is_disable));
            if (is_disable) {
                return ret;
            }
            if (output_one_row) {
                CHECK(ExprUtils::make_bool_expr(expr, false));
                happened = true;
            } else if (is_empty) {
                CHECK(ExprUtils::make_bool_expr(expr, true));
                happened = true;
            } else {
                CHECK(simplify_subquery(subquery,
                                        happened,
                                        op_expr->op_type));
            }
        }
    } else if (expr->has_flag(IS_SUBQUERY)) {
        SubQueryStmt_s subquery_expr = expr;
        subquery = subquery_expr->query_stmt;
        CHECK(check_subquery_status(subquery,
                                    is_empty, 
                                    output_one_row));
        CHECK(check_hint_disable(subquery, is_disable));
        if (is_disable) {
            return ret;
        }
        if (is_empty) {
            CHECK(ExprUtils::make_null_expr(expr));
            happened = true;
        } else {
            CHECK(simplify_subquery(subquery,
                                    happened));
        }
    }
    if (happened) {
        set_transform_happened();
        CHECK(generate_outline(subquery));
        LOG_TRACE("subquery expr has been simplified!", K(expr));
    }
    return ret;
}

u32 TransformSimplifySubquery::check_subquery_status(SelectStmt_s &subquery,
                                                    bool &is_empty, 
                                                    bool &output_one_row)
{
    u32 ret = SUCCESS;
    is_empty = false;
    output_one_row = false;
    if (subquery->is_scalar_group_by()) {
        output_one_row = true;
    }
    if (subquery->limit_stmt) {
        if (0 == subquery->limit_stmt->limit_size) {
            is_empty = true;
            output_one_row = false;
        } else if (output_one_row && 0 < subquery->limit_stmt->limit_offset) {
            is_empty = true;
            output_one_row = false;
        }
    }
    if (subquery->is_set_stmt() && !is_empty) {
        bool l_is_empty = false;
        bool l_output_one_row = false;
        bool r_is_empty = false;
        bool r_output_one_row = false;
        SetStmt_s set_stmt = subquery;
        CHECK(check_subquery_status(set_stmt->left_query,
                                    l_is_empty,
                                    l_output_one_row));
        CHECK(check_subquery_status(set_stmt->right_query,
                                    r_is_empty,
                                    r_output_one_row));
        if (INTERSECT == set_stmt->set_op) {
            if (l_is_empty || r_is_empty) {
                is_empty = true;
            }
        } else if (EXCEPT == set_stmt->set_op) {
            if (l_is_empty) {
                is_empty = true;
            }
            if (l_output_one_row && r_is_empty) {
                output_one_row = true;
            }
        } else {
            if (l_is_empty && r_is_empty) {
                is_empty = true;
            }
        }
    }
    return ret;
}

u32 TransformSimplifySubquery::simplify_subquery(SelectStmt_s &subquery, 
                                                 bool &happened,
                                                 OperationType type)
{
    u32 ret = SUCCESS;
    happened = false;
    bool has_offset = false;
    if (subquery->limit_stmt) {
        if (0 < subquery->limit_stmt->limit_offset) {
            has_offset = true;
        }
    }
    if (subquery->is_distinct && !has_offset) {
        subquery->is_distinct = false;
        happened = true;
    }
    if (subquery->is_set_stmt() && !has_offset) {
        SetStmt_s set_stmt = subquery;
        if (UNION == set_stmt->set_op) {
            set_stmt->set_op = UNION_ALL;
            happened = true;
        }
    }
    if (subquery->limit_stmt) {
        if (has_offset) {
            return ret;
        } else if (OP_EQ_ANY <= type && OP_LT_ALL >= type) {
            return ret;
        } else if (OP_IN == type || OP_NOT_IN == type) {
            return ret;
        } else if (OP_INVALID == type && !subquery->is_scalar_group_by()) {
            return ret;
        } else {
            subquery->limit_stmt.reset();
            happened = true;
        }
    }
    if (!subquery->order_exprs.empty()) {
        subquery->order_exprs.clear();
        happened = true;
    }
    if (OP_EXISTS == type || OP_NOT_EXISTS == type) {
        if (subquery->having_stmt.empty() && !subquery->group_exprs.empty()) {
            subquery->group_exprs.clear();
            happened = true;
        }
        if (subquery->is_set_stmt()) {
            SetStmt_s set_stmt = subquery;
            if (INTERSECT == set_stmt->set_op || EXCEPT == set_stmt->set_op) {
                return ret;
            }
        }
        bool is_dummy = false;
        CHECK(TransformUtils::is_dummy_select_expr(subquery, is_dummy));
        if (!is_dummy) {
            CHECK(TransformUtils::create_dummy_select_expr(subquery));
            happened = true;
        }
    }
    return ret;
}

u32 TransformSimplifySubquery::check_hint_disable(SelectStmt_s& subquery, bool &is_disable)
{
    u32 ret = SUCCESS;
    is_disable = false;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    is_disable = query_hint.enable_no_simplify_sq(subquery->get_qb_name());
    return ret;
}

u32 TransformSimplifySubquery::generate_outline(SelectStmt_s& subquery)
{
    u32 ret = SUCCESS;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    CHECK(query_hint.generate_transform_outline(subquery->get_qb_name(), control_hint));
    return ret;
}