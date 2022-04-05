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

u32 EstSelUtil::calc_selectivity(EstInfo_s &est_info, Vector<ExprStmt_s> &exprs, double &selectivity)
{
    u32 ret = SUCCESS;
    selectivity = 1.0;
    double sel = 1.0;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(calc_selectivity(est_info, exprs[i], sel));
        selectivity *= sel;
    }
    refine_selectivity(selectivity);
    return ret;
}

u32 EstSelUtil::calc_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity)
{
    u32 ret = SUCCESS;
    selectivity = 0.3;
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
            case OP_INVALID:
                ret = ERR_UNEXPECTED;
        }
    }
    refine_selectivity(selectivity);
    LOG_TRACE("expr`s selectivity:", K(expr), K(selectivity));
    return ret;
}

u32 EstSelUtil::calc_distinct_count(EstInfo_s &est_info, Vector<ExprStmt_s> &exprs, double &ndv)
{
    u32 ret = SUCCESS;
    ndv = 1.0;
    double single_ndv = 1.0;
    for (u32 i = 0; i < exprs.size(); ++i) {
        CHECK(calc_distinct_count(est_info, exprs[i], single_ndv));
        ndv *= single_ndv;
    }
    return ret;
}

u32 EstSelUtil::calc_distinct_count(EstInfo_s &est_info, ExprStmt_s &expr, double &ndv)
{
    u32 ret = SUCCESS;
    Vector<ColumnStmt_s> columns;
    CHECK(ExprUtils::get_column_exprs(expr, INVALID_ID, columns));
    ndv = 1.0;
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnStmt_s &column = columns[i];
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
        ndv *= column_statis->ndv;
    }
    if (ndv < 1.0) {
        ndv = 1.0;
    }
    LOG_TRACE("expr`s ndv:", K(expr), K(ndv));
    return ret;
}

u32 EstSelUtil::calc_eq_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity)
{
    u32 ret = SUCCESS;
    double l_ndv, r_ndv;
    MY_ASSERT(expr, expr->params.size() == 2);
    CHECK(calc_distinct_count(est_info, expr->params[0], l_ndv));
    CHECK(calc_distinct_count(est_info, expr->params[1], r_ndv));
    selectivity = 1.0 / std::max(l_ndv, r_ndv);
    return ret;
}

u32 EstSelUtil::calc_bwt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity)
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
    CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
    selectivity = (r_band - l_band) / (column_statis->max_value - column_statis->min_value);
    return ret;
}

u32 EstSelUtil::calc_lt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr, expr->params.size() == 2);
    if (!((COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) ||
        (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()))) {
        selectivity = 0.3;
        return ret;
    }
    if (COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[0];
        ConstStmt_s r_const = expr->params[1];
        double r_band = r_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
        selectivity = (r_band - column_statis->min_value) / (column_statis->max_value - column_statis->min_value);
    } else if (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[1];
        ConstStmt_s l_const = expr->params[0]; 
        double l_band = l_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
        selectivity = (column_statis->max_value - l_band) / (column_statis->max_value - column_statis->min_value);
    }
    return ret;
}

u32 EstSelUtil::calc_gt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr, expr->params.size() == 2);
    if (!((COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) ||
        (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()))) {
        selectivity = 0.3;
        return ret;
    }
    if (COLUMN == expr->params[0]->expr_type() &&
        CONST == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[0];
        ConstStmt_s r_const = expr->params[1];
        double r_band = r_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
        selectivity = (column_statis->max_value - r_band) / (column_statis->max_value - column_statis->min_value);
    } else if (CONST == expr->params[0]->expr_type() &&
        COLUMN == expr->params[1]->expr_type()) {
        ColumnStmt_s column = expr->params[1];
        ConstStmt_s l_const = expr->params[0]; 
        double l_band = l_const->value->value();
        ColumnEstInfo_s column_statis;
        CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
        selectivity = (l_band - column_statis->min_value) / (column_statis->max_value - column_statis->min_value);
    }
    return ret;
}

u32 EstSelUtil::calc_is_null_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity)
{
    u32 ret = SUCCESS;
    Vector<ColumnStmt_s> columns;
    selectivity = 0;
    CHECK(ExprUtils::get_column_exprs(expr, INVALID_ID, columns));
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnStmt_s &column = columns[i];
        ColumnEstInfo_s column_statis;
        TableEstInfo_s table_statis;
        CHECK(est_info->get_table_statis(column->table_id, table_statis));
        CHECK(est_info->get_column_statis(column->table_id, column->column_id, column_statis));
        selectivity += column_statis->null_count / table_statis->table_rows;
    }
    return ret;
}