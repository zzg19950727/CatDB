#include "update_plan.h"
#include "update_stmt.h"
#include "log_update.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

UpdatePlan::UpdatePlan()
{

}

UpdatePlan::~UpdatePlan()
{

}

Plan_s UpdatePlan::make_update_plan(const Stmt_s& lex_update_stmt)
{
	UpdatePlan* plan = new UpdatePlan;
	plan->set_lex_stmt(lex_update_stmt);
	return Plan_s(plan);
}

Plan::PlanType UpdatePlan::type() const
{
	return Plan::UPDATE_PLAN;
}

u32 UpdatePlan::generate_plan_tree()
{
	u32 ret = SUCCESS;
	UpdateStmt_s stmt = lex_stmt;
	CHECK(generate_join_order());
	CHECK(generate_subplan());
	root_operator = LogUpdate::make_update(root_operator,
										   stmt->table,
										   stmt->update_assign_stmt,
										   stmt->column_exprs,
										   stmt->value_exprs,
										   stmt->row_id_col);
	root_operator->init(query_ctx, est_info);
	CHECK(root_operator->compute_property());
	return ret;
}
