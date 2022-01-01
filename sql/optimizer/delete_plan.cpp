#include "delete_plan.h"
#include "delete_stmt.h"
#include "log_delete.h"
#include "log_distinct.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

DeletePlan::DeletePlan()
{

}

DeletePlan::~DeletePlan()
{

}

Plan_s DeletePlan::make_delete_plan(const Stmt_s& lex_delete_stmt)
{
	DeletePlan* plan = new DeletePlan;
	plan->set_lex_stmt(lex_delete_stmt);
	return Plan_s(plan);
}

Plan::PlanType DeletePlan::type() const
{
	return Plan::DELETE_PLAN;
}

u32 DeletePlan::generate_plan_tree()
{
	u32 ret = SUCCESS;
	DeleteStmt_s stmt = lex_stmt;
	CHECK(generate_join_order());
	CHECK(generate_subplan());
	bool is_delete_all = false;
	if (stmt->where_stmt.empty() && stmt->from_stmts.size() == 1) {
		is_delete_all = stmt->from_stmts[0]->same_as(stmt->table);
	} else {
		root_operator = LogDistinct::make_distinct(root_operator);
		root_operator->init(query_ctx, est_info);
		LogDistinct_s distinct_op = root_operator;
		distinct_op->add_distinct_expr(stmt->row_id_col);
		CHECK(root_operator->compute_property());
	}
	root_operator = LogDelete::make_delete(root_operator,
										   stmt->table,
										   stmt->row_id_col,
										   is_delete_all);
	root_operator->init(query_ctx, est_info);
	CHECK(root_operator->compute_property());
	return ret;
}
