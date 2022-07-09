#include "transform_unnest_J_N_subquery.h"
#include "transform_utils.h"
#include "select_stmt.h"
#include "session_info.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformUnnestJNSubquery::TransformUnnestJNSubquery()
    :TransformRule(PRE_ORDER, UNNEST)
{

}

TransformUnnestJNSubquery::~TransformUnnestJNSubquery()
{

}

u32 TransformUnnestJNSubquery::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    TransfomHelper helper;
    //check on condition
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        CHECK(transform_on_condition(stmt,
                                     stmt->from_stmts[i], 
                                     helper));
    }
    //check where condition
    helper.is_on_condition = false;
    CHECK(transform_conditions(stmt,
                               stmt->where_stmt, 
                               helper));
    if (stmt->is_select_stmt()) {
        SelectStmt_s sel_stmt = stmt;
        //check having condition
        CHECK(transform_conditions(stmt,
                                   sel_stmt->having_stmt, 
                                   helper));
    }
    return ret;
}

u32 TransformUnnestJNSubquery::transform_on_condition(DMLStmt_s &stmt,
                                                      TableStmt_s &table, 
                                                      TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    bool happened = false;
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        CHECK(transform_on_condition(stmt,
                                    joined_table->left_table, 
                                    helper));
        CHECK(transform_on_condition(stmt,
                                    joined_table->right_table, 
                                    helper));
        bool is_valid = true;
        if (Inner == joined_table->join_type) {
            helper.table_item = joined_table->left_table;
        } else if (FullOuter == joined_table->join_type) {
            is_valid = false;
        } else if (LeftOuter == joined_table->join_type ||
                   LeftSemi == joined_table->join_type) {
            helper.table_item = joined_table->left_table;
        } else if (RightOuter == joined_table->join_type ||
                   RightSemi == joined_table->join_type) {
            helper.table_item = joined_table->right_table;
        }
        if (is_valid) {
            Vector<ExprStmt_s> new_exprs;
            for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
                ExprStmt_s &expr = joined_table->join_condition[i];
                if (!expr->table_ids.is_subset(helper.table_item->table_ids)) {
                    new_exprs.push_back(expr);
                    continue;
                } else {
                    CHECK(transform_one_expr(stmt,
                                             expr, 
                                             helper, 
                                             happened));
                    if (happened) {
                        LOG_TRACE("succeed to transfom one condition", K(helper));
                    } else {
                        new_exprs.push_back(expr);
                    }
                }
            }
            joined_table->join_condition = new_exprs;
        }
        if (Inner == joined_table->join_type) {
            joined_table->left_table = helper.table_item;
        } else if (LeftOuter == joined_table->join_type ||
                   LeftSemi == joined_table->join_type) {
            joined_table->left_table = helper.table_item;
        } else if (RightOuter == joined_table->join_type ||
                   RightSemi == joined_table->join_type) {
            joined_table->right_table = helper.table_item;
        }
    }
    Vector<ExprStmt_s> filters = table->table_filter;
    helper.table_item = table;
    table->table_filter.clear();
    CHECK(transform_conditions(stmt, 
                               filters, 
                               helper));
    table = helper.table_item;
    table->table_filter = filters;
    return ret;
}

u32 TransformUnnestJNSubquery::transform_conditions(DMLStmt_s &stmt,
                                                    Vector<ExprStmt_s> &exprs, 
                                                    TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    bool  happened = false;
    Vector<ExprStmt_s> new_exprs;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(transform_one_expr(stmt,
                                 exprs[i], 
                                 helper, 
                                 happened));
        if (happened) {
            LOG_TRACE("succeed to transfom one condition", K(helper));
        } else {
            new_exprs.push_back(exprs[i]);
        }
    }
    exprs = new_exprs;
    return ret;
}

u32 TransformUnnestJNSubquery::transform_one_expr(DMLStmt_s &stmt,
                                                ExprStmt_s &expr, 
                                                TransfomHelper &helper, 
                                                bool &happened)
{
    u32 ret = SUCCESS;
    bool is_valid = false;
    happened = false;
    helper.reset();
    CHECK(check_expr_need_transform(expr, helper, is_valid));
    if (is_valid) {
        bool is_disable = false;
        CHECK(check_hint_disable(helper.subquery_expr->query_stmt, is_disable));
        if (!is_disable) {
            CHECK(do_transform(stmt, helper));
            CHECK(generate_outline(helper.subquery_expr->query_stmt));
            set_transform_happened();
            happened = true;
        }
    }
    return ret;
}

u32 TransformUnnestJNSubquery::check_expr_need_transform(ExprStmt_s &expr, 
                                                        TransfomHelper &helper, 
                                                        bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (!expr->has_flag(HAS_SUBQUERY) || 
        !expr->has_flag(IS_OP_EXPR)) {
        return ret;
    }
    helper.expr = expr;
    CHECK(check_subquery_type(helper, is_valid));
    if (!is_valid) {
        return ret;
    }
    if (!helper.subquery_expr->has_related_expr()) {
        return ret;
    }
    if (helper.subquery_expr->query_stmt->has_limit()) {
        is_valid = false;
        return ret;
    }
    CHECK(check_correlated_exprs(helper, is_valid));
    if (!is_valid) {
        return ret;
    }
    return ret;
}

/**
 * ANY/ALL --> N、J
 * IN/NOT IN --> N、J
 * EXISTS/NOT EXISTS --> J
 */
u32 TransformUnnestJNSubquery::check_subquery_type(TransfomHelper &helper, 
                                                   bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    SelectStmt_s subquery;
    OperationType op_type = helper.expr->op_type;
    if ((OP_EQ_ANY <= op_type && op_type <= OP_LT_ALL) ||
        OP_IN == op_type || OP_NOT_IN == op_type) {
        MY_ASSERT(2 == helper.expr->params.size());
        ExprStmt_s &r_expr = helper.expr->params[1];
        MY_ASSERT(r_expr->has_flag(IS_SUBQUERY));
        helper.subquery_expr = r_expr;
        subquery = helper.subquery_expr->query_stmt;
    } else if (OP_EXISTS == op_type || OP_NOT_EXISTS == op_type) {
        MY_ASSERT(1 == helper.expr->params.size());
        ExprStmt_s &r_expr = helper.expr->params[0];
        MY_ASSERT(r_expr->has_flag(IS_SUBQUERY));
        helper.subquery_expr = r_expr;
        subquery = helper.subquery_expr->query_stmt;
    }
    if (subquery) {
        if (subquery->is_scalar_group_by()) {
            //type A subquery
            is_valid = false;
        } else if (helper.expr->op_type >= OP_EQ_ANY && 
                   helper.expr->op_type <= OP_LT_ALL) {
            //ANY/ALL --> N、J
            is_valid = 1 == subquery->select_expr_list.size();
        } else if (helper.expr->op_type == OP_IN ||
                   helper.expr->op_type == OP_NOT_IN) {
            //IN/NOT IN --> N、J
            is_valid = 1 == subquery->select_expr_list.size();
        } else if (helper.expr->op_type == OP_EXISTS ||
                   helper.expr->op_type == OP_NOT_EXISTS) {
            //EXISTS/NOT EXISTS --> J
            is_valid = true;
        }
    }
    return ret;
}

u32 TransformUnnestJNSubquery::check_correlated_exprs(TransfomHelper &helper, 
                                                    bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (helper.subquery_expr->exec_params.empty()) {
        //none correlated subquery
        is_valid = true;
        return ret;
    }
    bool has_correlated_expr = false;
    bool is_correlated = false;
    SelectStmt_s &subquery = helper.subquery_expr->query_stmt;
    //check aggr item
    CHECK(TransformUtils::has_correlated_expr(subquery->get_aggr_exprs(),
                                              helper.subquery_expr,
                                              is_correlated));
    if (is_correlated) {
        is_valid = false;
        return ret;
    }
    //check child stmt
    Vector<SelectStmt_s> child_stmts;
    CHECK(subquery->get_child_stmts(child_stmts));
    for (u32 i = 0; !is_correlated && i < child_stmts.size(); ++i) {
        CHECK(TransformUtils::check_stmt_correlated(child_stmts[i], 
                                                    helper.subquery_expr, 
                                                    is_correlated));
    }
    if (is_correlated) {
        is_valid = false;
        return ret;
    }
    //check join condition
    for (u32 i = 0; i < subquery->from_stmts.size(); ++i) {
        CHECK(check_join_condition(helper,
                                   subquery->from_stmts[i],
                                   has_correlated_expr,
                                   is_valid));
        if (!is_valid) {
            return ret;
        }
    }
    //check where condition
    for (u32 i = 0; i < subquery->where_stmt.size(); ++i) {
        CHECK(TransformUtils::has_correlated_expr(subquery->where_stmt[i],
                                                  helper.subquery_expr,
                                                  has_correlated_expr));
        if (has_correlated_expr) {
            helper.correlated_exprs.push_back(subquery->where_stmt[i]);
        }
    }
    //check group by condition
    CHECK(TransformUtils::has_correlated_expr(subquery->group_exprs,
                                              helper.subquery_expr,
                                              has_correlated_expr));
    if (has_correlated_expr) {
        is_valid = false;
        return ret;
    }
    //check having condition
    for (u32 i = 0; i < subquery->having_stmt.size(); ++i) {
        CHECK(TransformUtils::has_correlated_expr(subquery->having_stmt[i],
                                                  helper.subquery_expr,
                                                  has_correlated_expr));
        if (has_correlated_expr) {
            helper.correlated_exprs.push_back(subquery->having_stmt[i]);
        }
    }
    if (has_correlated_expr && subquery->has_group_by()) {
        CHECK(check_expr_can_pullup(helper,
                                    helper.correlated_exprs, 
                                    subquery->group_exprs,
                                    is_valid));
        if (is_valid) {
            CHECK(get_pushdown_exprs(helper, helper.correlated_exprs));
        }
    }
    return ret;
}

u32 TransformUnnestJNSubquery::check_join_condition(TransfomHelper &helper, 
                                                    TableStmt_s &table,
                                                    bool &has_correlated_expr,
                                                    bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = true;
    has_correlated_expr = false;
    bool has_exec_param = false;
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        bool l_has_correlated_expr = false;
        bool r_has_correlated_expr = false;
        CHECK(check_join_condition(helper, 
                                   joined_table->left_table,
                                   l_has_correlated_expr,
                                   is_valid));
        if (!is_valid) {
            return ret;
        }
        if (l_has_correlated_expr) {
            if (RightOuter == joined_table->join_type ||
                FullOuter == joined_table->join_type ||
                RightSemi == joined_table->join_type ||
                RightAnti == joined_table->join_type) {
                is_valid = false;
                return ret;
            }
        }
        CHECK(check_join_condition(helper, 
                                   joined_table->right_table,
                                   r_has_correlated_expr,
                                   is_valid));
        if (!is_valid) {
            return ret;
        }
        if (r_has_correlated_expr) {
            if (LeftOuter == joined_table->join_type ||
                FullOuter == joined_table->join_type ||
                LeftSemi == joined_table->join_type ||
                LeftAnti == joined_table->join_type) {
                is_valid = false;
                return ret;
            }
        }
        
        for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
            CHECK(TransformUtils::has_correlated_expr(joined_table->join_condition[i],
                                                      helper.subquery_expr,
                                                      has_exec_param));
            if (has_exec_param) {
                if (Inner != joined_table->join_type) {
                    is_valid = false;
                    return ret;
                }
                helper.correlated_exprs.push_back(joined_table->join_condition[i]);
                has_correlated_expr = true;
            }
        }
    }
    for (u32 i = 0; i < table->table_filter.size(); ++i) {
        CHECK(TransformUtils::has_correlated_expr(table->table_filter[i],
                                                  helper.subquery_expr,
                                                  has_exec_param));
        if (has_exec_param) {
            helper.correlated_exprs.push_back(table->table_filter[i]);
            has_correlated_expr = true;
        }
    }
    return ret;
}

u32 TransformUnnestJNSubquery::check_expr_can_pullup(TransfomHelper &helper,
                                                     Vector<ExprStmt_s> &correlated_exprs,
                                                     Vector<ExprStmt_s> &group_exprs,
                                                     bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = true;
    for (u32 i = 0; is_valid && i < correlated_exprs.size(); ++i) {
        CHECK(check_expr_can_pullup(helper,
                                    correlated_exprs[i], 
                                    group_exprs,
                                    is_valid));
    }
    return ret;
}

u32 TransformUnnestJNSubquery::check_expr_can_pullup(TransfomHelper &helper,
                                                ExprStmt_s &correlated_expr,
                                                Vector<ExprStmt_s> &group_exprs,
                                                bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (ExprUtils::find_equal_expr(group_exprs, correlated_expr)) {
        is_valid = true;
    } else if (correlated_expr->params.size() > 0) {
        CHECK(check_expr_can_pullup(helper, 
                                    correlated_expr->params, 
                                    group_exprs, 
                                    is_valid));
    } else if (correlated_expr->has_flag(IS_CONST)) {
        is_valid = true;
    } else if (correlated_expr->has_flag(IS_SUBQUERY)) {
        is_valid = true;
    } else if (correlated_expr->has_flag(IS_EXEC_PARAM)) {
        is_valid = true;
    }
    return ret;
}

u32 TransformUnnestJNSubquery::get_pushdown_exprs(TransfomHelper &helper,
                                                Vector<ExprStmt_s> &correlated_exprs)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < correlated_exprs.size(); ++i) {
        CHECK(get_pushdown_exprs(helper,
                                 correlated_exprs[i]));
    }
    return ret;
}

u32 TransformUnnestJNSubquery::get_pushdown_exprs(TransfomHelper &helper,
                                                ExprStmt_s &correlated_expr)
{
    u32 ret = SUCCESS;
    bool is_valid = false;
    CHECK(TransformUtils::has_correlated_expr(correlated_expr,
                                            helper.subquery_expr,
                                            is_valid));
    if (!is_valid) {
        helper.pushdown_exprs.push_back(correlated_expr);
    } else {
        CHECK(get_pushdown_exprs(helper, correlated_expr->params));
    }
    return ret;
}

u32 TransformUnnestJNSubquery::do_transform(DMLStmt_s &stmt, 
                                          TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    JoinType join_type = LeftSemi;
    SelectStmt_s &subquery = helper.subquery_expr->query_stmt;
    MY_ASSERT(1 <= subquery->select_expr_list.size());
    if (!helper.is_on_condition) {
        CHECK(TransformUtils::build_joined_table(stmt->from_stmts, 
                                                    helper.table_item));
    }
    //adjust subquery
    ExprStmt_s value_expr = subquery->select_expr_list[0];
    subquery->select_expr_list.clear();
    Vector<ExprStmt_s> columns_exprs;
    CHECK(get_pushdown_exprs(helper, value_expr));
    //create view table
    ViewTableStmt_s view_table;
    CHECK(TransformUtils::create_table_with_view(subquery, 
                                                 view_table));
    CHECK(TransformUtils::create_select_item_for_view_table(view_table, 
                                                            helper.pushdown_exprs, 
                                                            columns_exprs));
    //remove correlated exprs
    SubQueryStmt_s &subquery_expr = helper.subquery_expr;
    CHECK(remove_correlated_exprs(subquery_expr));
    //create join condition
    Vector<ExprStmt_s> exec_params;
    append(exec_params, subquery_expr->exec_params);
    CHECK(build_join_condition(helper, value_expr, join_type));
    CHECK(ExprUtils::replace_exprs(exec_params, 
                                   subquery_expr->params, 
                                   helper.correlated_exprs));
    CHECK(ExprUtils::replace_exprs(helper.pushdown_exprs, 
                                   columns_exprs, 
                                   helper.correlated_exprs));
    //create left join
    TableStmt_s right_table = view_table;
    JoinedTableStmt_s joined_table = JoinedTableStmt::make_joined_table(helper.table_item,
                                                                        right_table,
                                                                        join_type);
    joined_table->join_condition = helper.correlated_exprs;
    helper.table_item = joined_table;
    CHECK(joined_table->formalize());
    if (!helper.is_on_condition) {
        stmt->from_stmts.clear();
        stmt->from_stmts.push_back(helper.table_item);
    }
    return ret;
}

u32 TransformUnnestJNSubquery::remove_correlated_exprs(SubQueryStmt_s &subquery_expr)
{
    u32 ret = SUCCESS;
    bool has_correlated_expr = false;
    SelectStmt_s &subquery = subquery_expr->query_stmt;
    //check join condition
    for (u32 i = 0; i < subquery->from_stmts.size(); ++i) {
        CHECK(remove_correlated_exprs(subquery->from_stmts[i],
                                      subquery_expr));
    }
    //check where condition
    CHECK(remove_correlated_exprs(subquery->where_stmt, subquery_expr));
    //check having condition
    CHECK(remove_correlated_exprs(subquery->having_stmt, subquery_expr));
    return ret;
}

u32 TransformUnnestJNSubquery::remove_correlated_exprs(TableStmt_s &table,
                                                       SubQueryStmt_s &subquery_expr)
{
    u32 ret = SUCCESS;
    bool has_correlated_expr = false;
    Vector<ExprStmt_s> new_exprs;
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        CHECK(remove_correlated_exprs(joined_table->join_condition, 
                                      subquery_expr));
        CHECK(remove_correlated_exprs(joined_table->left_table,
                                      subquery_expr));
        CHECK(remove_correlated_exprs(joined_table->right_table,
                                      subquery_expr));
    }
    CHECK(remove_correlated_exprs(table->table_filter, 
                                  subquery_expr));
    return ret;
}

u32 TransformUnnestJNSubquery::remove_correlated_exprs(Vector<ExprStmt_s> &exprs,
                                                       SubQueryStmt_s &subquery_expr)
{
    u32 ret = SUCCESS;
    bool has_correlated_expr = false;
    Vector<ExprStmt_s> new_exprs = exprs;
    exprs.clear();
    for (u32 i = 0; i < new_exprs.size(); ++i) {
        CHECK(TransformUtils::has_correlated_expr(new_exprs[i],
                                                  subquery_expr,
                                                  has_correlated_expr));
        if (!has_correlated_expr) {
            exprs.push_back(new_exprs[i]);
        }
    }
    return ret;
}

u32 TransformUnnestJNSubquery::build_join_condition(TransfomHelper &helper, 
                                                    ExprStmt_s &value_expr,
                                                    JoinType &join_type)
{
    u32 ret = SUCCESS;
    if (OP_EQ_ANY == helper.expr->op_type) {
        helper.expr->op_type = OP_EQ;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_NE_ANY == helper.expr->op_type) {
        helper.expr->op_type = OP_NE;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_GE_ANY == helper.expr->op_type) {
        helper.expr->op_type = OP_GE;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_GT_ANY == helper.expr->op_type) {
        helper.expr->op_type = OP_GT;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_LE_ANY == helper.expr->op_type) {
        helper.expr->op_type = OP_LE;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_LT_ANY == helper.expr->op_type) {
        helper.expr->op_type = OP_LT;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_EQ_ALL == helper.expr->op_type) {
        helper.expr->op_type = OP_NE;
        helper.expr->params[1] = value_expr;
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_NE_ALL == helper.expr->op_type) {
        helper.expr->op_type = OP_EQ;
        helper.expr->params[1] = value_expr;
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_GE_ALL == helper.expr->op_type) {
        helper.expr->op_type = OP_LT;
        helper.expr->params[1] = value_expr;
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_GT_ALL == helper.expr->op_type) {
        helper.expr->op_type = OP_LE;
        helper.expr->params[1] = value_expr;
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_LE_ALL == helper.expr->op_type) {
        helper.expr->op_type = OP_LT;
        helper.expr->params[1] = value_expr;
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_LT_ALL == helper.expr->op_type) {
        helper.expr->op_type = OP_GE;
        helper.expr->params[1] = value_expr;
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_EXISTS == helper.expr->op_type) {
        join_type = LeftSemi;
    } else if (OP_NOT_EXISTS == helper.expr->op_type) {
        join_type = LeftAnti;
    } else if (OP_IN == helper.expr->op_type) {
        helper.expr->op_type = OP_EQ;
        helper.expr->params[1] = value_expr;
        join_type = LeftSemi;
        helper.correlated_exprs.push_back(helper.expr);
    } else if (OP_NOT_IN == helper.expr->op_type) {
        Vector<ExprStmt_s> or_params;
        helper.expr->op_type = OP_NE;
        helper.expr->params[1] = value_expr;
        or_params.push_back(helper.expr);
        ExprStmt_s l_is_null;
        ExprStmt_s r_is_null;
        CHECK(ExprUtils::make_is_null_expr(helper.expr->params[0], l_is_null));
        CHECK(ExprUtils::make_is_null_expr(value_expr, r_is_null));
        or_params.push_back(l_is_null);
        or_params.push_back(r_is_null);
        ExprStmt_s or_expr;
        CHECK(ExprUtils::make_or_expr(or_params, or_expr));
        join_type = LeftAnti;
        helper.correlated_exprs.push_back(or_expr);
    }
    return ret;
}

u32 TransformUnnestJNSubquery::check_hint_disable(SelectStmt_s& subquery, bool &is_disable)
{
    u32 ret = SUCCESS;
    is_disable = false;
    QueryHint &query_hint = QUERY_CTX->query_hint;
    is_disable = query_hint.enable_no_unnest(subquery->get_qb_name());
    return ret;
}

u32 TransformUnnestJNSubquery::generate_outline(SelectStmt_s& subquery)
{
    u32 ret = SUCCESS;
    QueryHint &query_hint = QUERY_CTX->query_hint;
    CHECK(query_hint.generate_transform_outline(subquery->get_qb_name(), control_hint));
    return ret;
}