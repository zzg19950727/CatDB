#include "logical_operator.h"
#include "insert_plan.h"
#include "delete_plan.h"
#include "update_plan.h"
#include "select_plan.h"
#include "cmd_plan.h"
#include "expr_stmt.h"
#include "query_ctx.h"
#include "plan.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;

Plan::Plan()
{
}

Plan::~Plan()
{
}

Plan_s Plan::make_plan(const Stmt_s& lex_stmt, QueryCtx_s &ctx)
{
	if (!lex_stmt) {
		return Plan_s();
	}
	Plan_s plan = Plan_s();
	switch (lex_stmt->stmt_type())
	{
	case Insert:
		plan = InsertPlan::make_insert_plan(lex_stmt);
		break;
	case Delete:
		plan = DeletePlan::make_delete_plan(lex_stmt);
		break;
	case Update:
		plan = UpdatePlan::make_update_plan(lex_stmt);
		break;
	case Select:
		plan = SelectPlan::make_select_plan(lex_stmt);
		break;
	case SetOperation:
		plan = SelectPlan::make_select_plan(lex_stmt);
		break;
	case DoCMD:
		plan = CMDPlan::make_cmd_plan(lex_stmt);
		break;
	}
	plan->query_ctx = ctx;
	return plan;
}

void Plan::set_lex_stmt(const Stmt_s & stmt)
{
	lex_stmt = stmt;
}

Stmt_s Plan::get_lex_stmt() const
{
	return lex_stmt;
}

LogicalOperator_s Plan::get_root_operator()
{
	return root_operator;
}
