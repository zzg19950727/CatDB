#include "transform_unnest_JA_subquery.h"
#include "transform_utils.h"
#include "select_stmt.h"
#include "update_stmt.h"
#include "query_ctx.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformUnnestJASubquery::TransformUnnestJASubquery()
    :TransformRule(PRE_ORDER, UNNEST)
{

}

TransformUnnestJASubquery::~TransformUnnestJASubquery()
{

}

u32 TransformUnnestJASubquery::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    bool happened = false;
    TransfomHelper helper;
    //check on condition
    helper.is_on_condition = true;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        CHECK(transform_on_condition(stmt,
                                     stmt->from_stmts[i], 
                                     helper));
    }
    //check where condition
    helper.is_on_condition = false;
    CHECK(transform_exprs(stmt,
                        stmt->where_stmt, 
                        helper));
    if (stmt->is_select_stmt()) {
        SelectStmt_s sel_stmt = stmt;
        //check group by
        CHECK(transform_exprs(stmt,
                              sel_stmt->group_exprs, 
                              helper));
        //check having condition
        CHECK(transform_exprs(stmt,
                            sel_stmt->having_stmt, 
                            helper));
        //check select expr
        CHECK(transform_exprs(stmt,
                              sel_stmt->select_expr_list, 
                              helper));
        //check order by 
        CHECK(transform_exprs(stmt,
                              sel_stmt->order_exprs, 
                              helper));
    }
    if (stmt->is_update_stmt()) {
        UpdateStmt_s upd_stmt = stmt;
        //check update set expr
        CHECK(transform_exprs(stmt,
                              upd_stmt->update_assign_stmt, 
                              helper));
    }
    return ret;
}

u32 TransformUnnestJASubquery::transform_on_condition(DMLStmt_s &stmt,
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
            for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
                ExprStmt_s &expr = joined_table->join_condition[i];
                if (!expr->table_ids.is_subset(helper.table_item->table_ids)) {
                    continue;
                } else {
                    CHECK(transform_one_expr(stmt,
                                             expr, 
                                             helper, 
                                             happened));
                    if (happened) {
                        LOG_TRACE("succeed to transfom one expr", K(helper));
                    }
                }
            }
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
    CHECK(transform_exprs(stmt, 
                          filters, 
                          helper));
    table = helper.table_item;
    table->table_filter = filters;
    return ret;
}

u32 TransformUnnestJASubquery::transform_exprs(DMLStmt_s &stmt,
                                             Vector<ExprStmt_s> &exprs, 
                                             TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    bool happened = false;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(transform_one_expr(stmt,
                                 exprs[i], 
                                 helper, 
                                 happened));
        if (happened) {
            LOG_TRACE("succeed to transfom one expr", K(helper));
        }
    }
    return ret;
}

u32 TransformUnnestJASubquery::transform_one_expr(DMLStmt_s &stmt,
                                                ExprStmt_s &expr, 
                                                TransfomHelper &helper, 
                                                bool &happened)
{
    u32 ret = SUCCESS;
    happened = false;
    if (expr->has_flag(IS_SUBQUERY)) {
        bool is_valid = false;
        helper.reset();
        CHECK(check_expr_need_transform(expr, helper, is_valid));
        if (is_valid) {
            bool is_disable = false;
            CHECK(check_hint_disable(helper.subquery, is_disable));
            if (!is_disable) {
                CHECK(do_transform(stmt, expr, helper));
                CHECK(generate_outline(helper.subquery));
                set_transform_happened();
                happened = true;
            }
        }
    } else if (expr->has_flag(HAS_SUBQUERY)) {
        if (expr->has_flag(IS_OP_EXPR)) {
            OpExprStmt_s op_expr = expr;
            if (OP_EXISTS == op_expr->op_type ||
                OP_NOT_EXISTS == op_expr->op_type ||
                OP_IN == op_expr->op_type ||
                OP_NOT_IN == op_expr->op_type ||
                (OP_EQ_ANY <= op_expr->op_type &&
                OP_LT_ALL >= op_expr->op_type)) {
                return ret;
            }
        }
        CHECK(transform_exprs(stmt, 
                              expr->params, 
                              helper));
    }
    return ret;
}

u32 TransformUnnestJASubquery::check_expr_need_transform(ExprStmt_s &expr, 
                                                        TransfomHelper &helper, 
                                                        bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (!expr->has_flag(IS_SUBQUERY)) {
        return ret;
    }
    SubQueryStmt_s subquery_expr = expr;
    helper.subquery = subquery_expr->query_stmt;
    if (helper.subquery->has_limit() || 
        !helper.subquery->is_scalar_group_by() ||
        1 != helper.subquery->select_expr_list.size()) {
        return ret;
    }
    if (subquery_expr->exec_params.empty()) {
        is_valid = true;
        return ret;
    }
    CHECK(check_correlated_exprs(helper, 
                                 subquery_expr,
                                 is_valid));
    if (!is_valid) {
        return ret;
    }
    return ret;
}

u32 TransformUnnestJASubquery::check_correlated_exprs(TransfomHelper &helper, 
                                                      SubQueryStmt_s &subquery_expr,
                                                      bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    bool has_correlated_expr = false;
    SelectStmt_s &subquery = helper.subquery;
    //check child stmt
    Vector<SelectStmt_s> child_stmts;
    CHECK(subquery->get_child_stmts(child_stmts));
    for (u32 i = 0; !has_correlated_expr && i < child_stmts.size(); ++i) {
        CHECK(TransformUtils::check_stmt_correlated(child_stmts[i], 
                                                    subquery_expr, 
                                                    has_correlated_expr));
    }
    if (has_correlated_expr) {
        is_valid = false;
        return ret;
    }
    //check join condition
    for (u32 i = 0; i < subquery->from_stmts.size(); ++i) {
        CHECK(check_join_condition(helper,
                                   subquery->from_stmts[i],
                                   subquery_expr,
                                   helper.correlated_exprs,
                                   has_correlated_expr,
                                   is_valid));
        if (!is_valid) {
            return ret;
        }
    }
    //check where condition
    for (u32 i = 0; i < subquery->where_stmt.size(); ++i) {
        CHECK(TransformUtils::has_correlated_expr(subquery->where_stmt[i],
                                                  subquery_expr,
                                                  has_correlated_expr));
        if (has_correlated_expr) {
            helper.correlated_exprs.push_back(subquery->where_stmt[i]);
        }
    }
    //check select expr
    CHECK(TransformUtils::has_correlated_expr(subquery->select_expr_list,
                                              subquery_expr,
                                              has_correlated_expr));
    if (has_correlated_expr) {
        is_valid = false;
        return ret;
    }
    for (u32 i = 0; is_valid && i < helper.correlated_exprs.size(); ++i) {
        CHECK(is_simple_correlated_expr(helper,
                                        helper.correlated_exprs[i], 
                                        subquery_expr,
                                        is_valid));
    }
    return ret;
}

u32 TransformUnnestJASubquery::check_join_condition(TransfomHelper &helper, 
                                                    TableStmt_s &table,
                                                    SubQueryStmt_s &subquery_expr,
                                                    Vector<ExprStmt_s> &correlated_exprs,
                                                    bool &has_correlated_expr,
                                                    bool &is_valid)
{
    u32 ret = SUCCESS;
    bool has = false;
    is_valid = true;
    has_correlated_expr = false;
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
            CHECK(TransformUtils::has_correlated_expr(joined_table->join_condition[i],
                                                    subquery_expr,
                                                    has));
            if (has) {
                if (Inner != joined_table->join_type) {
                    is_valid = false;
                    return ret;
                }
                correlated_exprs.push_back(joined_table->join_condition[i]);
                has_correlated_expr = true;
            }
        }
        bool l_has_correlated_expr = false;
        bool r_has_correlated_expr = false;
        CHECK(check_join_condition(helper, 
                                joined_table->left_table,
                                subquery_expr,
                                correlated_exprs,
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
                                subquery_expr,
                                correlated_exprs,
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
    }
    for (u32 i = 0; i < table->table_filter.size(); ++i) {
        CHECK(TransformUtils::has_correlated_expr(table->table_filter[i],
                                                  subquery_expr,
                                                  has));
        if (has) {
            correlated_exprs.push_back(table->table_filter[i]);
            has_correlated_expr = true;
        }
    }
    return ret;
}

u32 TransformUnnestJASubquery::is_simple_correlated_expr(TransfomHelper &helper,
                                                        ExprStmt_s &correlated_expr, 
                                                        SubQueryStmt_s &subquery_expr,
                                                        bool &is_simple)
{
    u32 ret = SUCCESS;
    is_simple = false;
    if (!correlated_expr->has_flag(IS_OP_EXPR)) {
        return ret;
    } else if (2 != correlated_expr->params.size()) {
        return ret;
    } else {
        bool has_correlated_expr = false;
        ExprStmt_s &l_expr = correlated_expr->params[0];
        ExprStmt_s &r_expr = correlated_expr->params[1];
        OpExprStmt_s op_expr = correlated_expr;
        if (OP_EQ != op_expr->op_type) {
            return ret;
        } else if (!l_expr->table_ids.is_empty() && 
                   !r_expr->table_ids.is_empty()) {
            return ret;
        } else if (!l_expr->table_ids.is_empty()) {
            CHECK(TransformUtils::has_correlated_expr(l_expr,
                                                    subquery_expr,
                                                    has_correlated_expr));
            if (!has_correlated_expr) {
                helper.inner_exprs.push_back(l_expr);
                helper.outer_exprs.push_back(r_expr);
                is_simple = true;
                return ret;
            }
        } else if (!r_expr->table_ids.is_empty()) {
            CHECK(TransformUtils::has_correlated_expr(r_expr,
                                                    subquery_expr,
                                                    has_correlated_expr));
            if (!has_correlated_expr) {
                helper.inner_exprs.push_back(r_expr);
                helper.outer_exprs.push_back(l_expr);
                is_simple = true;
                return ret;
            }
        } else {
            CHECK(TransformUtils::has_correlated_expr(l_expr,
                                                    subquery_expr,
                                                    has_correlated_expr));
            if (!has_correlated_expr) {
                helper.inner_exprs.push_back(l_expr);
                helper.outer_exprs.push_back(r_expr);
                is_simple = true;
                return ret;
            }
            CHECK(TransformUtils::has_correlated_expr(r_expr,
                                                    subquery_expr,
                                                    has_correlated_expr));
            if (!has_correlated_expr) {
                helper.inner_exprs.push_back(r_expr);
                helper.outer_exprs.push_back(l_expr);
                is_simple = true;
                return ret;
            }
        }
    }
    return ret;
}

u32 TransformUnnestJASubquery::do_transform(DMLStmt_s &stmt,
                                            ExprStmt_s &expr,
                                            TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    MY_ASSERT(1 == helper.subquery->select_expr_list.size());
    if (!helper.is_on_condition) {
        CHECK(TransformUtils::build_joined_table(stmt->from_stmts, 
                                                helper.table_item));
    }
    //adjust subquery
    ExprStmt_s value_expr = helper.subquery->select_expr_list[0];
    helper.subquery->select_expr_list.clear();
    helper.subquery->group_exprs = helper.inner_exprs;
    Vector<ExprStmt_s> aggr_exprs;
    Vector<ExprStmt_s> columns_part1;
    Vector<ExprStmt_s> columns_part2;
    CHECK(ExprUtils::extract_aggr_exprs(value_expr, aggr_exprs));
    //create view table
    ViewTableStmt_s view_table;
    CHECK(TransformUtils::create_table_with_view(helper.subquery, 
                                                 view_table, 
                                                 ctx));
    CHECK(TransformUtils::create_select_item_for_view_table(view_table, 
                                                            helper.inner_exprs, 
                                                            columns_part1));
    CHECK(TransformUtils::create_select_item_for_view_table(view_table, 
                                                            aggr_exprs, 
                                                            columns_part2));
    //remove correlated exprs
    SubQueryStmt_s subquery_expr = expr;
    if (subquery_expr->has_related_expr()) {
        CHECK(remove_correlated_exprs(subquery_expr));
    }
    //create join condition
    Vector<ExprStmt_s> exec_params;
    append(exec_params, subquery_expr->exec_params);
    CHECK(ExprUtils::replace_exprs(exec_params, 
                                   subquery_expr->params, 
                                   helper.correlated_exprs));
    CHECK(ExprUtils::replace_exprs(helper.inner_exprs, 
                                   columns_part1, 
                                   helper.correlated_exprs));
    //create left join
    TableStmt_s right_table = view_table;
    JoinType join_type = LeftOuter;
    if (!subquery_expr->has_related_expr()) {
        join_type = Inner;
    }
    JoinedTableStmt_s joined_table = JoinedTableStmt::make_joined_table(helper.table_item,
                                                                        right_table,
                                                                        join_type);
    joined_table->join_condition = helper.correlated_exprs;
    helper.table_item = joined_table;
    //transform return expr
    if (subquery_expr->has_related_expr()) {
        MY_ASSERT(!columns_part1.empty());
        CHECK(try_add_case_when(aggr_exprs, 
                                columns_part2, 
                                columns_part1[0]));
    }
    CHECK(ExprUtils::replace_expr(aggr_exprs, 
                                columns_part2, 
                                value_expr));
    value_expr->alias_name = expr->alias_name;
    expr = value_expr;
    CHECK(joined_table->formalize());
    if (!helper.is_on_condition) {
        stmt->from_stmts.clear();
        stmt->from_stmts.push_back(helper.table_item);
    }
    return ret;
}

u32 TransformUnnestJASubquery::remove_correlated_exprs(SubQueryStmt_s &subquery_expr)
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
    return ret;
}

u32 TransformUnnestJASubquery::remove_correlated_exprs(TableStmt_s &table,
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

u32 TransformUnnestJASubquery::remove_correlated_exprs(Vector<ExprStmt_s> &exprs,
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

u32 TransformUnnestJASubquery::try_add_case_when(Vector<ExprStmt_s> &aggr_exprs,
                                                Vector<ExprStmt_s> &column_exprs,
                                                ExprStmt_s &not_null_expr)
{
    u32 ret = SUCCESS;
    ExprStmt_s new_expr;
    ExprStmt_s is_null_expr;
    ExprStmt_s zero_expr;
    CHECK(ExprUtils::make_int_expr(zero_expr, 0));
    CHECK(ExprUtils::make_is_null_expr(not_null_expr, is_null_expr));
    MY_ASSERT(aggr_exprs.size() == column_exprs.size());

    for (u32 i = 0; i < aggr_exprs.size(); ++i) {
        MY_ASSERT(aggr_exprs[i]->has_flag(IS_AGG));
        AggrStmt_s aggr_expr = aggr_exprs[i];
        if (COUNT == aggr_expr->aggr_func) {
            new_expr = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
            new_expr->params.push_back(is_null_expr); 
            new_expr->params.push_back(zero_expr); 
            new_expr->params.push_back(column_exprs[i]); 
            column_exprs[i] = new_expr;
        }
    }
    return ret;
}

u32 TransformUnnestJASubquery::check_hint_disable(SelectStmt_s& subquery, bool &is_disable)
{
    u32 ret = SUCCESS;
    is_disable = false;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    is_disable = query_hint.enable_no_unnest(subquery->get_qb_name());
    return ret;
}

u32 TransformUnnestJASubquery::generate_outline(SelectStmt_s& subquery)
{
    u32 ret = SUCCESS;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    CHECK(query_hint.generate_transform_outline(subquery->get_qb_name(), control_hint));
    return ret;
}