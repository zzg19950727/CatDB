#include "insert_plan.h"
#include "insert_stmt.h"
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
		Vector<Vector<ExprStmt_s>> values;
		u32 size = 0;
		for (u32 i = 0; i < stmt->value_list.size(); ++i) {
			MY_ASSERT(ExprStmt::List == stmt->value_list[i]->expr_type());
			if (0 == i) {
				size = stmt->value_list[i]->params.size();
				MY_ASSERT(size > 0);
			} else {
				MY_ASSERT(size == stmt->value_list[i]->params.size());
			}
			values.push_back(stmt->value_list[i]->params);
		}
		root_operator = LogExprValue::make_expr_value(values);
		root_operator->set_query_ctx(query_ctx);
	}
	return ret;
}

u32 InsertPlan::generate_plan_tree()
{
	u32 ret = SUCCESS;
	InsertStmt_s stmt = lex_stmt;
	CHECK(generate_value_plan_tree());
	CHECK(generate_subplan());
	root_operator = LogInsert::make_insert(root_operator,
										   stmt->table);
	root_operator->set_query_ctx(query_ctx);
	return ret;
}
