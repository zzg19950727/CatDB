#include "transform_win_magic.h"
#include "transform_utils.h"
#include "stmt_compare.h"
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

TransformWinMagic::TransformWinMagic()
    :TransformRule(PRE_ORDER, WIN_MAGIC)
{

}

TransformWinMagic::~TransformWinMagic()
{

}

u32 TransformWinMagic::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    TransfomHelper helper;
    bool is_valid = false;
    LOG_TRACE("start transform where conditions");
    CHECK(get_transform_expr(stmt,
                             stmt->where_stmt, 
                             helper,
                             is_valid));
    if (is_valid) {
        CHECK(do_transform(stmt, helper));
        CHECK(generate_outline(stmt, helper.subquery_expr->query_stmt));
        set_transform_happened();
        LOG_TRACE("succeed to transform where conditions", K(stmt));
    }
    if (stmt->is_select_stmt()) {
        SelectStmt_s sel_stmt = stmt;
        helper.reset();
        helper.from_having = true;
        LOG_TRACE("start transform having conditions");
        CHECK(get_transform_expr(stmt,
                                sel_stmt->having_stmt, 
                                helper,
                                is_valid));
        if (is_valid) {
            CHECK(do_transform(stmt, helper));
            CHECK(generate_outline(stmt, helper.subquery_expr->query_stmt));
            set_transform_happened();
            LOG_TRACE("succeed to transform having conditions", K(stmt));
        }
    }
    return ret;
}

u32 TransformWinMagic::get_transform_expr(DMLStmt_s &stmt,
                                          Vector<ExprStmt_s> &exprs, 
                                          TransfomHelper &helper,
                                          bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(get_transform_expr(stmt,
                                 exprs[i], 
                                 helper, 
                                 is_valid));
        if (is_valid) {
            LOG_TRACE("succeed to get transfom one expr", K(helper));
			break;
        }
    }
    return ret;
}

u32 TransformWinMagic::get_transform_expr(DMLStmt_s &stmt,
                                          ExprStmt_s &expr, 
                                          TransfomHelper &helper, 
                                          bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (expr->has_flag(IS_SUBQUERY)) {
        CHECK(check_expr_need_transform(stmt,
                                        expr, 
                                        helper, 
                                        is_valid));
        if (is_valid) {
            bool is_disable = false;
            CHECK(check_hint_disable(stmt, 
                                     helper.subquery_expr->query_stmt, 
                                     is_disable));
            if (is_disable) {
                is_valid = false;
            }
        }
    } else if (expr->has_flag(HAS_SUBQUERY)) {
        CHECK(get_transform_expr(stmt, 
                                 expr->params, 
                                 helper,
                                 is_valid));
    }
    return ret;
}

u32 TransformWinMagic::check_expr_need_transform(DMLStmt_s &stmt,
                                                 ExprStmt_s &expr, 
                                                 TransfomHelper &helper, 
                                                 bool &is_valid)
{
    u32 ret = SUCCESS;
    is_valid = false;
    if (!expr->has_flag(IS_SUBQUERY)) {
        return ret;
    }
    SubQueryStmt_s subquery_expr = expr;
    SelectStmt_s subquery = subquery_expr->query_stmt;
    if (subquery->has_limit() || 
        !subquery->is_scalar_group_by() ||
        1 != subquery->select_expr_list.size()) {
        return ret;
    }
    DMLStmt_s sub_stmt = subquery;
    helper.cmp_map.reset();
    CHECK(StmtCompare::compare_stmt(sub_stmt, stmt, helper.cmp_map));
    is_valid = true;
    for (u32 i = 0; is_valid && i < helper.cmp_map.table_map.size(); ++i) {
        if (INVALID_IDX == helper.cmp_map.table_map[i]) {
            is_valid = false;
            return ret;
        }
    }
    for (u32 i = 0; is_valid && i < helper.cmp_map.where_map.size(); ++i) {
        MY_ASSERT(i < subquery->where_stmt.size());
        if (INVALID_IDX == helper.cmp_map.where_map[i]) {
            ExprStmt_s &expr = subquery->where_stmt[i];
            if (!helper.from_having && 
                expr->has_flag(HAS_EXEC_PARAM)) {
                CHECK(is_simple_expr(helper,
                                     stmt,
                                     subquery_expr,
                                     expr, 
                                     is_valid));
            } else {
                is_valid = false;
                return ret;
            }
        }
    }
    if (helper.from_having) {
        if (stmt->from_stmts.size() != subquery->from_stmts.size() ||
            stmt->where_stmt.size() != subquery->where_stmt.size()) {
            is_valid = false;
        } else {
            //check aggr item can be split
            Vector<ExprStmt_s> &aggr_exprs = subquery->get_aggr_exprs();
            for (u32 i = 0; is_valid && i < aggr_exprs.size(); ++i) {
                AggrStmt_s aggr_expr = aggr_exprs[i];
                if (aggr_expr->distinct || AVG == aggr_expr->aggr_func) {
                    is_valid = false;
                }
            }
        }
    }
    if (is_valid) {
        helper.subquery_expr = subquery_expr;
    }
    return ret;
}

u32 TransformWinMagic::is_simple_expr(TransfomHelper &helper,
                                      DMLStmt_s &stmt,
                                      SubQueryStmt_s &subquery_expr,
                                      ExprStmt_s &expr,
                                      bool &is_simple)
{
    u32 ret = SUCCESS;
    is_simple = false;
    if (expr->has_flag(IS_OP_EXPR)) {
        OpExprStmt_s op_expr = expr;
        if (OP_EQ == op_expr->op_type) {
            ExprStmt_s inner_expr;
            ExprStmt_s outer_expr;
            MY_ASSERT(2 == op_expr->params.size());
            if (op_expr->params[0]->has_flag(IS_EXEC_PARAM) && 
                !op_expr->params[1]->has_flag(HAS_EXEC_PARAM)) {
                inner_expr = op_expr->params[1];
                ExecParamStmt_s exec_param = op_expr->params[0];
                subquery_expr->get_related_exprs(exec_param, outer_expr);
            } else if (op_expr->params[1]->has_flag(IS_EXEC_PARAM) && 
                       !op_expr->params[0]->has_flag(HAS_EXEC_PARAM)) {
                inner_expr = op_expr->params[0];
                ExecParamStmt_s exec_param = op_expr->params[1];
                subquery_expr->get_related_exprs(exec_param, outer_expr);
            }
            if (inner_expr && outer_expr) {
                StmtCompareCtx ctx(helper.cmp_map);
                is_simple = inner_expr->same_as(outer_expr, &ctx);
                if (is_simple) {
                    helper.partition_by_exprs.push_back(inner_expr);
                } else {
                    CHECK(find_same_condition(helper, 
                                              stmt, 
                                              inner_expr, 
                                              outer_expr, 
                                              is_simple));
                    if (is_simple) {
                        helper.partition_by_exprs.push_back(inner_expr);
                    }
                }
            }
        }
    }
    return ret;
}

u32 TransformWinMagic::find_same_condition(TransfomHelper &helper,
                                            DMLStmt_s &stmt,
                                            ExprStmt_s &inner_expr,
                                            ExprStmt_s &outer_expr,
                                            bool &find)
{
    u32 ret = SUCCESS;
    find = false;
    StmtCompareCtx ctx(helper.cmp_map);
    for (u32 i = 0; !find && i < stmt->where_stmt.size(); ++i) {
        ExprStmt_s &expr = stmt->where_stmt[i];
        if (expr->has_flag(IS_OP_EXPR)) {
            OpExprStmt_s op_expr = expr;
            if (OP_EQ == op_expr->op_type) {
                MY_ASSERT(2 == op_expr->params.size());
                find = (inner_expr->same_as(op_expr->params[0], &ctx) && 
                        outer_expr->same_as(op_expr->params[1], &ctx)) ||
                       (inner_expr->same_as(op_expr->params[1], &ctx) && 
                        outer_expr->same_as(op_expr->params[0], &ctx));
            }
        }
    }
    return ret;
}

u32 TransformWinMagic::do_transform(DMLStmt_s &stmt,
                                    TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    if (helper.from_having) {
        CHECK(do_transform_for_having(stmt, helper));
    } else {
        CHECK(do_transform_for_where(stmt, helper));
    }
    return ret;
}

u32 TransformWinMagic::do_transform_for_where(DMLStmt_s &stmt,
                                              TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    ExprStmt_s expr;
    Vector<ExprStmt_s> new_select_exprs;
    Vector<ExprStmt_s> new_column_exprs;
    Vector<ExprStmt_s> old_column_exprs;
    SelectStmt_s &subquery = helper.subquery_expr->query_stmt;
    //transform aggr item
    CHECK(transform_aggr_expr_to_win_expr(helper, expr));
    new_select_exprs.push_back(expr);
    subquery->select_expr_list.clear();
    //remove correlated expr for subquery
    Vector<ExprStmt_s> new_conditions;
    for (u32 i = 0; i < helper.cmp_map.where_map.size(); ++i) {
        if (INVALID_IDX != helper.cmp_map.where_map[i]) {
            MY_ASSERT(i < subquery->where_stmt.size());
            new_conditions.push_back(subquery->where_stmt[i]);
        }
    }
    subquery->where_stmt = new_conditions;
    //remove dummy table for upper stmt
    Vector<TableStmt_s> new_tables;
    Vector<u32> remove_table_ids;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        bool remove = false;
        for (u32 j = 0; !remove && j < helper.cmp_map.table_map.size(); ++j) {
            int idx = helper.cmp_map.table_map[j];
            if (i == idx) {
                remove = true;
            }
        }
        if (remove) {
            CHECK(TransformUtils::get_table_ids(stmt->from_stmts[i], remove_table_ids));
        } else {
            new_tables.push_back(stmt->from_stmts[i]);
        }
    }
    stmt->from_stmts = new_tables;
    //remove dummy conditions for upper stmt
    new_conditions.clear();
    for (u32 i = 0; i < stmt->where_stmt.size(); ++i) {
        bool remove = false;
        for (u32 j = 0; !remove && j < helper.cmp_map.where_map.size(); ++j) {
            int idx = helper.cmp_map.where_map[j];
            if (i == idx) {
                remove = true;
            }
        }
        if (!remove) {
            new_conditions.push_back(stmt->where_stmt[i]);
        }
    }
    stmt->where_stmt = new_conditions;
    //collect old columns
    for (u32 i = 0; i < remove_table_ids.size(); ++i) {
        u32 src_table_id = remove_table_ids[i];
        u32 dst_table_id = INVALID_ID;
        for (u32 j = 0; j < helper.cmp_map.table_id_map.size(); ++j) {
            if (helper.cmp_map.table_id_map[j].second == src_table_id) {
                dst_table_id = helper.cmp_map.table_id_map[j].first;
                break;
            }
        }
        MY_ASSERT(INVALID_ID != dst_table_id);
        CHECK(stmt->replace_table_id_of_columns(src_table_id, dst_table_id));
        CHECK(stmt->get_column_exprs(dst_table_id, old_column_exprs));
    }
    //create new select expr for subquery
    append(new_select_exprs, old_column_exprs);
    ViewTableStmt_s view_table;
    CHECK(TransformUtils::create_table_with_view(subquery, view_table));
    CHECK(TransformUtils::create_select_item_for_view_table(view_table, 
                                                            new_select_exprs, 
                                                            new_column_exprs));
    //replace expr
    if (!new_select_exprs.empty()) {
        new_select_exprs[0] = helper.subquery_expr;
    }
    CHECK(stmt->replace_stmt_exprs(new_select_exprs, new_column_exprs));
    stmt->from_stmts.push_back(view_table);
    return ret;
}

u32 TransformWinMagic::do_transform_for_having(DMLStmt_s &stmt,
                                               TransfomHelper &helper)
{
    u32 ret = SUCCESS;
    ExprStmt_s expr;
    Vector<ExprStmt_s> new_select_exprs;
    Vector<ExprStmt_s> new_column_exprs;
    SelectStmt_s &subquery = helper.subquery_expr->query_stmt;
    SelectStmt_s sel_stmt = stmt;
    //transform aggr item
    CHECK(transform_aggr_expr_to_win_expr(helper, expr));
    subquery->select_expr_list.clear();
    //remove dummy table for upper stmt
    Vector<u32> remove_table_ids;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        CHECK(TransformUtils::get_table_ids(stmt->from_stmts[i], remove_table_ids));
    }
    stmt->from_stmts.clear();
    //remove dummy conditions for upper stmt
    stmt->where_stmt.clear();
    //collect old columns
    for (u32 i = 0; i < remove_table_ids.size(); ++i) {
        u32 src_table_id = remove_table_ids[i];
        u32 dst_table_id = INVALID_ID;
        for (u32 j = 0; j < helper.cmp_map.table_id_map.size(); ++j) {
            if (helper.cmp_map.table_id_map[j].second == src_table_id) {
                dst_table_id = helper.cmp_map.table_id_map[j].first;
                break;
            }
        }
        MY_ASSERT(INVALID_ID != dst_table_id);
        CHECK(stmt->replace_table_id_of_columns(src_table_id, dst_table_id));
    }
    //create new select expr for subquery
    append(new_select_exprs, sel_stmt->group_exprs);
    //collect aggr expr
    append(new_select_exprs, sel_stmt->get_aggr_exprs());
    append(new_select_exprs, subquery->get_aggr_exprs());
    //pushdown group exprs
    append(subquery->group_exprs, sel_stmt->group_exprs);
    sel_stmt->group_exprs.clear();
    ViewTableStmt_s view_table;
    CHECK(TransformUtils::create_table_with_view(subquery, view_table));
    CHECK(TransformUtils::create_select_item_for_view_table(view_table, 
                                                            new_select_exprs, 
                                                            new_column_exprs));
    //replace expr
    CHECK(stmt->replace_stmt_exprs(new_select_exprs, new_column_exprs));
    CHECK(ExprUtils::replace_expr(new_select_exprs, new_column_exprs, expr));
    //create win func view
    SelectStmt_s win_func_view;
    TableStmt_s table = view_table;
    CHECK(TransformUtils::create_view_with_table_item(table, win_func_view));
    new_select_exprs.clear();
    new_select_exprs.push_back(expr);
    append(new_select_exprs, new_column_exprs);
    new_column_exprs.clear();
    CHECK(TransformUtils::create_table_with_view(win_func_view, view_table));
    CHECK(TransformUtils::create_select_item_for_view_table(view_table, 
                                                            new_select_exprs, 
                                                            new_column_exprs));
    new_select_exprs[0] = helper.subquery_expr;
    //replace expr
    CHECK(stmt->replace_stmt_exprs(new_select_exprs, new_column_exprs));
    //move having condition to where condition
    sel_stmt->where_stmt = sel_stmt->having_stmt;
    sel_stmt->having_stmt.clear();
    stmt->from_stmts.push_back(view_table);
    return ret;
}

u32 TransformWinMagic::transform_aggr_expr_to_win_expr(TransfomHelper &helper, ExprStmt_s &expr)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> aggr_exprs;
    Vector<ExprStmt_s> win_exprs;
    SelectStmt_s &subquery = helper.subquery_expr->query_stmt;
    MY_ASSERT(1 == subquery->select_expr_list.size());
    expr = subquery->select_expr_list[0];
    CHECK(ExprUtils::extract_aggr_exprs(expr, aggr_exprs));
    ExprStmt_s dst_expr;
    for (u32 i = 0; i < aggr_exprs.size(); ++i) {
        CHECK(transform_one_aggr_expr_to_win_expr(helper, aggr_exprs[i], dst_expr));
        win_exprs.push_back(dst_expr);
    }
    CHECK(ExprUtils::replace_expr(aggr_exprs, win_exprs, expr));
    return ret;
}

/**
 * for where condition: 
 *      SUM     ->  SUM
 *      COUNT   ->  COUNT
 *      MIN     ->  MIN 
 *      MAX     ->  MAX
 * for having condition:
 *      SUM     ->  SUM( SUM )
 *      COUNT   ->  SUM( COUNT )
 *      MIN     ->  MIN( MIN )
 *      MAX     ->  MAX( MAX )
 */
u32 TransformWinMagic::transform_one_aggr_expr_to_win_expr(TransfomHelper &helper, 
                                                           ExprStmt_s &src_expr,
                                                           ExprStmt_s &dst_expr)
{
    u32 ret = SUCCESS;
    MY_ASSERT(src_expr->has_flag(IS_AGG));
    AggrStmt_s aggr_expr = src_expr;
    MY_ASSERT(AVG != aggr_expr->aggr_func);
    WinType win_type = WIN_SUM;
    switch (aggr_expr->aggr_func)
    {
    case SUM:
        win_type = WIN_SUM;
        break;
    case COUNT:
        if (helper.from_having) {
            win_type = WIN_SUM;
        } else {
            win_type = WIN_COUNT;
        }
        break;
    case MIN:
        win_type = WIN_MIN;
        break;
    case MAX:
        win_type = WIN_MAX;
        break;
    default:
        break;
    }
    WinExprStmt_s win_expr = WinExprStmt::make_win_expr_stmt(win_type);
    win_expr->is_distinct = aggr_expr->distinct;
    if (helper.from_having) {
        win_expr->set_win_func_expr(aggr_expr);
    } else {
        win_expr->set_win_func_expr(aggr_expr->get_aggr_expr());
    }
    win_expr->set_win_part_by_exprs(helper.partition_by_exprs);
    dst_expr = win_expr;
    CHECK(dst_expr->formalize());
    return ret;
}

u32 TransformWinMagic::check_hint_disable(DMLStmt_s &stmt,
                                          SelectStmt_s& subquery, 
                                          bool &is_disable)
{
    u32 ret = SUCCESS;
    is_disable = false;
    QueryHint_s &query_hint = QUERY_CTX->query_hint;
    is_disable = query_hint->enable_no_win_magic(stmt->get_qb_name(), subquery->get_qb_name());
    return ret;
}

u32 TransformWinMagic::generate_outline(DMLStmt_s &stmt,
                                        SelectStmt_s& subquery)
{
    u32 ret = SUCCESS;
    QueryHint_s &query_hint = QUERY_CTX->query_hint;
    CHECK(query_hint->generate_win_magic_outline(stmt->get_qb_name(), 
                                                control_hint,
                                                subquery->get_qb_name()));
    return ret;
}
