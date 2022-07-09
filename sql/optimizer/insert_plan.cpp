#include "insert_plan.h"
#include "insert_stmt.h"
#include "expr_stmt.h"
#include "log_expr_value.h"
#include "log_insert.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

InsertPlan::InsertPlan()
{

}

InsertPlan::~InsertPlan()
{

}

Plan_s InsertPlan::make_insert_plan(const Stmt_s& lex_insert_stmt)
{
	InsertPlan* plan = new InsertPlan;
	plan->set_lex_stmt(lex_insert_stmt);
	return Plan_s(plan);
}

Plan::PlanType InsertPlan::type() const
{
	return Plan::INSERT_PLAN;
}

u32 InsertPlan::generate_value_plan_tree()
{
	u32 ret = SUCCESS;
	InsertStmt_s stmt = lex_stmt;
	if (stmt->query_values) {
		CHECK(generate_sub_select_plan_tree(stmt->query_values, root_operator));
	} else {
		for (u32 i = 0; i < stmt->value_list.size(); ++i) {
			CHECK(generate_subquery_evaluate(root_operator,
											stmt->value_list[i], 
											false));
		}
		root_operator = LogExprValue::make_expr_value(stmt->value_list);
		root_operator->init(est_info);
		CHECK(root_operator->compute_property());
	}
	return ret;
}

u32 InsertPlan::generate_plan_tree()
{
	u32 ret = SUCCESS;
	InsertStmt_s stmt = lex_stmt;
	CHECK(generate_value_plan_tree());
	root_operator = LogInsert::make_insert(root_operator,
										   stmt->table);
	root_operator->init(est_info);
	CHECK(root_operator->compute_property());
	return ret;
}
