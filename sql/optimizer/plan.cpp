#include "logical_operator.h"
#include "insert_plan.h"
#include "delete_plan.h"
#include "update_plan.h"
#include "select_plan.h"
#include "cmd_plan.h"
#include "plan.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;

Plan::Plan()
	:query_ctx(NULL)
{
}

Plan::~Plan()
{
}

Plan_s Plan::make_plan(const Stmt_s& lex_stmt)
{
	if (!lex_stmt) {
		return Plan_s();
	}
	switch (lex_stmt->stmt_type())
	{
	case Stmt::Insert:
		return InsertPlan::make_insert_plan(lex_stmt);
	case Stmt::Delete:
		return DeletePlan::make_delete_plan(lex_stmt);
	case Stmt::Update:
		return UpdatePlan::make_update_plan(lex_stmt);
	case Stmt::Select:
		return SelectPlan::make_select_plan(lex_stmt);
	case Stmt::SetOperation:
		return SelectPlan::make_select_plan(lex_stmt);
	case Stmt::DoCMD:
		return CMDPlan::make_cmd_plan(lex_stmt);
	default:
		return Plan_s();
	}
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
