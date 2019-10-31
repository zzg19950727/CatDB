#include "schema_checker.h"
#include "table_space.h"
#include "drop_plan.h"
#include "drop_stmt.h"
#include "expr_stmt.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Storage;

DropTablePlan::DropTablePlan()
{

}

DropTablePlan::~DropTablePlan()
{

}

Plan_s DropTablePlan::make_drop_table_plan(const Stmt_s& lex_drop_stmt)
{
	DropTablePlan* plan = new DropTablePlan();
	plan->set_lex_stmt(lex_drop_stmt);
	return Plan_s(plan);
}

u32 DropTablePlan::execute()
{
	if (database.empty() || table.empty()) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 ret = TableSpace::delete_table(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = checker->delete_table(database, table);
	set_error_code(ret);
	return ret;
}

u32 DropTablePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::DropTable)
	{
		Log(LOG_ERR, "DropTablePlan", "error lex stmt when build drop table plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	DropTableStmt* lex = dynamic_cast<DropTableStmt*>(lex_stmt.get());
	if (lex->table->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(lex->table.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Table) {
		return ERROR_LEX_STMT;
	}
	TableStmt* table_stmt = dynamic_cast<TableStmt*>(expr_stmt);
	database = table_stmt->database;
	table = table_stmt->table_name;
	return SUCCESS;
}

u32 DropTablePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType DropTablePlan::type() const
{
	return Plan::DropTable;
}

DropDatabasePlan::DropDatabasePlan()
{

}

DropDatabasePlan::~DropDatabasePlan()
{

}

Plan_s DropDatabasePlan::make_drop_database_plan(const Stmt_s& lex_drop_stmt)
{
	DropDatabasePlan* plan = new DropDatabasePlan();
	plan->set_lex_stmt(lex_drop_stmt);
	return Plan_s(plan);
}

u32 DropDatabasePlan::execute()
{
	if (database.empty()) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 id;
	u32 ret = checker->get_database_id(database, id);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = TableSpace::delete_database(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = checker->delete_database(database);
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 DropDatabasePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::DropDatabase)
	{
		Log(LOG_ERR, "DropDatabasePlan", "error lex stmt when build drop database plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	DropDatabaseStmt* lex = dynamic_cast<DropDatabaseStmt*>(lex_stmt.get());
	database = lex->database;
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 DropDatabasePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType DropDatabasePlan::type() const
{
	return Plan::DropDatabase;
}

