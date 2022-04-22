#include "log_dual_table.h"
#include "log_table_scan.h"
#include "log_subquery_evaluate.h"
#include "log_view.h"
#include "dml_plan.h"
#include "select_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "expr_utils.h"
#include "join_property.def"
#include "opt_est_info.h"
#include "query_ctx.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

DMLPlan::DMLPlan()
{

}

DMLPlan::~DMLPlan()
{

}

u32 DMLPlan::build_plan()
{
    u32 ret = SUCCESS;
    CHECK(generate_plan_tree());
    MY_ASSERT(root_operator);
    CHECK(root_operator->visit_plan());
    return ret;
}

u32 DMLPlan::generate_conflict_detectors(Vector<TableStmt_s> &tables, 
                                         Vector<ExprStmt_s> &conds,
										 Vector<ConflictDetector_s> &detectors)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> join_conditions;
    Vector<ConflictDetector_s> outer_join_detectors;
    Vector<ConflictDetector_s> inner_join_detectors;
    //下推过滤条件
    for (u32 i = 0; i < conds.size(); ++i) {
        bool is_join_condition = true;
        for (u32 j = 0; j < tables.size(); ++j) {
            if (conds[i]->table_ids.is_subset(tables[j]->table_ids)) {
                is_join_condition = false;
                tables[j]->table_filter.push_back(conds[i]);
                if (!conds[i]->table_ids.is_empty()) {
                    break;
                } else {
                    //常量条件下推到所有基表
                }
            }
        }
        if (is_join_condition) {
            join_conditions.push_back(conds[i]);
        }
    }
    //为每个joined table生成detector
    for (u32 i = 0; i < tables.size(); ++i) {
        if (tables[i]->is_joined_table()) {
            CHECK(generate_outer_join_detecotrs(tables[i], outer_join_detectors));
        }
    }
    //inner join的detector
    CHECK(generate_inner_join_detectors(join_conditions, 
                                        outer_join_detectors, 
                                        inner_join_detectors));
    CHECK(generate_cross_product_detector(tables, 
                                          inner_join_detectors, 
                                          outer_join_detectors));
    append(detectors, outer_join_detectors);
    append(detectors, inner_join_detectors);
    return ret;
}

u32 DMLPlan::generate_cross_product_detector(Vector<TableStmt_s> &tables, 
											 Vector<ConflictDetector_s> &detectors,
                                             Vector<ConflictDetector_s> &outer_join_detectors)
{
    u32 ret = SUCCESS;
    if (tables.size() < 2) {
        return ret;
    }
    ConflictDetector_s detector = ConflictDetector::make_conflict_detector();
    for (u32 i = 0; i < tables.size(); ++i) {
        detector->L_DS.add_members(tables[i]->table_ids);
        detector->R_DS.add_members(tables[i]->table_ids);
        detector->cross_product_rules.push_back(tables[i]->table_ids);
    }
    detector->is_symmetric = true;
    detector->is_degenerate = true;
    detector->join_info.join_type = Inner;
    CHECK(generate_conflict_rules(detector, true, outer_join_detectors));
    CHECK(generate_conflict_rules(detector, false, outer_join_detectors));
    detectors.push_back(detector);
    return ret;
}

u32 DMLPlan::flatten_table_items(Vector<TableStmt_s> &tables, Vector<ExprStmt_s> &conds)
{
    u32 ret = SUCCESS;
    Vector<TableStmt_s> flatten_tables;
    for (u32 i = 0; i < tables.size(); ++i) {
        CHECK(flatten_table_items(tables[i], flatten_tables, conds));
    }
    tables = flatten_tables;
    return ret;
}

u32 DMLPlan::flatten_table_items(const TableStmt_s &table, 
                                Vector<TableStmt_s> &tables, 
                                Vector<ExprStmt_s> &conds)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table);
    if (table->is_joined_table()) {
        JoinedTableStmt_s joined_table = table;
        if (Inner == joined_table->join_type) {
            append(conds, joined_table->join_condition);
            append(conds, joined_table->table_filter);
            CHECK(flatten_table_items(joined_table->left_table, tables, conds));
            CHECK(flatten_table_items(joined_table->right_table, tables, conds));
        } else {
            tables.push_back(table);
        }
    } else {
        tables.push_back(table);
    }
    return ret;
}

u32 DMLPlan::generate_outer_join_detecotrs(TableStmt_s &table, 
                                           Vector<ConflictDetector_s> &detectors)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table, table->is_joined_table());
    JoinedTableStmt_s joined_table = table;
    if (Inner == joined_table->join_type) {
        Vector<ExprStmt_s> conds;
        Vector<TableStmt_s> flatten_tables;
        //抚平inner join
        CHECK(flatten_table_items(table, flatten_tables, conds));
        CHECK(generate_conflict_detectors(flatten_tables, conds, detectors));
    } else {
        Vector<ConflictDetector_s> left_detectors;
        Vector<ConflictDetector_s> right_detectors;
        ConflictDetector_s detector;
        CHECK(change_right_join_to_left(joined_table));
        CHECK(pushdown_outer_join_filter(joined_table));
        CHECK(pushdown_on_condition(joined_table));
        if (joined_table->left_table->is_joined_table()) {
            CHECK(generate_outer_join_detecotrs(joined_table->left_table, left_detectors));
        }
        if (joined_table->right_table->is_joined_table()) {
            CHECK(generate_outer_join_detecotrs(joined_table->right_table, right_detectors));
        }
        CHECK(inner_generate_outer_join_detectors(joined_table, detector));
        CHECK(generate_conflict_rules(detector, true, left_detectors));
        CHECK(generate_conflict_rules(detector, false, right_detectors));
        if (detector->is_symmetric) {
            CHECK(generate_conflict_rules(detector, false, left_detectors));
            CHECK(generate_conflict_rules(detector, true, right_detectors));
        }
        detectors.push_back(detector);
        append(detectors, left_detectors);
        append(detectors, right_detectors);
    }
    return ret;
}

u32 DMLPlan::change_right_join_to_left(JoinedTableStmt_s &joined_table)
{
    u32 ret = SUCCESS;
    if (RightOuter == joined_table->join_type || 
        RightAnti == joined_table->join_type || 
        RightSemi == joined_table->join_type) {
        TableStmt_s left = joined_table->right_table;
        joined_table->right_table = joined_table->left_table;
        joined_table->left_table = left;
        joined_table->join_type = ReverseJoinType[joined_table->join_type];
    }
    return ret;
}

u32 DMLPlan::pushdown_outer_join_filter(JoinedTableStmt_s &joined_table)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> new_filters;
    for (u32 i = 0; i < joined_table->table_filter.size(); ++i) {
        ExprStmt_s &expr = joined_table->table_filter[i];
        if (LeftOuter == joined_table->join_type ||
            LeftSemi == joined_table->join_type ||
            LeftAnti == joined_table->join_type) {
            if (expr->table_ids.is_subset(joined_table->left_table->table_ids)) {
                joined_table->left_table->table_filter.push_back(expr);
            } else {
                new_filters.push_back(expr);
            }
        } else if (RightOuter == joined_table->join_type || 
                   RightAnti == joined_table->join_type || 
                   RightSemi == joined_table->join_type) {
            if (expr->table_ids.is_subset(joined_table->right_table->table_ids)) {
                joined_table->right_table->table_filter.push_back(expr);
            } else {
                new_filters.push_back(expr);
            }         
        } else {
            new_filters.push_back(expr);
        }
    }
    joined_table->table_filter = new_filters;
    return ret;
}

u32 DMLPlan::pushdown_on_condition(JoinedTableStmt_s &joined_table)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> new_condition;
    for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
        ExprStmt_s &expr = joined_table->join_condition[i];
        if (LeftOuter == joined_table->join_type ||
            LeftSemi == joined_table->join_type ||
            LeftAnti == joined_table->join_type) {
            if (expr->table_ids.is_subset(joined_table->right_table->table_ids)) {
                joined_table->right_table->table_filter.push_back(expr);
            } else {
                new_condition.push_back(expr);
            }
        } else if (RightOuter == joined_table->join_type || 
                   RightAnti == joined_table->join_type || 
                   RightSemi == joined_table->join_type) {
            if (expr->table_ids.is_subset(joined_table->left_table->table_ids)) {
                joined_table->left_table->table_filter.push_back(expr);
            } else {
                new_condition.push_back(expr);
            }         
        } else {
            new_condition.push_back(expr);
        }
    }
    joined_table->join_condition = new_condition;
    return ret;
}

u32 DMLPlan::inner_generate_outer_join_detectors(JoinedTableStmt_s &joined_table, 
                                                 ConflictDetector_s &detector)
{
    u32 ret = SUCCESS;
    detector = ConflictDetector::make_conflict_detector();
    detector->join_info.join_type = joined_table->join_type;
    append(detector->join_info.outer_join_filter, joined_table->table_filter);
    for (u32 i = 0; i < joined_table->table_filter.size(); ++i) {
        ExprStmt_s &cond = joined_table->table_filter[i];
        detector->table_set.add_members(cond->table_ids);
    }
    for (u32 i = 0; i < joined_table->join_condition.size(); ++i) {
        ExprStmt_s &cond = joined_table->join_condition[i];
        CHECK(add_join_condition(cond, detector->join_info));
        detector->table_set.add_members(cond->table_ids);
    }
    detector->L_DS.add_members(joined_table->left_table->table_ids);
    detector->R_DS.add_members(joined_table->right_table->table_ids);
    detector->L_TES.intersect(detector->L_DS, detector->table_set);
    detector->R_TES.intersect(detector->R_DS, detector->table_set);
    if (detector->L_TES.is_empty() || detector->R_TES.is_empty()) {
        detector->is_degenerate = true;
    } else {
        detector->is_degenerate = false;
    }
    detector->is_symmetric = CommProperty[joined_table->join_type];
    return ret;
}

u32 DMLPlan::generate_inner_join_detectors(Vector<ExprStmt_s> &join_conditions, 
                                        Vector<ConflictDetector_s> &outer_join_detectors, 
                                        Vector<ConflictDetector_s> &inner_join_detector)
{
    u32 ret = SUCCESS;
    ConflictDetector_s detector;
    //连接条件分组
    for (u32 i = 0; i < join_conditions.size(); ++i) {
        bool find = false;
        for (u32 j = 0; j < inner_join_detector.size(); ++j) {
            if (inner_join_detector[j]->table_set.is_equal(join_conditions[i]->table_ids)) {
                find = true;
                detector = inner_join_detector[j];
                break;
            }
        }
        if (find && detector) {
            CHECK(add_join_condition(join_conditions[i], detector->join_info));
        } else {
            detector = ConflictDetector::make_conflict_detector();
            CHECK(add_join_condition(join_conditions[i], detector->join_info));
            detector->table_set = join_conditions[i]->table_ids;
            inner_join_detector.push_back(detector);
        }
    }
    //生成笛卡尔积冲突规则

    //生成冲突规则
    for (u32 i = 0; i < inner_join_detector.size(); ++i) {
        CHECK(inner_generate_inner_join_detectors(inner_join_detector[i]));
        CHECK(generate_conflict_rules(inner_join_detector[i], true, outer_join_detectors));
        CHECK(generate_conflict_rules(inner_join_detector[i], false, outer_join_detectors));
    }
    return ret;
}

u32 DMLPlan::inner_generate_inner_join_detectors(ConflictDetector_s &detector)
{
    u32 ret = SUCCESS;
    detector->join_info.join_type = Inner;
    detector->L_DS.add_members(detector->table_set);
    detector->R_DS.add_members(detector->table_set);
    detector->L_TES.add_members(detector->table_set);
    detector->R_TES.add_members(detector->table_set);
    detector->is_degenerate = false;
    detector->is_symmetric = CommProperty[Inner];
    return ret;
}

u32 DMLPlan::add_join_condition(const ExprStmt_s& cond, JoinInfo &info)
{
    u32 ret = SUCCESS;
    if (cond->has_flag(HAS_SUBQUERY)) {
        ret = OPERATION_NOT_SUPPORT;
    } else if (cond->has_flag(IS_OP_EXPR)) {
        const OpExprStmt_s op_expr = cond;
        if (OP_EQ == op_expr->op_type) {
            info.equal_join_condition.push_back(cond);
        } else {
            info.other_join_condition.push_back(cond);
        }
    } else {
        info.other_join_condition.push_back(cond);
    }
    return ret;
}

u32 DMLPlan::generate_conflict_rules(ConflictDetector_s &detector, 
                                    bool is_left_child, 
                                    Vector<ConflictDetector_s> &child_detectors)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < child_detectors.size(); ++i) {
        CHECK(generate_conflict_rules(detector, is_left_child, child_detectors[i]));
    }
    return ret;
}

u32 DMLPlan::generate_conflict_rules(ConflictDetector_s &detector, 
                                    bool is_left_child, 
                                    const ConflictDetector_s &child_detector)
{
    u32 ret = SUCCESS;
    if (is_left_child) {
        JoinType r_type = detector->join_info.join_type;
        JoinType l_type = child_detector->join_info.join_type;
        if (!AssocProperty[l_type][r_type]) {
            BitSet common_ids;
            common_ids.intersect(child_detector->L_DS, child_detector->table_set);
            if (common_ids.is_empty()) {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->R_DS, 
                                        child_detector->L_DS));
            } else {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->R_DS, 
                                        common_ids));
            }
        }
        if (!L_AssComProperty[l_type][r_type]) {
            BitSet common_ids;
            common_ids.intersect(child_detector->R_DS, child_detector->table_set);
            if (common_ids.is_empty()) {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->L_DS, 
                                        child_detector->R_DS));
            } else {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->L_DS, 
                                        common_ids));
            }
        }
    } else {
        JoinType l_type = detector->join_info.join_type;
        JoinType r_type = child_detector->join_info.join_type;
        if (!AssocProperty[l_type][r_type]) {
            BitSet common_ids;
            common_ids.intersect(child_detector->R_DS, child_detector->table_set);
            if (common_ids.is_empty()) {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->L_DS, 
                                        child_detector->R_DS));
            } else {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->L_DS, 
                                        common_ids));
            }
        }
        if (!R_AssComProperty[l_type][r_type]) {
            BitSet common_ids;
            common_ids.intersect(child_detector->L_DS, child_detector->table_set);
            if (common_ids.is_empty()) {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->R_DS, 
                                        child_detector->L_DS));
            } else {
                CHECK(add_conflict_rule(detector->conflict_rules, 
                                        child_detector->R_DS, 
                                        common_ids));
            }
        }
    }
    return ret;
}

u32 DMLPlan::add_conflict_rule(Vector<ConflictRule> &conflict_rules, 
                            const BitSet& condition, 
                            const BitSet &constraint)
{
    u32 ret = SUCCESS;
    bool find_condition = false;
    bool find_constraint = false;
    for (u32 i = 0; i < conflict_rules.size(); ++i) {
        ConflictRule &rule = conflict_rules[i];
        if (rule.condition.is_equal(condition)) {
            find_condition = true;
            rule.constraint.add_members(constraint);
            break;
        } else if (rule.constraint.is_equal(constraint)) {
            find_constraint = true;
            rule.condition.add_members(condition);
            break;
        }
    }
    if (!find_condition && !find_constraint) {
        ConflictRule rule;
        rule.condition = condition;
        rule.constraint = constraint;
        conflict_rules.push_back(rule);
    }
    return ret;
}

u32 DMLPlan::generate_join_operator(LogicalOperator_s &left,
                                    LogicalOperator_s &right,
                                    JoinInfo &join_info,
                                    JoinAlgo join_algo,
                                    LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    LogJoin_s join = LogJoin::make_join(left, right, join_info.join_type, join_algo);
    join->init(query_ctx, est_info);
    join->equal_join_condition = join_info.equal_join_condition;
    join->other_join_condition = join_info.other_join_condition;
    op = join;
    op->add_table_ids(left->get_tables_ids());
    op->add_table_ids(right->get_tables_ids());
    CHECK(add_filter(op, 
                     join_info.outer_join_filter,
                     true));
    CHECK(op->compute_property());
    return ret;
}

u32 DMLPlan::generate_join_order_with_joined_table(JoinedTableStmt_s table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    LogicalOperator_s left_op, right_op;
    JoinInfo join_info;
    join_info.join_type = table_stmt->join_type;
    join_info.other_join_condition = table_stmt->join_condition;
    CHECK(generate_join_order_with_table_item(table_stmt->left_table, left_op));
    CHECK(generate_join_order_with_table_item(table_stmt->right_table, right_op));
    CHECK(generate_join_operator(left_op, 
                                 right_op,
                                 join_info,
                                 NL_JOIN,
                                 op));
    return ret;
}

u32 DMLPlan::generate_join_order_with_basic_table(BasicTableStmt_s table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    if (table_stmt->is_dual) {
        op = LogDualTable::make_dual_table(table_stmt);
    } else {
        op = LogTableScan::make_table_scan(table_stmt);
        CHECK(set_table_access_columns(op));
    }
    op->init(query_ctx, est_info);
    CHECK(add_filter(op, table_stmt->table_filter));
    CHECK(op->compute_property());
    return ret;
}

u32 DMLPlan::generate_join_order_with_view_table(ViewTableStmt_s table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    CHECK(generate_sub_select_plan_tree(table_stmt->ref_query, op));
    op = LogView::make_view(table_stmt, op);
    op->init(query_ctx, est_info);
    CHECK(set_table_access_columns(op));
    CHECK(add_filter(op, table_stmt->table_filter));
    CHECK(op->compute_property());
    return ret;
}

u32 DMLPlan::generate_join_order_with_table_item(TableStmt_s& table_stmt, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    if (table_stmt->is_basic_table()) {
        CHECK(generate_join_order_with_basic_table(table_stmt, op));
    } else if (table_stmt->is_joined_table()) {
        CHECK(generate_join_order_with_joined_table(table_stmt, op));
    } else if (table_stmt->is_view_table()) {
        CHECK(generate_join_order_with_view_table(table_stmt, op));
    } else {
        LOG_ERR("unknow table type", K(table_stmt));
        ret = ERROR_LEX_STMT;
    }
    if (SUCC(ret)) {
        op->add_table_ids(table_stmt->table_ids);
    }
    return ret;
}

u32 DMLPlan::generate_join_order()
{
    u32 ret = SUCCESS;
    MY_ASSERT(lex_stmt, lex_stmt->stmt_type() !=  DoCMD)
    DMLStmt_s stmt = lex_stmt;
    Vector<TableStmt_s> base_tables;
    Vector<ExprStmt_s> subquery_exprs;
    Vector<ExprStmt_s> none_subquery_exprs;
    CHECK(ExprUtils::extract_subquery_exprs(stmt->where_stmt, 
                                            subquery_exprs, 
                                            none_subquery_exprs));
    CHECK(generate_conflict_detectors(stmt->from_stmts, 
                                      none_subquery_exprs, 
                                      conflict_detectors));
    LOG_TRACE("succeed to generate conflict detectors", 
                            K(stmt->from_stmts), 
                            K(none_subquery_exprs),
                            K(conflict_detectors));
    CHECK(stmt->get_table_items(base_tables));
    CHECK(generate_base_plan(base_tables));
    CHECK(init_leading_info());
    if (query_ctx->query_hint.has_leading_hint(stmt->get_qb_name())) {
        CHECK(generate_join_order_with_DP(false));
    }
    if (join_orders.size() != base_tables.size() || 
        join_orders[base_tables.size() - 1].size() != 1) {
        CHECK(generate_join_order_with_DP(true));
    }
    MY_ASSERT(join_orders.size() == base_tables.size());
    MY_ASSERT(join_orders[base_tables.size() - 1].size() == 1);
    root_operator = join_orders[base_tables.size() - 1][0];
    CHECK(add_filter(root_operator, subquery_exprs));
    MY_ASSERT(root_operator);
    CHECK(generate_plan_hint());
    LOG_TRACE("succeed to generate join order");
    return ret;
}

u32 DMLPlan::generate_base_plan(Vector<TableStmt_s> &base_tables)
{
    u32 ret = SUCCESS;
    Vector<LogicalOperator_s> base_plans;
    LogicalOperator_s op;
    est_info = EstInfo::make_est_info();
    for (u32 i = 0; i < base_tables.size(); ++i) {
        CHECK(generate_join_order_with_table_item(base_tables[i], op));
        base_plans.push_back(op);
    }
    join_orders.clear();
    join_orders.push_back(base_plans);
    u32 join_levels = base_plans.size();
    Vector<LogicalOperator_s> join_plans;
    for (u32 level = 1; level < join_levels; ++level) {
        join_orders.push_back(join_plans);
    }
    return ret;
}

u32 DMLPlan::generate_join_order_with_DP(bool ignore_hint)
{
    u32 ret = SUCCESS;
    MY_ASSERT(join_orders.size() > 0);
    u32 join_levels = join_orders[0].size();
    for (u32 level = 1; level < join_levels; ++level) {
        LOG_TRACE("begin to generate join order", K(level));
        for (u32 left_level = 0; left_level < level; ++left_level) {
            u32 right_level = level - 1 - left_level;
            CHECK(generate_join_order_with_DP(ignore_hint, left_level, right_level));
        }
        LOG_TRACE("succeed to generate join order", K(level), K(join_orders[level].size()));
    }
    return ret;
}

u32 DMLPlan::generate_join_order_with_DP(bool ignore_hint, u32 left_level, u32 right_level)
{
    u32 ret = SUCCESS;
    LogicalOperator_s join_op;
    MY_ASSERT(left_level < join_orders.size(), right_level < join_orders.size());
    for (u32 i = 0; i < join_orders[left_level].size(); ++i) {
        for (u32 j = 0; j < join_orders[right_level].size(); ++j) {
            CHECK(query_ctx->check_query_status());
            JoinInfo join_info;
            Vector<ConflictDetector_s> detectors;
            bool is_legal = false;
            LogicalOperator_s &left_tree = join_orders[left_level][i];
            LogicalOperator_s &right_tree = join_orders[right_level][j];
            if (left_tree->table_ids.overlap(right_tree->table_ids)) {
                continue;
            } else if (!ignore_hint) {
                CHECK(is_leading_legal(left_tree->table_ids, right_tree->table_ids, is_legal));
                if (!is_legal) {
                    LOG_TRACE("join order conflict with leading hint", K(left_tree->table_ids), 
                                K(right_tree->table_ids), K(join_info));
                    continue;
                }
            }
            CHECK(choose_join_info(left_tree, right_tree, detectors, is_legal));
            if (is_legal) {
                CHECK(merge_join_infos(left_tree->table_ids, 
                                        right_tree->table_ids, 
                                        detectors, 
                                        join_info));
            } else {
                CHECK(choose_join_info(right_tree, left_tree, detectors, is_legal));
                if (is_legal) {
                    CHECK(merge_join_infos(left_tree->table_ids, 
                                            right_tree->table_ids, 
                                            detectors, 
                                            join_info));
                    join_info.join_type = ReverseJoinType[join_info.join_type];
                } else {
                    LOG_TRACE("join order is not legal", K(left_tree->table_ids), 
                                K(right_tree->table_ids), K(join_info));
                    continue;
                }
            }
            ret = (generate_join_plan(left_tree, right_tree, join_info, join_op));
            if (OPERATION_NOT_SUPPORT == ret) {
                ret = SUCCESS;
                continue;
            } else if (SUCC(ret)) {
                append(join_op->get_used_conflict_detectors(), left_tree->get_used_conflict_detectors());
                append(join_op->get_used_conflict_detectors(), right_tree->get_used_conflict_detectors());
                append(join_op->get_used_conflict_detectors(), detectors);
                CHECK(add_join_order(join_op, left_level + right_level + 1));
                LOG_TRACE("generate join plan for", K(left_tree->table_ids), K(right_tree->table_ids), K(join_info));
            }
        }
    }
    return ret;
}

u32 DMLPlan::is_leading_legal(const BitSet &left_tables, 
                            const BitSet &right_tables, 
                            bool &is_legal)
{
    u32 ret = SUCCESS;
    is_legal = false;
    if (left_tables.is_subset(leading_info.table_ids) && 
        right_tables.is_subset(leading_info.table_ids)) {
        for (u32 i = 0; !is_legal && i < leading_info.table_pairs.size(); ++i) {
            if (leading_info.table_pairs[i].left_ids.is_equal(left_tables) &&
                leading_info.table_pairs[i].right_ids.is_equal(right_tables)) {
                is_legal = true;
            }
        }
    } else if (leading_info.table_ids.is_subset(left_tables)) {
        is_legal = true;
    }
    return ret;
}

u32 DMLPlan::choose_join_info(LogicalOperator_s &left_tree, 
							  LogicalOperator_s &right_tree, 
                              Vector<ConflictDetector_s> &detectors,
                              bool &is_legal)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < conflict_detectors.size(); ++i) {
        bool is_legal = false;
        if (!conflict_detectors[i]->cross_product_rules.empty()) {
            //共享的笛卡尔积冲突检测器，忽略检测
        } else if (ExprUtils::find_item(left_tree->get_used_conflict_detectors(), conflict_detectors[i])) {
            continue;
        } else if (ExprUtils::find_item(right_tree->get_used_conflict_detectors(), conflict_detectors[i])) {
            continue;
        }
        CHECK(is_join_legal(left_tree->table_ids, 
                            right_tree->table_ids, 
                            conflict_detectors[i], 
                            is_legal));
        if (is_legal) {
            detectors.push_back(conflict_detectors[i]);
        }
    }
    if (detectors.empty()) {
        is_legal = false;
    } else {
        is_legal = true;
    }
    return ret;
}

u32 DMLPlan::is_join_legal(const BitSet &left_tables, 
                            const BitSet &right_tables, 
                            const ConflictDetector_s &detector, 
                            bool &is_legal)
{
    u32 ret = SUCCESS;
    is_legal = false;
    BitSet table_set;
    table_set.add_members(left_tables);
    table_set.add_members(right_tables);
    if (detector->is_degenerate) {
        if (left_tables.overlap(detector->L_DS) && 
            right_tables.overlap(detector->R_DS) &&
            table_set.is_superset(detector->table_set)) {
            is_legal = true;
        } else if (detector->is_symmetric &&
                   left_tables.overlap(detector->R_DS) && 
                   right_tables.overlap(detector->L_DS) &&
                   table_set.is_superset(detector->table_set)) {
            is_legal = true;
        }
    } else if (detector->is_symmetric) {
        if (table_set.is_superset(detector->L_TES) && 
            table_set.is_superset(detector->R_TES)) {
            is_legal = true;
        }
    } else {
        if (left_tables.is_superset(detector->L_TES) && 
            right_tables.is_superset(detector->R_TES)) {
            is_legal = true;
        }
    }
    if (!is_legal) {
        return ret;
    }
    for (u32 i = 0; is_legal && i < detector->conflict_rules.size(); ++i) {
        const ConflictRule& rule = detector->conflict_rules[i];
        if (table_set.overlap(rule.condition)) {
            is_legal = rule.constraint.is_subset(table_set);
        }
    }
    if (!is_legal) {
        return ret;
    }
    for (u32 i = 0; is_legal && i < detector->cross_product_rules.size(); ++i) {
        if (left_tables.overlap(detector->cross_product_rules[i])) {
            is_legal = left_tables.is_superset(detector->cross_product_rules[i]);
        }
        if (is_legal && right_tables.overlap(detector->cross_product_rules[i])) {
            is_legal = right_tables.is_superset(detector->cross_product_rules[i]);
        }
    }
    return ret;
}

u32 DMLPlan::merge_join_infos(const BitSet &left_tables, 
							  const BitSet &right_tables, 
                              Vector<ConflictDetector_s> &join_infos, 
                              JoinInfo &join_info)
{
    u32 ret = SUCCESS;
    join_info.join_type = Inner;
    for (u32 i = 0; i < join_infos.size(); ++i) {
        JoinInfo &info = join_infos[i]->join_info;
        if (Inner == info.join_type) {
            append(join_info.outer_join_filter, info.equal_join_condition);
            append(join_info.outer_join_filter, info.other_join_condition);
            append(join_info.outer_join_filter, info.outer_join_filter);
        } else if (Inner == join_info.join_type) {
            append(join_info.equal_join_condition, info.equal_join_condition);
            append(join_info.other_join_condition, info.other_join_condition);
            append(join_info.outer_join_filter, info.outer_join_filter);
            join_info.join_type = info.join_type;
        } else {
            ret = ERR_UNEXPECTED;
            LOG_ERR("unexpect join infos", K(left_tables), K(right_tables), K(join_infos), K(ret));
        }
    }
    Vector<ExprStmt_s> join_conds;
    if (Inner == join_info.join_type) {
        append(join_conds, join_info.outer_join_filter);
        join_info.outer_join_filter.clear();
    } else {
        append(join_conds, join_info.equal_join_condition);
        append(join_conds, join_info.other_join_condition);
        join_info.equal_join_condition.clear();
        join_info.other_join_condition.clear();
    }
    for (u32 i = 0; i < join_conds.size(); ++i) {
        ExprStmt_s &cond = join_conds[i];
        bool is_eq_join_cond = false;
        if (cond->has_flag(IS_OP_EXPR)) {
            const OpExprStmt_s op_expr = cond;
            if (OP_EQ == op_expr->op_type &&
                2 == op_expr->params.size()) {
                const ExprStmt_s &l_expr = op_expr->params[0];
                const ExprStmt_s &r_expr = op_expr->params[1];
                if (l_expr->table_ids.is_subset(left_tables) &&
                    !l_expr->table_ids.is_empty() &&
                    r_expr->table_ids.is_subset(right_tables) &&
                    !r_expr->table_ids.is_empty()) {
                    is_eq_join_cond = true;
                } else if (r_expr->table_ids.is_subset(left_tables) &&
                            !r_expr->table_ids.is_empty() &&
                            l_expr->table_ids.is_subset(right_tables) &&
                            !l_expr->table_ids.is_empty()) {
                    is_eq_join_cond = true;
                }
            }
        }
        if (is_eq_join_cond) {
            join_info.equal_join_condition.push_back(cond);
        } else {
            join_info.other_join_condition.push_back(cond);
        }
    }
    return ret;
}

u32 DMLPlan::generate_join_plan(LogicalOperator_s &left_tree, 
                                LogicalOperator_s &right_tree, 
                                JoinInfo &join_info,
                                LogicalOperator_s &join_plan)
{
    u32 ret = SUCCESS;
    JoinAlgo algo;
    ret = get_join_method(left_tree->table_ids, 
                          right_tree->table_ids, 
                          join_info,
                          algo);
    if (FAIL(ret)) {
        return ret;
    }
    CHECK(generate_join_operator(left_tree, 
                                right_tree, 
                                join_info, 
                                algo, 
                                join_plan));
    return ret;
}

u32 DMLPlan::get_join_method(const BitSet &left_tables,
                            const BitSet &right_tables,
                            JoinInfo &join_info,
                            JoinAlgo &algo)
{
    #define USE_ALGO(method, algo)  method |= 1 << (algo)
    #define SET_ALGO(method, algo)  method = 1 << (algo)
    #define NO_USE_ALGO(method, algo) method &= ~(1 << (algo))
    #define HAS_ALGO(method, algo)  method & (1 << (algo))
    u32 ret = SUCCESS;
    Vector<JoinHintStmt_s> join_hints;
    DMLStmt_s stmt = lex_stmt;
    query_ctx->query_hint.get_join_hints(stmt->get_qb_name(), join_hints);
    u32 method = 0;
    USE_ALGO(method, NL_JOIN);
    USE_ALGO(method, HASH_JOIN);
    if (join_info.equal_join_condition.empty()) {
        NO_USE_ALGO(method, HASH_JOIN);
    }
    if (RightOuter == join_info.join_type ||
        FullOuter == join_info.join_type ||
        RightSemi == join_info.join_type ||
        RightAnti == join_info.join_type) {
        NO_USE_ALGO(method, NL_JOIN);
    }
    u32 hint_method = method;
    for (u32 i = 0; i < join_hints.size(); ++i) {
        JoinHintStmt_s &hint = join_hints[i];
        if (right_tables.is_equal(hint->table_ids)) {
            if (hint->is_enable()) {
                if (HAS_ALGO(hint_method, hint->join_algo)) {
                    SET_ALGO(hint_method, hint->join_algo);
                    hint->set_used(true);
                }
            } else {
                if (HAS_ALGO(hint_method, hint->join_algo)) {
                    NO_USE_ALGO(hint_method, hint->join_algo);
                    hint->set_used(true);
                }
            }
        }
    }
    if (0 != hint_method) {
        //use hint algo
        method = hint_method;
    } else {
        //ignore hint
        for (u32 i = 0; i < join_hints.size(); ++i) {
            JoinHintStmt_s &hint = join_hints[i];
            if (right_tables.is_equal(hint->table_ids)) {
                hint->set_used(false);
            }
        }
    }
    if (HAS_ALGO(method, HASH_JOIN)) {
        //use hash join first
        algo = HASH_JOIN;
    } else if (HAS_ALGO(method, NL_JOIN)) {
        algo = NL_JOIN;
    } else {
        ret = OPERATION_NOT_SUPPORT;
        LOG_TRACE("join type not support", K(join_info));
    }
    return ret;
}

u32 DMLPlan::add_join_order(LogicalOperator_s& join_order, u32 level)
{
    u32 ret = SUCCESS;
    MY_ASSERT(join_orders.size() > level);
    Vector<LogicalOperator_s> &join_plans = join_orders[level];
    bool find = false;
    for (u32 i = 0; !find && i < join_plans.size(); ++i) {
        if (join_plans[i]->get_tables_ids().is_equal(join_order->get_tables_ids())) {
            if (join_plans[i]->get_cost() > join_order->get_cost()) {
                join_plans[i] = join_order;
            }
            find = true;
        }
    }
    if (!find) {
        join_plans.push_back(join_order);
    }
    return ret;
}

u32 DMLPlan::generate_sub_select_plan_tree(SelectStmt_s &sub_select, LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
	Plan_s sub_plan = Plan::make_plan(sub_select, query_ctx);
	CHECK(sub_plan->generate_plan_tree());
    op = sub_plan->get_root_operator();
	return ret;
}

u32 DMLPlan::set_table_access_columns(LogicalOperator_s &op)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> columns;
    DMLStmt_s stmt = lex_stmt;
    u32 table_id = INVALID_ID;
    MY_ASSERT(stmt, op, LOG_TABLE_SCAN == op->type() || LOG_VIEW == op->type());
    if (LOG_TABLE_SCAN == op->type()) {
        LogTableScan_s table_scan = op;
        table_id = table_scan->table_item->table_id;
    } else if (LOG_VIEW == op->type()) {
        LogView_s view = op;
        table_id = view->table_item->table_id;
    }
    stmt->get_column_exprs(table_id, columns);
    append(op->access_exprs, columns);
    return ret;
}

u32 DMLPlan::add_filter(LogicalOperator_s &log_op, 
                        Vector<ExprStmt_s> &exprs,
                        bool is_join_condition)
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> subquery_exprs;
    Vector<ExprStmt_s> none_subquery_exprs;
    CHECK(ExprUtils::extract_subquery_exprs(exprs, 
                                            subquery_exprs, 
                                            none_subquery_exprs));
    append(log_op->filters, none_subquery_exprs);
    CHECK(log_op->compute_property());
    CHECK(generate_subquery_evaluate(log_op, 
                                     subquery_exprs, 
                                     true, 
                                     true,
                                     !is_join_condition));
    return ret;
}

u32 DMLPlan::generate_subquery_evaluate(LogicalOperator_s &log_op, 
                                        Vector<ExprStmt_s> &exprs, 
                                        bool is_filter,
                                        bool ignore_aggr,
                                        bool ignore_used_expr)
{
    u32 ret = SUCCESS;
    Vector<SubQueryStmt_s> subquery_exprs;
    Vector<ExprStmt_s> copy_exprs = exprs;
    if (ignore_used_expr) {
        CHECK(ExprUtils::remote_items(copy_exprs, used_subquery_exprs));
        append(used_subquery_exprs, copy_exprs);
    }
    CHECK(ExprUtils::extract_subquery_exprs(copy_exprs, 
                                            subquery_exprs, 
                                            ignore_aggr));
    if (subquery_exprs.empty()) {
        return ret;
    }
    MY_ASSERT(log_op);
    LogSubQueryEvaluate_s subquery_evaluate;
    if (LOG_SUBQUERY_EVALUATE == log_op->type()) {
        subquery_evaluate = log_op;
    } else {
        subquery_evaluate = LogSubQueryEvaluate::make_subquery_evaluate(log_op);
        subquery_evaluate->init(query_ctx, est_info);
        log_op = subquery_evaluate;
    }
    CHECK(generate_subquery_evaluate(subquery_exprs, subquery_evaluate));
    if (is_filter) {
        append(subquery_evaluate->filters, exprs);
    }
    return ret;
}

u32 DMLPlan::generate_subquery_evaluate(Vector<SubQueryStmt_s> &subquery_exprs, 
                                        LogSubQueryEvaluate_s &log_op)
{
    u32 ret = SUCCESS;
    if (subquery_exprs.empty()) {
        return ret;
    }
    MY_ASSERT(log_op);
    for (u32 i = 0; i < subquery_exprs.size(); ++i) {
        SubQueryStmt_s &subquery = subquery_exprs[i];
        subquery->set_subquery_id(log_op->get_next_subquery_id());
        LogicalOperator_s subplan;
        CHECK(generate_sub_select_plan_tree(subquery->query_stmt, subplan));
        log_op->add_subplan(subquery, subplan);
    }
    CHECK(log_op->compute_property());
    return ret;
}

u32 DMLPlan::init_leading_info()
{
    u32 ret = SUCCESS;
    DMLStmt_s stmt = lex_stmt;
    if (!query_ctx->query_hint.has_leading_hint(stmt->get_qb_name())) {
        return ret;
    }
    LeadingHintStmt_s leading_hint = query_ctx->query_hint.get_leading_hint(stmt->get_qb_name());
    MY_ASSERT(leading_hint->tables);
    CHECK(get_leading_info(leading_hint->tables, leading_info.table_ids));
    return ret;
}

u32 DMLPlan::get_leading_info(const LeadingTable_s &leading_table, BitSet &table_ids)
{
    u32 ret = SUCCESS;
    if (leading_table->is_base_table) {
        table_ids.add_member(leading_table->table_id);
    } else if (leading_table->table_list.empty()) {
        //do nothing
    } else if (1 == leading_table->table_list.size()) {
        CHECK(get_leading_info(leading_table->table_list[0], table_ids));
    } else {
        BitSet left_ids, right_ids;
        CHECK(get_leading_info(leading_table->table_list[0], left_ids));
        for (u32 i = 1; i < leading_table->table_list.size(); ++i) {
            CHECK(get_leading_info(leading_table->table_list[i], right_ids));
            LeadingInfo::TablePair pair;
            pair.left_ids.add_members(left_ids);
            pair.right_ids.add_members(right_ids);
            leading_info.table_pairs.push_back(pair);
            left_ids.add_members(right_ids);
            right_ids.clear();
        }
        table_ids.add_members(left_ids);
    }
    return ret;
}

u32 DMLPlan::generate_plan_hint()
{
    u32 ret = SUCCESS;
    DMLStmt_s stmt = lex_stmt;
    Vector<String> table_names;
    LeadingTable_s table;
    CHECK(generate_plan_hint(root_operator, table_names, table));
    MY_ASSERT(table);
    if (!table->is_base_table) {
        CHECK(query_ctx->query_hint.generate_leading_outline(stmt->get_qb_name(), table));
    }
    return ret;
}

u32 DMLPlan::generate_plan_hint(LogicalOperator_s &op, 
                                Vector<String> &table_names, 
                                LeadingTable_s &table)
{
    u32 ret = SUCCESS;
    if (LOG_TABLE_SCAN == op->type() ||
        LOG_DUAL_TABLE == op->type() ||
        LOG_VIEW == op->type()) {
        TableStmt_s table_item;
        if (LOG_TABLE_SCAN == op->type()) {
            LogTableScan_s scan = op;
            table_item = scan->table_item;
        } else if (LOG_DUAL_TABLE == op->type()) {
            LogDualTable_s dual = op;
            table_item = dual->table_item;
        } else if (LOG_VIEW == op->type()) {
            LogView_s view = op;
            table_item = view->table_item;
        }
        MY_ASSERT(table_item);
        table_names.push_back(table_item->alias_name);
        table = LeadingTable::make_leading_table();
        table->is_base_table = true;
        table->table_name = table_item->alias_name;
    } else if (LOG_JOIN == op->type()) {
        LogJoin_s join_op = op;
        Vector<String> left_table_names;
        Vector<String> right_table_names;
        LeadingTable_s left_table;
        LeadingTable_s right_table;
        CHECK(generate_plan_hint(join_op->left_child(), left_table_names, left_table));
        CHECK(generate_plan_hint(join_op->right_child(), right_table_names, right_table));
        append(table_names, left_table_names);
        append(table_names, right_table_names);
        MY_ASSERT(left_table);
        MY_ASSERT(right_table);
        table = LeadingTable::make_leading_table();
        table->is_base_table = false;
        table->table_list.push_back(left_table);
        table->table_list.push_back(right_table);

        //generate join hint
        DMLStmt_s stmt = lex_stmt;
        CHECK(query_ctx->query_hint.generate_join_outline(stmt->get_qb_name(),
                                                          right_table_names,
                                                          join_op->join_algo));
    } else if (LOG_SUBQUERY_EVALUATE == op->type()) {
        CHECK(generate_plan_hint(op->childs[0], table_names, table));
    } else {
        ret = ERR_UNEXPECTED;
    }
    return ret;
}