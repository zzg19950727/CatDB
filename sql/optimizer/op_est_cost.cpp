#include "opt_est_cost.h"
#include "expr_stmt.h"
#include <cmath>

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

double COST_READ_ONE_BLOCK = 4.1;
double COST_DES_PER_COLUMN = 0.05;
double COST_CMP_PER_EXPR = 0.03;
double COST_BUILD_HASH_PER_ROW = 0.06;
double COST_PROBE_PER_ROW = 0.01;
double COST_JOIN_PER_ROW = 0.03;
double COST_HASH_PER_EXPR = 0.02;
double COST_DELETE_PER_ROW = 0.001;
double COST_GET_ONE_ROW = 0.02;
double COST_PER_AGG_EXPR = 0.035;
double COST_INSERT_PER_ROW = 1.0;

double EstCostUtil::sample_formula(double size, double red, double sample_size)
{
    if (size == 0 || red == 0) {
        return 0;
    } else {
        return red * (1.0 - std::pow((1.0 - sample_size / size), size / red));
    }
}

double EstCostUtil::cost_hash_expr(double rows, const Vector<ExprStmt_s> &exprs)
{
    return rows * exprs.size() * COST_BUILD_HASH_PER_ROW;
}

double EstCostUtil::cost_cmp_expr(double rows, const Vector<ExprStmt_s> &exprs)
{
    return rows * exprs.size() * COST_CMP_PER_EXPR;
}

double EstCostUtil::cost_table_scan(double rows,
                                    double block_count,
                                    const Vector<ExprStmt_s> &access_exprs)
{
    double io_cost = block_count * COST_READ_ONE_BLOCK;
    double cpu_cost = rows * access_exprs.size() * COST_DES_PER_COLUMN;
    double op_cost = io_cost + cpu_cost;
    return op_cost;
}

double EstCostUtil::cost_filters(double rows, const Vector<ExprStmt_s> &filters)
{
    return cost_cmp_expr(rows, filters);
}

double EstCostUtil::cost_hash_join(double left_rows,
                                double right_rows,
                                double output_rows,
                                const Vector<ExprStmt_s> &equal_join_condition,
                                const Vector<ExprStmt_s> &other_join_condition)
{
    double build_hash_cost = left_rows * COST_BUILD_HASH_PER_ROW;
    double hash_cost = cost_hash_expr(left_rows + right_rows, equal_join_condition);
    double probe_cost = right_rows * COST_PROBE_PER_ROW;
    double join_cost = output_rows * other_join_condition.size() * COST_JOIN_PER_ROW;
    double op_cost = build_hash_cost + hash_cost + probe_cost + join_cost;
    return op_cost;
}

double EstCostUtil::cost_nestloop_join(double left_rows,
                                    double right_rows,
                                    double right_cost,
                                    const Vector<ExprStmt_s> &equal_join_condition,
                                    const Vector<ExprStmt_s> &other_join_condition)
{
    double op_cost = left_rows * right_cost;
    op_cost += (equal_join_condition.size() + other_join_condition.size()) * 
                left_rows * right_rows * COST_JOIN_PER_ROW;
    return op_cost;
}

double EstCostUtil::cost_delete(double table_rows, 
                                double block_count, 
                                double delete_rows, 
                                bool is_delete_all)
{
    double op_cost = 0.0;
    if (is_delete_all) {
        op_cost = 1.0;
    } else {
        double read_block_count = sample_formula(table_rows, block_count, delete_rows);
        op_cost += read_block_count * COST_READ_ONE_BLOCK;
        op_cost += delete_rows * COST_DELETE_PER_ROW;
    }
    return op_cost;
}

double EstCostUtil::cost_distinct(double rows,
                                  const Vector<ExprStmt_s> &distinct_exprs)
{
    double op_cost = cost_hash_expr(rows, distinct_exprs);
    op_cost += rows * COST_BUILD_HASH_PER_ROW;
    op_cost += rows * COST_PROBE_PER_ROW;
    return op_cost;
}

double EstCostUtil::cost_expr_value(double rows)
{
    return rows * COST_GET_ONE_ROW;
}

double EstCostUtil::cost_hash_group_by(double rows, 
                                        double ndv, 
                                        const Vector<ExprStmt_s> &group_exprs,
                                        const Vector<ExprStmt_s> &aggr_exprs)
{
    double op_cost = cost_hash_expr(rows, group_exprs);
    op_cost += rows * COST_BUILD_HASH_PER_ROW;
    op_cost += ndv > 0 ? ndv * cost_sort(rows / ndv, group_exprs) : 0;
    op_cost += aggr_exprs.size() * rows * COST_PER_AGG_EXPR;
    return op_cost;
}   

double EstCostUtil::cost_sort(double rows, const Vector<ExprStmt_s> &sort_exprs)
{
    return cost_cmp_expr(rows * std::log(rows), sort_exprs);
}

double EstCostUtil::cost_insert(double rows)
{
    return rows * COST_INSERT_PER_ROW;
}

double EstCostUtil::cost_limit(double rows)
{
    return rows * COST_GET_ONE_ROW;
}

double EstCostUtil::cost_scalar_group(double rows, const Vector<ExprStmt_s> &aggr_exprs)
{
    double op_cost = rows * COST_PER_AGG_EXPR * aggr_exprs.size();
    return op_cost;
}

double EstCostUtil::cost_union_all(double left_rows, double right_rows)
{
    return (left_rows + right_rows) * COST_GET_ONE_ROW;
}

double EstCostUtil::cost_hash_union(double left_rows, double right_rows)
{
    double op_cost = (left_rows + right_rows) * COST_HASH_PER_EXPR;
    op_cost += (left_rows + right_rows) * COST_BUILD_HASH_PER_ROW;
    op_cost += (left_rows + right_rows) * COST_PROBE_PER_ROW;
    return op_cost;
}

double EstCostUtil::cost_hash_intersec(double left_rows, double right_rows)
{
    double op_cost = (left_rows + right_rows) * COST_HASH_PER_EXPR;
    op_cost += right_rows * COST_BUILD_HASH_PER_ROW;
    op_cost += left_rows * COST_PROBE_PER_ROW;
    return op_cost;
}

double EstCostUtil::cost_hash_except(double left_rows, double right_rows)
{
    double op_cost = (left_rows + right_rows) * COST_HASH_PER_EXPR;
    op_cost += right_rows * COST_BUILD_HASH_PER_ROW;
    op_cost += left_rows * COST_PROBE_PER_ROW;
    return op_cost;
}

double EstCostUtil::cost_update(double table_rows, 
                                double block_count, 
                                double update_rows)
{
    double op_cost = 0.0;
    double read_block_count = sample_formula(table_rows, block_count, update_rows);
    op_cost += read_block_count * COST_READ_ONE_BLOCK;
    op_cost += update_rows * COST_DELETE_PER_ROW;
    op_cost += update_rows * COST_INSERT_PER_ROW;
    return op_cost;
}
