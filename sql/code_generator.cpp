#include "code_generator.h"
#include "expr_generator.h"

#include "log_delete.h"
#include "log_distinct.h"
#include "log_dual_table.h"
#include "log_expr_value.h"
#include "log_group_by.h"
#include "log_insert.h"
#include "log_join.h"
#include "log_limit.h"
#include "log_scalar_group_by.h"
#include "log_set.h"
#include "log_sort.h"
#include "log_subquery_evaluate.h"
#include "log_table_scan.h"
#include "log_update.h"
#include "log_view.h"

#include "phy_delete.h"
#include "phy_hash_distinct.h"
#include "phy_dual_table.h"
#include "phy_expr_value.h"
#include "phy_hash_group.h"
#include "phy_insert.h"
#include "phy_hash_join.h"
#include "phy_nested_loop_join.h"
#include "phy_limit.h"
#include "phy_scalar_group.h"
#include "phy_hash_set.h"
#include "phy_sort.h"
#include "phy_table_scan.h"
#include "phy_update.h"
#include "phy_view.h"
#include "phy_expression.h"
#include "phy_filter.h"

#include "query_ctx.h"
#include "table_space.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Storage;
using namespace CatDB::Optimizer;
using namespace CatDB::Parser;
using namespace CatDB::Sql;


u32 CodeGenerator::generate_phy_plan(ExprGenerateCtx &ctx, LogicalOperator_s& log_root, PhyOperator_s &phy_root)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_root);
    if (LogicalOperator::LOG_SUBQUERY_EVALUATE == log_root->type()) {
        CHECK(generate_subquery_evaluate_op(ctx, log_root, phy_root));
    }
    MY_ASSERT(log_root);
    Vector<PhyOperator_s> child_ops;
    PhyOperator_s phy_op;
    for (u32 i = 0; i < log_root->childs.size(); ++i) {
        CHECK(generate_phy_plan(ctx, log_root->childs[i], phy_op));
        child_ops.push_back(phy_op);
    }
    ctx.child_ops = child_ops;
    CHECK(generate_access_exprs(ctx, log_root));
    switch (log_root->type())
    {
    case LogicalOperator::LOG_DELETE:
        CHECK(generate_delete_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_DISTINCT:
        CHECK(generate_distinct_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_DUAL_TABLE:
        CHECK(generate_dual_table_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_EXPR_VALUE:
        CHECK(generate_expr_value_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_GROUP_BY:
        CHECK(generate_group_by_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_INSERT:
        CHECK(generate_insert_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_JOIN:
        CHECK(generate_join_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_LIMIT:
        CHECK(generate_limit_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_SCALAR_GROUP:
        CHECK(generate_scalar_group_by_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_SET:
        CHECK(generate_set_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_SORT:
        CHECK(generate_sort_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_TABLE_SCAN:
        CHECK(generate_table_scan_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_UPDATE:
        CHECK(generate_update_op(ctx, log_root, phy_root));
        break;
    case LogicalOperator::LOG_VIEW:
        CHECK(generate_view_op(ctx, log_root, phy_root));
        break;
    default:
        ret = ERR_UNEXPECTED;
        break;
    }
    CHECK(generate_output_exprs(ctx, log_root, phy_root));
	return ret;
}

u32 CodeGenerator::generate_delete_op(ExprGenerateCtx &ctx, LogDelete_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    BasicTableStmt_s &delete_table = log_op->table_item;
    Expression_s rt_row_id;
    CHECK(ExprGenerator::generate_expr(ctx, log_op->row_id, rt_row_id));
    phy_op = PhyDelete::make_delete(ctx.child_ops[0],
                                    delete_table->database, 
                                    delete_table->table_name,
                                    rt_row_id,
                                    log_op->is_delete_all);
	return ret;
}

u32 CodeGenerator::generate_distinct_op(ExprGenerateCtx &ctx, LogDistinct_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    phy_op = PhyHashDistinct::make_hash_distinct(ctx.child_ops[0]);
	return ret;
}

u32 CodeGenerator::generate_dual_table_op(ExprGenerateCtx &ctx, LogDualTable_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 0);
    phy_op = PhyDualTable::make_dual_table();
	return ret;
}

u32 CodeGenerator::generate_expr_value_op(ExprGenerateCtx &ctx, LogExprValue_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 0);
    PhyExprValue_s expr_value = PhyExprValue::make_expr_value();
    for (u32 i = 0; i < log_op->values.size(); ++i) {
        Vector<Expression_s> rt_values;
        CHECK(ExprGenerator::generate_exprs(ctx, log_op->values[i], rt_values));
        expr_value->add_value(rt_values);
    }
    phy_op = expr_value;
	return ret;
}

u32 CodeGenerator::generate_group_by_op(ExprGenerateCtx &ctx, LogGroupBy_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    Vector<Expression_s> rt_group_exprs;
    Vector<Expression_s> rt_aggr_items;
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->group_by_exprs, rt_group_exprs));
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->agg_items, rt_aggr_items));
    MY_ASSERT(log_op->agg_items.size() == rt_aggr_items.size());
    for (u32 i = 0; i < log_op->agg_items.size(); ++i) {
        ctx.access_expr_map[log_op->agg_items[i]] = rt_aggr_items[i];
    }
    phy_op = PhyHashGroup::make_hash_group(ctx.child_ops[0], 
                                           rt_group_exprs, 
                                           rt_aggr_items);
	return ret;
}

u32 CodeGenerator::generate_insert_op(ExprGenerateCtx &ctx, LogInsert_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    phy_op = PhyInsert::make_insert(ctx.child_ops[0],
                                    log_op->table_item->database, 
                                    log_op->table_item->table_name);
	return ret;
}

u32 CodeGenerator::generate_join_op(ExprGenerateCtx &ctx, LogJoin_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    if (LogJoin::HASH_JOIN == log_op->join_algo) {
        CHECK(generate_hash_join_op(ctx, log_op, phy_op));
    } else {
        CHECK(generate_nested_loop_join_op(ctx, log_op, phy_op));
    }
	return ret;
}

u32 CodeGenerator::generate_hash_join_op(ExprGenerateCtx &ctx, LogJoin_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 2, log_op->childs.size() == 2);
    Vector<ExprStmt_s> hash_exprs;
    Vector<ExprStmt_s> prob_exprs;
    Vector<Expression_s> rt_other_join_conds;
    Vector<Expression_s> rt_hash_exprs;
    Vector<Expression_s> rt_prob_exprs;
    LogicalOperator_s &left_child = log_op->childs[0];
    LogicalOperator_s &right_child = log_op->childs[1];
    for (u32 i = 0; i < log_op->equal_join_condition.size(); ++i) {
        ExprStmt_s &expr = log_op->equal_join_condition[i];
        MY_ASSERT(ExprStmt::OperationExpr == expr->expr_type());
        OpExprStmt_s op_expr = expr;
        MY_ASSERT(op_expr->params.size() == 2);
        ExprStmt_s &left_expr = op_expr->params[0];
        ExprStmt_s &right_expr = op_expr->params[1];
        if (left_expr->table_ids.is_subset(left_child->table_ids) && 
            right_expr->table_ids.is_subset(right_child->table_ids)) {
            hash_exprs.push_back(left_expr);
            prob_exprs.push_back(right_expr);
        } else if (left_expr->table_ids.is_subset(right_child->table_ids) && 
                   right_expr->table_ids.is_subset(left_child->table_ids)) {
            hash_exprs.push_back(right_expr);
            prob_exprs.push_back(left_expr);           
        }
    }
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->other_join_condition, rt_other_join_conds));
    CHECK(ExprGenerator::generate_exprs(ctx, hash_exprs, rt_hash_exprs));
    CHECK(ExprGenerator::generate_exprs(ctx, prob_exprs, rt_prob_exprs));
    PhyHashJoin_s hash_op = PhyHashJoin::make_hash_join(ctx.child_ops[0], 
                                                      ctx.child_ops[1], 
                                                      rt_other_join_conds,
                                                      rt_hash_exprs,
                                                      rt_prob_exprs);
    hash_op->set_join_type(log_op->join_type);
    phy_op = hash_op;
	return ret;
}

u32 CodeGenerator::generate_nested_loop_join_op(ExprGenerateCtx &ctx, LogJoin_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 2);
    Vector<Expression_s> rt_join_conds;
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->equal_join_condition, rt_join_conds));
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->other_join_condition, rt_join_conds));
    PhyNestedLoopJoin_s nl_op = PhyNestedLoopJoin::make_nested_loop_join(ctx.child_ops[0], 
                                                                         ctx.child_ops[1], 
                                                                         rt_join_conds);
    nl_op->set_join_type(log_op->join_type);
    phy_op = nl_op;
	return ret;
}

u32 CodeGenerator::generate_limit_op(ExprGenerateCtx &ctx, LogLimit_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    phy_op = PhyLimit::make_limit(ctx.child_ops[0],
                                  log_op->limit_value,
                                  log_op->offset);
	return ret;
}

u32 CodeGenerator::generate_scalar_group_by_op(ExprGenerateCtx &ctx, LogScalarGroupBy_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    Vector<Expression_s> rt_aggr_items;
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->agg_items, rt_aggr_items));
    MY_ASSERT(log_op->agg_items.size() == rt_aggr_items.size());
    for (u32 i = 0; i < log_op->agg_items.size(); ++i) {
        ctx.access_expr_map[log_op->agg_items[i]] = rt_aggr_items[i];
    }
    phy_op = PhyScalarGroup::make_scalar_group(ctx.child_ops[0],
                                               rt_aggr_items);
	return ret;
}

u32 CodeGenerator::generate_set_op(ExprGenerateCtx &ctx, LogSet_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 2);
    phy_op = PhyHashSetOp::make_hash_setop(ctx.child_ops[0],
                                           ctx.child_ops[1],
                                           log_op->set_type);
	return ret;
}

u32 CodeGenerator::generate_sort_op(ExprGenerateCtx &ctx, LogSort_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    if (log_op->top_n > 0) {
        CHECK(generate_topn_sort_op(ctx, log_op, phy_op));
    } else {
        CHECK(generate_normal_sort_op(ctx, log_op, phy_op));
    }
	return ret;
}

u32 CodeGenerator::generate_normal_sort_op(ExprGenerateCtx &ctx, LogSort_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    Vector<ExprStmt_s> sort_exprs;
    Vector<Expression_s> rt_sort_exprs;
    Vector<bool> asc;
    for (u32 i = 0; i < log_op->sort_keys.size(); ++i) {
        sort_exprs.push_back(log_op->sort_keys[i]->order_expr);
        asc.push_back(log_op->sort_keys[i]->asc);
    }
    CHECK(ExprGenerator::generate_exprs(ctx, sort_exprs, rt_sort_exprs));
    phy_op = PhySort::make_sort(ctx.child_ops[0], rt_sort_exprs, asc);
	return ret;
}

u32 CodeGenerator::generate_topn_sort_op(ExprGenerateCtx &ctx, LogSort_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    Vector<ExprStmt_s> sort_exprs;
    Vector<Expression_s> rt_sort_exprs;
    Vector<bool> asc;
    for (u32 i = 0; i < log_op->sort_keys.size(); ++i) {
        sort_exprs.push_back(log_op->sort_keys[i]->order_expr);
        asc.push_back(log_op->sort_keys[i]->asc);
    }
    CHECK(ExprGenerator::generate_exprs(ctx, sort_exprs, rt_sort_exprs));
    phy_op = PhyTopNSort::make_topn_sort(ctx.child_ops[0], rt_sort_exprs, asc, log_op->top_n);
	return ret;
}

u32 CodeGenerator::generate_subquery_evaluate_op(ExprGenerateCtx &ctx, LogicalOperator_s &log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(LogicalOperator::LOG_SUBQUERY_EVALUATE == log_op->type(), 
              log_op->childs.size() > 1);
    LogSubQueryEvaluate_s subquery_evaluate_op = log_op;
    MY_ASSERT(log_op->childs.size()-1 == subquery_evaluate_op->subqueries.size());
    ctx.subplan_map.clear();
    for (u32 i = 1; i < log_op->childs.size(); ++i) {
        SubQueryStmt_s &subquery = subquery_evaluate_op->subqueries[i-1];
        ctx.subplan_map[ctx.key(subquery)] = log_op->childs[i];
        
    }
    log_op = log_op->childs[0];
	return ret;
}

u32 CodeGenerator::generate_table_scan_op(ExprGenerateCtx &ctx, LogTableScan_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 0, log_op->query_ctx);
    PhyTableScan_s scan = PhyTableScan::make_table_scan(log_op->table_item->database, 
                                                        log_op->table_item->table_name, 
                                                        log_op->query_ctx->sample_size);
	RowDesc row_desc;
    ColumnDesc col_desc;
    for (u32 i = 0; i < log_op->access_exprs.size(); ++i) {
        ExprStmt_s &expr = log_op->access_exprs[i];
        MY_ASSERT(ExprStmt::Column == expr->expr_type());
        ColumnStmt_s col = expr;
        col_desc.set_tid_cid(col->table_id, col->column_id);
        CHECK(row_desc.add_column_desc(col_desc));
    }
    scan->set_access_desc(row_desc);
    phy_op = scan;
    return ret;
}

u32 CodeGenerator::generate_update_op(ExprGenerateCtx &ctx, LogUpdate_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    Vector<Expression_s> rt_value_exprs;
    Expression_s rt_row_id;
    RowDesc row_desc;
    ColumnDesc col_desc;
    for (u32 i = 0; i < log_op->column_exprs.size(); ++i) {
        ColumnStmt_s col = log_op->column_exprs[i];
        col_desc.set_tid_cid(col->table_id, col->column_id);
        CHECK(row_desc.add_column_desc(col_desc));
    }
    CHECK(ExprGenerator::generate_exprs(ctx, log_op->value_exprs, rt_value_exprs));
    CHECK(ExprGenerator::generate_expr(ctx, log_op->row_id, rt_row_id));
    phy_op = PhyUpdate::make_update(ctx.child_ops[0], 
                                    log_op->table_item->database, 
                                    log_op->table_item->table_name, 
                                    rt_row_id, 
                                    rt_value_exprs);
	phy_op->set_row_desc(row_desc);
    return ret;
}

u32 CodeGenerator::generate_view_op(ExprGenerateCtx &ctx, LogView_s log_op, PhyOperator_s &phy_op)
{
	u32 ret = SUCCESS;
    MY_ASSERT(log_op, ctx.child_ops.size() == 1);
    phy_op = PhyView::make_view(ctx.child_ops[0]);
	return ret;
}

u32 CodeGenerator::generate_access_exprs(ExprGenerateCtx &ctx, LogicalOperator_s& log_root)
{
    u32 ret = SUCCESS;
    MY_ASSERT(log_root);
    ctx.access_expr_map.clear();
    if (!log_root->childs.empty() && !log_root->access_exprs.empty()) {
        if (LogicalOperator::LOG_VIEW == log_root->type()) {
            MY_ASSERT(log_root->childs.size() == 1);
            LogicalOperator_s &child = log_root->childs[0];
            Expression_s rt_expr;
            ColumnDesc desc;
            for (u32 i = 0; i < log_root->access_exprs.size(); ++i) {
                ExprStmt_s &expr = log_root->access_exprs[i];
                MY_ASSERT(ExprStmt::Column == expr->expr_type());
                ColumnStmt_s col_expr = expr;
                desc.set_tid_cid(child->operator_id, i);
                rt_expr = ColumnExpression::make_column_expression(desc);
                ctx.access_expr_map[expr] = rt_expr;
            }
        } else {
            Expression_s rt_expr;
            ColumnDesc desc;
            for (u32 i = 0; i < log_root->access_exprs.size(); ++i) {
                ExprStmt_s &expr = log_root->access_exprs[i];
                u32 index;
                bool find_expr = false;;
                for (u32 j = 0; !find_expr && j < log_root->childs.size(); ++j) {
                    LogicalOperator_s &child = log_root->childs[j];
                    if (ExprUtils::find_item(child->output_exprs, expr, &index)) {
                        desc.set_tid_cid(child->operator_id, index);
                        find_expr = true;
                        break;
                    }
                }
                MY_ASSERT(find_expr);
                rt_expr = ColumnExpression::make_column_expression(desc);
                ctx.access_expr_map[expr] = rt_expr;
            }
        }
    }
    return ret;
}

u32 CodeGenerator::generate_output_exprs(ExprGenerateCtx &ctx, LogicalOperator_s& log_root, PhyOperator_s &phy_root)
{
    u32 ret = SUCCESS;
    MY_ASSERT(log_root, phy_root);
    Vector<Expression_s> rt_output_exprs;
    Vector<Expression_s> rt_filters;
    CHECK(ExprGenerator::generate_exprs(ctx, log_root->output_exprs, rt_output_exprs));
    CHECK(ExprGenerator::generate_exprs(ctx, log_root->filters, rt_filters));
    phy_root->set_output_exprs(log_root->operator_id, rt_output_exprs);
    if (!rt_filters.empty()) {
        phy_root->set_filter(PhyFilter::make_filter(rt_filters));
    }
    return ret;
}
