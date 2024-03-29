#include "transform_expr_normalize.h"
#include "transform_utils.h"
#include "select_stmt.h"
#include "sql_engine.h"
#include "session_info.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "dml_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformExprNormalize::TransformExprNormalize()
    :TransformRule(PRE_ORDER, EXPR_NORMALIZE)
{

}

TransformExprNormalize::~TransformExprNormalize()
{

}

bool TransformExprNormalize::need_rewrite(DMLStmt_s stmt) const
{
    bool no_rewrit = QUERY_CTX->query_hint->enable_no_rewrite(stmt->get_qb_name());
    if (no_rewrit) {
        LOG_TRACE("no_rewrite hint reject transform");
        return false;
    }
    bool is_disable = false;
    check_hint_disable(stmt, is_disable);
    LOG_TRACE("hint reject transform", K(is_disable));
    return !is_disable;
}

u32 TransformExprNormalize::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    CHECK(transform_exprs(stmt->where_stmt));
    if (stmt->is_select_stmt()) {
        SelectStmt_s sel_stmt = stmt;
        CHECK(transform_exprs(sel_stmt->having_stmt));
    }
    if (transform_happened()) {
        CHECK(generate_outline(stmt));
    }
    return ret;
}

u32 TransformExprNormalize::transform_exprs(Vector<ExprStmt_s> &exprs)
{
    u32 ret = SUCCESS;
    bool is_false = false;
    bool is_true = false;
    Vector<ExprStmt_s> new_exprs;
    cur_transform_exprs = exprs;
    if (1 == exprs.size()) {
        bool is_calculable = false;
        CHECK(ExprUtils::is_calculable_expr(exprs[0], is_calculable));
        if (is_calculable) {
            Object_s obj_result;
            bool bool_result = false;
            CHECK(Sql::SqlEngine::handle_const_expr(exprs[0], obj_result, bool_result));
            if (!bool_result) {
                exprs.clear();
                ExprStmt_s false_expr;
                CHECK(ExprUtils::make_bool_expr(false_expr, false));
                exprs.push_back(false_expr);
            } else {
                exprs.clear();
                set_transform_happened();
            }
            return ret;
        }
    }
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(transform_one_expr(exprs[i],
                                 new_exprs,
                                 is_false,
                                 is_true));
        if (is_false) {
            new_exprs.clear();
            ExprStmt_s false_expr;
            CHECK(ExprUtils::make_bool_expr(false_expr, false));
            new_exprs.push_back(false_expr);
            break;
        }
    }
    if (exprs.size() != new_exprs.size()) {
        set_transform_happened();
    } else {
        for (u32 i = 0; i < new_exprs.size(); ++i) {
            if (new_exprs[i] != exprs[i]) {
                set_transform_happened();
                break;
            }
        }
    }
    exprs = new_exprs;
    return ret;
}

u32 TransformExprNormalize::transform_one_expr(ExprStmt_s &expr, 
                                                Vector<ExprStmt_s> &exprs, 
                                                bool &is_false, 
                                                bool &is_true)
{
    u32 ret = SUCCESS;
    bool is_calculable = false;
    is_false = false;
    is_true = false;
    CHECK(ExprUtils::is_calculable_expr(expr, is_calculable));
    if (is_calculable) {
        Object_s obj_result;
        bool bool_result = false;
        CHECK(Sql::SqlEngine::handle_const_expr(expr, obj_result, bool_result));
        if (bool_result) {
            is_true = true;
        } else {
            is_false = true;
        }
    } else if (expr->has_flag(IS_OP_EXPR)) {
        OpExprStmt_s op_expr = expr;
        if (OP_AND == op_expr->op_type) {
            CHECK(transform_and_expr(op_expr,
                                     exprs,
                                     is_false,
                                     is_true));
        } else if (OP_OR == op_expr->op_type) {
            CHECK(transform_or_expr(op_expr,
                                    exprs,
                                    is_false,
                                    is_true));
        } else {
            exprs.push_back(expr);
        }
    } else {
        exprs.push_back(expr);
    }
    return ret;
}

u32 TransformExprNormalize::transform_and_expr(OpExprStmt_s &expr, 
                                                Vector<ExprStmt_s> &exprs, 
                                                bool &is_false, 
                                                bool &is_true)
{
    u32 ret = SUCCESS;
    is_false = false;
    is_true = false;
    bool all_is_true = true;
    Vector<ExprStmt_s> new_params;
    CHECK(flatten_exprs(expr, OP_AND, new_params));
    expr->params = new_params;
    for (u32 i = 0; i < expr->params.size(); ++i) {
        CHECK(transform_one_expr(expr->params[i],
                                 exprs,
                                 is_false,
                                 is_true));
        if (is_false) {
            all_is_true = false;
            break;
        }
        if (!is_true) {
            all_is_true = false;
        }
    }
    is_true = all_is_true;
    return ret;
}

u32 TransformExprNormalize::transform_or_expr(OpExprStmt_s &expr, 
                                            Vector<ExprStmt_s> &exprs, 
                                            bool &is_false, 
                                            bool &is_true)
{
    u32 ret = SUCCESS;
    Vector<Vector<ExprStmt_s>> all_param_exprs;
    bool all_is_false = true;
    is_false = false;
    is_true = false;
    //flatten expr
    Vector<ExprStmt_s> new_params;
    CHECK(flatten_exprs(expr, OP_OR, new_params));
    expr->params = new_params;
    //transform child exprs
    for (u32 i = 0; i < expr->params.size(); ++i) {
        Vector<ExprStmt_s> param_exprs;
        CHECK(transform_one_expr(expr->params[i],
                                 param_exprs,
                                 is_false,
                                 is_true));
        if (!is_false) {
            all_is_false = false;
        }
        if (is_true) {
            all_is_false = false;
            break;
        }
        if (!is_false) {
            all_param_exprs.push_back(param_exprs);
        }
    }
    is_false = all_is_false;
    if (is_false || is_true) {
        return ret;
    }
    //get common exprs
    Vector<ExprStmt_s> common_exprs = all_param_exprs[0];
    for (u32 i = 1; i < all_param_exprs.size(); ++i) {
        CHECK(ExprUtils::expr_intersect(common_exprs, 
                                        all_param_exprs[i], 
                                        common_exprs));
    }
    if (!common_exprs.empty()) {
        bool has_empty_exprs = false;
        for (u32 i = 0; i < all_param_exprs.size(); ++i) {
            CHECK(ExprUtils::remove_equal_exprs(all_param_exprs[i], 
                                                common_exprs));
            if (all_param_exprs[i].empty()) {
                has_empty_exprs = true;
                break;
            }
        }
        append(exprs, common_exprs);
        if (!has_empty_exprs) {
            Vector<ExprStmt_s> and_exprs;
            ExprStmt_s and_expr;
            ExprStmt_s or_expr;
            for (u32 i = 0; i < all_param_exprs.size(); ++i) {
                if (0 == all_param_exprs[i].size()) {
                    ret = ERR_UNEXPECTED;
                    return ret;
                } else if (1 == all_param_exprs[i].size()) {
                    and_expr = all_param_exprs[i][0];
                } else {
                    CHECK(ExprUtils::make_and_expr(all_param_exprs[i], and_expr));
                }
                and_exprs.push_back(and_expr);
            }
            CHECK(ExprUtils::make_or_expr(and_exprs, or_expr));
            exprs.push_back(or_expr);
        }
    } else {
        exprs.push_back(expr);
    }
    //split or exprs
    if (expr->table_ids.num_members() > 1) {
        Vector<ExprStmt_s> or_exprs;
        CHECK(split_or_expr(all_param_exprs, or_exprs));
        append(exprs, or_exprs);
    }
    return ret;
}

u32 TransformExprNormalize::split_or_expr(Vector<Vector<ExprStmt_s>> &all_exprs,
                                          Vector<ExprStmt_s> &or_exprs)
{
    u32 ret = SUCCESS;
    if (all_exprs.empty() || all_exprs[0].empty()) {
        return ret;
    }
    Vector<BitSet> visited_table_ids;
    for (u32 i = 0; i < all_exprs[0].size(); ++i) {
        Vector<ExprStmt_s> param_exprs;
        ExprStmt_s &expr = all_exprs[0][i];
        if (expr->table_ids.num_members() != 1) {
            continue;
        }
        bool visited = false;
        for (u32 i = 0; !visited && i < visited_table_ids.size(); ++i) {
            if (expr->table_ids.is_equal(visited_table_ids[i])) {
                visited = true;
            }
        }
        if (visited) {
            continue;
        }
        for (u32 j = 0; j < all_exprs.size(); ++j) {
            Vector<ExprStmt_s> and_exprs;
            for (u32 k = 0; k < all_exprs[j].size(); ++k) {
                if (all_exprs[j][k]->table_ids.is_equal(expr->table_ids)) {
                    and_exprs.push_back(all_exprs[j][k]);
                }
            }
            if (and_exprs.empty()) {
                break;
            } else if (1 == and_exprs.size()) {
                param_exprs.push_back(and_exprs[0]);
            } else {
                ExprStmt_s and_expr;
                CHECK(ExprUtils::make_and_expr(and_exprs, and_expr));
                param_exprs.push_back(and_expr);
            }
        }
        if (param_exprs.size() == all_exprs.size()) {
            ExprStmt_s or_expr;
            CHECK(ExprUtils::make_or_expr(param_exprs, or_expr));
            if (!ExprUtils::find_equal_expr(cur_transform_exprs, or_expr)) {
                or_exprs.push_back(or_expr);
                visited_table_ids.push_back(expr->table_ids);
            }
        }
    }
    return ret;
}

u32 TransformExprNormalize::flatten_exprs(ExprStmt_s expr, 
                                          OperationType type, 
                                          Vector<ExprStmt_s> &params)
{
    u32 ret = SUCCESS;
    if (OP_EXPR == expr->expr_type()) {
        OpExprStmt_s op_expr = expr;
        if (type == op_expr->op_type) {
            for (u32 i = 0; i < expr->params.size(); ++i) {
                CHECK(flatten_exprs(expr->params[i], type, params));
            }
        } else {
            params.push_back(expr);
        }
    } else {
        params.push_back(expr);
    }
    return ret;
}

u32 TransformExprNormalize::check_hint_disable(DMLStmt_s& stmt, bool &is_disable) const
{
    u32 ret = SUCCESS;
    is_disable = false;
    QueryHint_s &query_hint = QUERY_CTX->query_hint;
    is_disable = query_hint->enable_no_expr_normalize(stmt->get_qb_name());
    return ret;
}

u32 TransformExprNormalize::generate_outline(DMLStmt_s& stmt)
{
    u32 ret = SUCCESS;
    QueryHint_s &query_hint = QUERY_CTX->query_hint;
    CHECK(query_hint->generate_transform_outline(stmt->get_qb_name(), control_hint));
    return ret;
}