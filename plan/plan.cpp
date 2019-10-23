#include "insert_plan.h"
#include "delete_plan.h"
#include "update_plan.h"
#include "plan.h"
#include "stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Sql;

Plan::Plan()
	:affect_rows_(0)
{
}

Plan::~Plan()
{
}

Plan_s Plan::make_plan(const Stmt_s& lex_stmt)
{
	switch (lex_stmt->stmt_type())
	{
	case Stmt::Insert:
		return InsertPlan::make_insert_plan(lex_stmt);
	case Stmt::Delete:
		return DeletePlan::make_delete_plan(lex_stmt);
	case Stmt::Update:
		return UpdatePlan::make_update_plan(lex_stmt);
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

Object_s Plan::get_result()
{
	return result;
}

u32 CatDB::Sql::Plan::affect_rows() const
{
	return affect_rows_;
}
