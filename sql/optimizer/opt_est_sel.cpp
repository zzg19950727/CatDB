#include "opt_est_sel.h"
#include "opt_est_info.h"
#include "expr_utils.h"
#include "expr_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

inline void refine_selectivity(double &sel) {
    if (sel < 0) {
        sel = 0;
    } else if (sel > 1) {
        sel = 1;
    }
}

u32 EstSelUtil::calc_selectivity(EstInfo_s &est_info, 
                                 Vector<ExprStmt_s> &exprs, 
                                 double &selectivity)
{
    u32 ret = SUCCESS;
    BitSet visited_exprs;
    selectivity = 1.0;
    double sel = 1.0;
    bool is_valid = false;
    BitSet table_ids;
    for (u32 i = 0; i < exprs.size(); ++i) {
        table_ids.add_members(exprs[i]->table_ids);
        if (visited_exprs.has_member(i)) {
            continue;
        }
        is_valid = false;
        for (u32 j = 0; j < exprs.size(); ++j) {
            if (visited_exprs.has_member(i)) {
                continue;
            }
            CHECK(calc_gt_lt_selectivity(est_info, 
                                         exprs[i], 
                                         exprs[j], 
                                         is_valid, 
                                         sel));
            if (is_valid) {
                selectivity *= sel;
                visited_exprs.add_member(i);
                visited_exprs.add_member(j);
                break;
            }
        }
        if (!is_valid) {
            CHECK(calc_selectivity(est_info, exprs[i], sel));
            selectivity *= sel;
        }
    }
    if (table_ids.num_members() <= 2) {
        //selectivity must greater than 1.0 / table_rows
        Vector<ColumnStmt_s> columns;
        CHECK(ExprUtils::get_column_exprs(exprs, INVALID_ID, columns));
        double max_rows = 0;
        for (u32 i = 0; i < columns.size(); ++i) {
            ColumnStmt_s &column = columns[i];
            TableEstInfo_s table_statis;
            CHECK(est_info->get_table_statis(column->table_id, table_statis));
            if (table_statis->table_rows > max_rows) {
                max_rows = table_statis->table_rows;
            }
        }
        if (max_rows > 0 && max_rows < 1.0 / selectivity) {
            selectivity = 1.0 / max_rows;
        }
    }
    refine_selectivity(selectivity);
    return ret;
}

u32 EstSelUtil::calc_selectivity(EstInfo_s &est_info, 
                                 ExprStmt_s &expr, 
                                 double &selectivity)
{
    u32 ret = SUCCESS;
    selectivity = 0.3;
    double sel = 1.0;
    if (OP_EXPR == expr->expr_type()) {
        OpExprStmt_s op_expr = expr;
        switch(op_expr->op_type) {
            case OP_EQ:
                CHECK(calc_eq_selectivity(est_info, expr, selectivity));
                break;
            case OP_NE:
                CHECK(calc_eq_selectivity(est_info, expr, selectivity));
                selectivity = 1.0 - selectivity;
                break;
            case OP_BETWEEN:
                CHECK(calc_bwt_selectivity(est_info, expr, selectivity));
                break;
            case OP_NOT_BETWEEN:
                CHECK(calc_bwt_selectivity(est_info, expr, selectivity));
                selectivity = 1.0 - selectivity;
                break;
            case OP_LE:
            case OP_LT:
                CHECK(calc_lt_selectivity(est_info, expr, selectivity));
                break;
            case OP_GE:
            case OP_GT:
                CHECK(calc_gt_selectivity(est_info, expr, selectivity));
                break;
            case OP_IS_NULL:
                CHECK(calc_is_null_selectivity(est_info, expr, selectivity));
                break;
            case OP_IS_NOT_NULL:
                CHECK(calc_is_null_selectivity(est_info, expr, selectivity));
                selectivity = 1.0 - selectivity;
                break;
            case OP_IN_LIST:
                CHECK(calc_in_selectivity(est_info, expr, selectivity));
                break;
            case OP_NOT_IN_LIST:
                CHECK(calc_in_selectivity(est_info, expr, selectivity));
                selectivity = 1.0 - selectivity;
                break;
            case OP_OR:
            {
                sel = 0;
                if (expr->table_ids.num_members() == 1) {
                    for (u32 i = 0; i < expr->params.size(); ++i) {
                        CHECK(calc_selectivity(est_info, expr->params[i], selectivity));
                        sel += selectivity;
                    }
                    selectivity = sel;
                }
                break;
            }
            case OP_AND:
            {
                sel = 1.0;
                if (expr->table_ids.num_members() == 1) {
                    for (u32 i = 0; i < expr->params.size(); ++i) {
                        CHECK(calc_selectivity(est_info, expr->params[i], selectivity));
                        sel *= selectivity;
                    }
                    selectivity = sel;
                }
                break;
            }
            case OP_LIKE:
                selectivity = 0.05;
                break;
            case OP_NOT_LIKE:
                selectivity = 1 - 0.05;
                break;
            case OP_INVALID:
                ret = ERR_UNEXPECTED;
            default:
                selectivity = 1.0 / 3.0;
        }
    }
    refine_selectivity(selectivity);
    LOG_TRACE("expr`s selectivity:", K(expr), K(selectivity));
    return ret;
}

u32 EstSelUtil::calc_distinct_count(EstInfo_s &est_info, 
                                    Vector<ExprStmt_s> &exprs, 
                                    double &ndv,
                                    bool use_ori_ndv)
{
    u32 ret = SUCCESS;
    ndv = 1.0;
    double single_ndv = 1.0;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(calc_distinct_count(est_info, exprs[i], single_ndv, use_ori_ndv));
        ndv *= single_ndv;
    }
    return ret;
}

u32 EstSelUtil::calc_distinct_count(EstInfo_s &est_info, 
                                    ExprStmt_s &expr, 
                                    double &ndv,
                                    bool use_ori_ndv)
{
    u32 ret = SUCCESS;
    Vector<ColumnStmt_s> columns;
    CHECK(ExprUtils::get_column_exprs(expr, INVALID_ID, columns));
    ndv = 1.0;
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnStmt_s &column = columns[i];
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, 
                                          column->column_id, 
                                          column_statis));
        double col_ndv = use_ori_ndv ? column_statis->ori_ndv : column_statis->ndv;
        if (col_ndv > ndv) {
            ndv = col_ndv;
        }
    }
    if (ndv < 1.0) {
        ndv = 1.0;
    }
    LOG_TRACE("expr`s ndv:", K(expr), K(ndv));
    return ret;
}

u32 EstSelUtil::calc_gt_lt_selectivity(EstInfo_s &est_info, 
                                        ExprStmt_s &expr1,
                                        ExprStmt_s &expr2, 
                                        bool &is_valid,
                                        double &selectivity)
{
    u32 ret = SUCCESS;
    if (OP_EXPR == expr1->expr_type() && 
        OP_EXPR == expr2->expr_type() &&
        2 == expr1->params.size() &&
        2 == expr2->params.size()) {
        OpExprStmt_s op_expr1 = expr1;
        OpExprStmt_s op_expr2 = expr2;
        ColumnStmt_s column_expr;
        ConstStmt_s l_band;
        ConstStmt_s g_band;
        if (COLUMN == op_expr1->params[0]->expr_type() &&
            CONST == op_expr1->params[1]->expr_type()) {
            if (COLUMN == op_expr2->params[0]->expr_type() &&
                CONST == op_expr2->params[1]->expr_type() &&
                op_expr1->params[0]->same_as(op_expr2->params[0])) {
                if ((OP_LT == op_expr1->op_type || 
                     OP_LE == op_expr1->op_type) && 
                    (OP_GT == op_expr2->op_type || 
                     OP_GE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[0];
                    l_band = op_expr2->params[1];
                    g_band = op_expr1->params[1]; 
                } else if ((OP_GT == op_expr1->op_type || 
                            OP_GE == op_expr1->op_type) && 
                            (OP_LT == op_expr2->op_type || 
                            OP_LE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[0];
                    l_band = op_expr1->params[1];
                    g_band = op_expr2->params[1]; 
                }
            } else if (COLUMN == op_expr2->params[1]->expr_type() &&
                        CONST == op_expr2->params[0]->expr_type()&&
                        op_expr1->params[0]->same_as(op_expr2->params[1])) {
                if ((OP_LT == op_expr1->op_type || 
                     OP_LE == op_expr1->op_type) && 
                    (OP_LT == op_expr2->op_type || 
                     OP_LE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[0];
                    l_band = op_expr2->params[0];
                    g_band = op_expr1->params[1]; 
                } else if ((OP_GT == op_expr1->op_type || 
                            OP_GE == op_expr1->op_type) && 
                            (OP_GT == op_expr2->op_type || 
                            OP_GE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[0];
                    l_band = op_expr1->params[1];
                    g_band = op_expr2->params[0]; 
                }            
            }
        } else if (COLUMN == op_expr1->params[1]->expr_type() &&
                    CONST == op_expr1->params[0]->expr_type()) {
            if (COLUMN == op_expr2->params[0]->expr_type() &&
                CONST == op_expr2->params[1]->expr_type()&&
                op_expr1->params[1]->same_as(op_expr2->params[0])) {
                if ((OP_LT == op_expr1->op_type || 
                     OP_LE == op_expr1->op_type) && 
                    (OP_LT == op_expr2->op_type || 
                     OP_LE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[1];
                    l_band = op_expr1->params[0];
                    g_band = op_expr2->params[1]; 
                } else if ((OP_GT == op_expr1->op_type || 
                            OP_GE == op_expr1->op_type) && 
                            (OP_GT == op_expr2->op_type || 
                            OP_GE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[1];
                    l_band = op_expr2->params[1];
                    g_band = op_expr1->params[0]; 
                }
            } else if (COLUMN == op_expr2->params[1]->expr_type() &&
                        CONST == op_expr2->params[0]->expr_type()&&
                        op_expr1->params[1]->same_as(op_expr2->params[1])) {
                if ((OP_LT == op_expr1->op_type || 
                     OP_LE == op_expr1->op_type) && 
                    (OP_GT == op_expr2->op_type || 
                     OP_GE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[1];
                    l_band = op_expr1->params[0];
                    g_band = op_expr2->params[0]; 
                } else if ((OP_GT == op_expr1->op_type || 
                            OP_GE == op_expr1->op_type) && 
                            (OP_LT == op_expr2->op_type || 
                            OP_LE == op_expr2->op_type)) {
                    column_expr = op_expr1->params[1];
                    l_band = op_expr2->params[0];
                    g_band = op_expr1->params[0]; 
                }            
            }            
        }
        if (column_expr && l_band && g_band) {
            double l_band_value = l_band->value->value();
            double g_band_value = g_band->value->value();
            ColumnEstInfo_s column_statis;
            CHECK(est_info->get_column_statis(column_expr->table_id, 
                                            column_expr->column_id, 
                                            column_statis));
            g_band_value = g_band_value <= column_statis->max_value ? g_band_value : column_statis->max_value;
            l_band_value = l_band_value >= column_statis->min_value ? l_band_value : column_statis->min_value;
            selectivity = (g_band_value - l_band_value) / 
                        (column_statis->max_value - column_statis->min_value);
            is_valid = true;
        }
    }
    return ret;
}

u32 EstSelUtil::calc_eq_selectivity(EstInfo_s &est_info, 
                                    ExprStmt_s &expr, 
                                    double &selectivity)
{
    u32 ret = SUCCESS;
    double l_ndv, r_ndv;
    MY_ASSERT(expr, expr->params.size() == 2);
    CHECK(calc_distinct_count(est_info, 
                              expr->params[0], 
                              l_ndv,
                              expr->table_ids.num_members() == 1));
    CHECK(calc_distinct_count(est_info, 
                              expr->params[1], 
                              r_ndv,
                              expr->table_ids.num_members() == 1));
    selectivity = 1.0 / std::max(l_ndv, r_ndv);
    return ret;
}

u32 EstSelUtil::calc_bwt_selectivity(EstInfo_s &est_info, 
                                     ExprStmt_s &expr, 
                                     double &selectivity)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr, expr->params.size() == 3);
    if (COLUMN != expr->params[0]->expr_type() ||
        CONST != expr->params[1]->expr_type() ||
        CONST != expr->params[2]->expr_type()) {
        selectivity = 0.3;
        return ret;
    }
    ColumnStmt_s column = expr->params[0];
    ConstStmt_s l_const = expr->params[1];
    ConstStmt_s r_const = expr->params[2];
    double l_band = l_const->value->value();
    double r_band = r_const->value->value();
    ColumnEstInfo_s column_statis;
    CHECK(est_info->get_column_statis(column->table_id, 
                                      column->column_id, 
                                      column_statis));
    r_band = r_band <= column_statis->max_value ? r_band : column_statis->max_value;
    l_band = l_band >= column_statis->min_value ? l_band : column_statis->min_value;
    selectivity = (r_band - l_band) / 
                (column_statis->max_value - column_statis->min_value);
    return ret;
}

u32 EstSelUtil::calc_lt_selectivity(EstInfo_s &est_info, 
                                    ExprStmt_s &expr, 
                                    double &selectivity)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr, expr->params.size() == 2);
    if (!((COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) ||
        (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()))) {
        selectivity = 0.5;
        return ret;
    }
    if (COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[0];
        ConstStmt_s r_const = expr->params[1];
        double r_band = r_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, 
                                          column->column_id, 
                                          column_statis));
        selectivity = (r_band - column_statis->min_value) / 
                    (column_statis->max_value - column_statis->min_value);
    } else if (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[1];
        ConstStmt_s l_const = expr->params[0]; 
        double l_band = l_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, 
                                          column->column_id, 
                                          column_statis));
        selectivity = (column_statis->max_value - l_band) / 
                    (column_statis->max_value - column_statis->min_value);
    }
    return ret;
}

u32 EstSelUtil::calc_gt_selectivity(EstInfo_s &est_info, 
                                    ExprStmt_s &expr, 
                                    double &selectivity)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr, expr->params.size() == 2);
    if (!((COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) ||
        (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()))) {
        selectivity = 0.5;
        return ret;
    }
    if (COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[0];
        ConstStmt_s r_const = expr->params[1];
        double r_band = r_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, 
                                          column->column_id, 
                                          column_statis));
        selectivity = (column_statis->max_value - r_band) / 
                    (column_statis->max_value - column_statis->min_value);
    } else if (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[1];
        ConstStmt_s l_const = expr->params[0]; 
        double l_band = l_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, 
                                          column->column_id, 
                                          column_statis));
        selectivity = (l_band - column_statis->min_value) / 
                    (column_statis->max_value - column_statis->min_value);
    }
    return ret;
}

u32 EstSelUtil::calc_is_null_selectivity(EstInfo_s &est_info, 
                                         ExprStmt_s &expr, 
                                         double &selectivity)
{
    u32 ret = SUCCESS;
    Vector<ColumnStmt_s> columns;
    selectivity = 0.3;
    CHECK(ExprUtils::get_column_exprs(expr, INVALID_ID, columns));
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnStmt_s &column = columns[i];
        ColumnEstInfo_s column_statis;
        TableEstInfo_s table_statis;
        CHECK(est_info->get_table_statis(column->table_id, table_statis));
        CHECK(est_info->get_column_statis(column->table_id, 
                                          column->column_id, 
                                          column_statis));
        selectivity += column_statis->null_count / 
                        table_statis->table_rows;
    }
    return ret;
}

u32 EstSelUtil::calc_in_selectivity(EstInfo_s &est_info, 
                                    ExprStmt_s &expr, 
                                    double &selectivity)
{
    u32 ret = SUCCESS;
    double l_ndv;
    MY_ASSERT(expr, expr->params.size() == 2);
    CHECK(calc_distinct_count(est_info, 
                              expr->params[0], 
                              l_ndv,
                              expr->table_ids.num_members() == 1));
    
    selectivity = expr->params[1]->params.size() * 1.0 / l_ndv;
    return ret;
}