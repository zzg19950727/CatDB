#include "select_plan.h"
#include "select_stmt.h"
#include "log_group_by.h"
#include "log_scalar_group_by.h"
#include "log_window_func.h"
#include "log_distinct.h"
#include "log_sort.h"
#include "log_limit.h"
#include "log_set.h"
#include "error.h"
#include "log.h"


using namespace CatDB::Optimizer;
using namespace CatDB::Common;
using namespace CatDB::Parser;

SelectPlan::SelectPlan()
	:DMLPlan()
{

}

SelectPlan::~SelectPlan()
{

}

Plan_s SelectPlan::make_select_plan(const Stmt_s& lex_select_stmt)
{
	SelectPlan* plan = new SelectPlan;
	plan->set_lex_stmt(lex_select_stmt);
	return Plan_s(plan);
}

Plan::PlanType SelectPlan::type() const
{
	return Plan::SELECT_PLAN;
}

u32 SelectPlan::generate_plan_tree()
{
	u32 ret = SUCCESS;
	if (SetOperation == lex_stmt->stmt_type()) {
		CHECK(generate_set_plan_tree());
	} else if (Select == lex_stmt->stmt_type()) {
		CHECK(generate_normal_plan_tree());
	} else {
		ret = ERR_UNEXPECTED;
		LOG_ERR("unexpect type stmt", K(lex_stmt), K(ret));
	}
	return ret;
}

u32 SelectPlan::generate_set_plan_tree()
{
	u32 ret = SUCCESS;
	SetStmt_s stmt = lex_stmt;
	LogicalOperator_s left_op, right_op;
	MY_ASSERT(stmt->left_query->select_expr_list.size() == stmt->right_query->select_expr_list.size());
	CHECK(generate_sub_select_plan_tree(stmt->left_query, left_op));
	CHECK(generate_sub_select_plan_tree(stmt->right_query, right_op));
	root_operator = LogSet::make_set_op(left_op,
										right_op,
										stmt->set_op);
	root_operator->init(est_info);
	CHECK(root_operator->compute_property());
	append(root_operator->output_exprs, stmt->select_expr_list);
	return ret;
}

u32 SelectPlan::generate_normal_plan_tree()
{
	u32 ret = SUCCESS;
	SelectStmt_s stmt = lex_stmt;
	bool need_limit = true;
	CHECK(generate_join_order());
	if (stmt->has_group_by()) {
		if (stmt->is_scalar_group_by()) {
			CHECK(generate_scalar_group_by());
		} else {
			CHECK(generate_group_by());
		}
	}
	if (stmt->has_window_func()) {
		CHECK(generate_window_function());
	}
	if (stmt->is_distinct) {
		CHECK(generate_distinct());
	}
	if (stmt->order_exprs.size() > 0) {
		CHECK(generate_order_by(need_limit));
	}
	if (stmt->limit_stmt && need_limit) {
		CHECK(generate_limit());
	}
	CHECK(generate_subquery_evaluate(root_operator,
									 stmt->select_expr_list, 
									 false));
	append(root_operator->output_exprs, stmt->select_expr_list);
	return ret;
}

u32 SelectPlan::generate_group_by()
{
	u32 ret = SUCCESS;
	SelectStmt_s stmt = lex_stmt;
	CHECK(generate_subquery_evaluate(root_operator,
									 stmt->group_exprs, 
									 false));
	CHECK(generate_subquery_evaluate(root_operator,
									 stmt->get_aggr_exprs(), 
									 false, 
									 false));
	root_operator = LogGroupBy::make_group_by(root_operator, stmt->group_exprs, stmt->get_aggr_exprs());
	root_operator->init(est_info);
	CHECK(add_filter(root_operator, stmt->having_stmt));
	CHECK(root_operator->compute_property());
	return ret;
}

u32 SelectPlan::generate_scalar_group_by()
{
	u32 ret = SUCCESS;
	SelectStmt_s stmt = lex_stmt;
	CHECK(generate_subquery_evaluate(root_operator,
									 stmt->get_aggr_exprs(), 
									 false, 
									 false));
	root_operator = LogScalarGroupBy::make_scalar_group_by(root_operator, stmt->get_aggr_exprs());
	root_operator->init(est_info);
	CHECK(add_filter(root_operator, stmt->having_stmt));
	CHECK(root_operator->compute_property());
	return ret;
}

u32 SelectPlan::generate_window_function()
{
	u32 ret = SUCCESS;
	SelectStmt_s stmt = lex_stmt;
	Vector<ExprStmt_s> &win_func_exprs = stmt->get_win_func_exprs();
	CHECK(generate_subquery_evaluate(root_operator,
									 win_func_exprs, 
									 false, 
									 false));
	for (u32 i = 0; i < win_func_exprs.size(); ++i) {
		CHECK(generate_one_window_function(win_func_exprs[i]));
	}
	return ret;
}

u32 SelectPlan::generate_one_window_function(WinExprStmt_s win_expr)
{
	u32 ret = SUCCESS;
	Vector<ExprStmt_s> part_by_exprs;
	Vector<ExprStmt_s> order_by_exprs;
	win_expr->get_win_part_by_exprs(part_by_exprs);
	win_expr->get_win_order_by_exprs(order_by_exprs);
	if (!part_by_exprs.empty()) {
		root_operator = LogSort::make_sort(root_operator, order_by_exprs, 0);
		root_operator->init(est_info);
		LogSort_s log_sort = root_operator;
		log_sort->set_partition_keys(part_by_exprs);
		CHECK(root_operator->compute_property());
	} else if (!order_by_exprs.empty()) {
		root_operator = LogSort::make_sort(root_operator, order_by_exprs, 0);
		root_operator->init(est_info);
		CHECK(root_operator->compute_property());
	}
	Vector<ExprStmt_s> win_func_exprs;
	win_func_exprs.push_back(win_expr);
	root_operator = LogWindowFunc::make_window_func(root_operator, 
													win_func_exprs,
													part_by_exprs);
	root_operator->init(est_info);
	CHECK(root_operator->compute_property());
	return ret;
}

u32 SelectPlan::generate_distinct()
{
	u32 ret = SUCCESS;
	SelectStmt_s stmt = lex_stmt;
	CHECK(generate_subquery_evaluate(root_operator,
									 stmt->select_expr_list, 
									 false));
	root_operator = LogDistinct::make_distinct(root_operator);
	root_operator->init(est_info);
	LogDistinct_s distinct_op = root_operator;
	distinct_op->set_distinct_exprs(stmt->select_expr_list);
	CHECK(root_operator->compute_property());
	return ret;
}

u32 SelectPlan::generate_order_by(bool &need_limit)
{
	u32 ret = SUCCESS;
	need_limit = true;
	u32 top_n = 0;
	bool need_sort = true;
	SelectStmt_s stmt = lex_stmt;
	if (stmt->limit_stmt) {
		top_n = stmt->limit_stmt->limit_offset + stmt->limit_stmt->limit_size;
		if (stmt->limit_stmt->limit_size == 0) {
			need_sort = false;
		}
		if (stmt->limit_stmt->limit_offset == 0 && stmt->limit_stmt->limit_size != 0) {
			need_limit = false;
		}
	}
	if (need_sort) {
		CHECK(generate_subquery_evaluate(root_operator,
										 stmt->order_exprs, 
										 false));
		root_operator = LogSort::make_sort(root_operator, stmt->order_exprs, top_n);
		root_operator->init(est_info);
		CHECK(root_operator->compute_property());
	}
	return ret;
}

u32 SelectPlan::generate_limit()
{
	u32 ret = SUCCESS;
	SelectStmt_s stmt = lex_stmt;
	root_operator = LogLimit::make_limit(root_operator,
										 stmt->limit_stmt->limit_offset,
										 stmt->limit_stmt->limit_size);
	root_operator->init(est_info);
	CHECK(root_operator->compute_property());
	return ret;
}
