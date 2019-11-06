#include "schema_checker.h"
#include "table_space.h"
#include "create_plan.h"
#include "create_stmt.h"
#include "expr_stmt.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Storage;

CreateTablePlan::CreateTablePlan()
{

}

CreateTablePlan::~CreateTablePlan()
{

}

Plan_s CreateTablePlan::make_create_table_plan(const Stmt_s& lex_create_stmt)
{
	CreateTablePlan* plan = new CreateTablePlan();
	plan->set_lex_stmt(lex_create_stmt);
	return Plan_s(plan);
}

u32 CreateTablePlan::execute()
{
	if (database.empty() || table.empty() || columns.empty()) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 ret = checker->add_table(database, table, columns);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = TableSpace::create_table(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	set_error_code(ret);
	return ret;
}

u32 CreateTablePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::CreateTable)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt when build create table plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	CreateTableStmt* lex = dynamic_cast<CreateTableStmt*>(lex_stmt.get());
	if (lex->table->stmt_type() != Stmt::Expr) {
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(lex->table.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Table) {
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	TableStmt* table_stmt = dynamic_cast<TableStmt*>(expr_stmt);
	database = table_stmt->database;
	table = table_stmt->table_name;

	if (lex->column_define_list->stmt_type() != Stmt::Expr) {
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	expr_stmt = dynamic_cast<ExprStmt*>(lex->column_define_list.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	ListStmt* list = dynamic_cast<ListStmt*>(expr_stmt);
	for (u32 i = 0; i < list->stmt_list.size(); ++i) {
		if (list->stmt_list[i]->stmt_type() != Stmt::ColumnDefine) {
			set_error_code(ERROR_LEX_STMT);
			return ERROR_LEX_STMT;
		}
		ColumnDefineStmt* column_define = dynamic_cast<ColumnDefineStmt*>(list->stmt_list[i].get());
		String type;
		switch (column_define->data_type)
		{
		case ColumnDefineStmt::NUMBER:
			type = "number";
			break;
		case ColumnDefineStmt::DATETIME:
			type = "datetime";
			break;
		case ColumnDefineStmt::VARCHAR:
			type = "varchar";
			break;
		case ColumnDefineStmt::INT:
			type = "int";
			break;
		default:
			set_error_code(ERROR_LEX_STMT);
			return ERROR_LEX_STMT;
		}
		columns.push_back(Pair<String, String>(column_define->column_name, type));
	}
	return SUCCESS;
}

u32 CreateTablePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType CreateTablePlan::type() const
{
	return Plan::CreateTable;
}

CreateDatabasePlan::CreateDatabasePlan()
{

}

CreateDatabasePlan::~CreateDatabasePlan()
{

}

Plan_s CreateDatabasePlan::make_create_database_plan(const Stmt_s& lex_create_stmt)
{
	CreateDatabasePlan* plan = new CreateDatabasePlan();
	plan->set_lex_stmt(lex_create_stmt);
	return Plan_s(plan);
}

u32 CreateDatabasePlan::execute()
{
	if (database.empty()) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 ret = TableSpace::create_database(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = checker->add_database(database);
	set_error_code(ret);
	return ret;
}

u32 CreateDatabasePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::CreateDatabase)
	{
		Log(LOG_ERR, "CreateDatabasePlan", "error lex stmt when build create database plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	CreateDatabaseStmt* lex = dynamic_cast<CreateDatabaseStmt*>(lex_stmt.get());
	database = lex->database;
	return SUCCESS;
}

u32 CreateDatabasePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType CreateDatabasePlan::type() const
{
	return Plan::CreateDatabase;
}