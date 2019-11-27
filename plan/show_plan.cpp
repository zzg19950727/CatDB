#include "schema_checker.h"
#include "request_handle.h"
#include "query_result.h"
#include "show_plan.h"
#include "show_stmt.h"
#include "expr_stmt.h"
#include "object.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Server;

ShowTablesPlan::ShowTablesPlan()
{

}

ShowTablesPlan::~ShowTablesPlan()
{

}

Plan_s ShowTablesPlan::make_show_tables_plan(const Stmt_s& lex_show_stmt)
{
	ShowTablesPlan* plan = new ShowTablesPlan();
	plan->set_lex_stmt(lex_show_stmt);
	return Plan_s(plan);
}

u32 ShowTablesPlan::execute()
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	Vector<String> tables;
	u32 ret = checker->show_tables(database, tables);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	RowDesc row_desc(1);
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	result_title = Row::make_row(row_desc);
	String title = String("tables_in_") + database;
	Object_s table = Varchar::make_object(title);
	result_title->set_cell(0, table);
	for (u32 i = 0; i < tables.size(); ++i) {
		Row_s row = Row::make_row(row_desc);
		Object_s table = Varchar::make_object(tables[i]);
		row->set_cell(0, table);
		query_result->add_row(row);
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 ShowTablesPlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::ShowTables)
	{
		Log(LOG_ERR, "ShowTablesPlan", "error lex stmt when build show tables plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	ShowTablesStmt* lex = dynamic_cast<ShowTablesStmt*>(lex_stmt.get());
	database = lex->database;
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 ShowTablesPlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType ShowTablesPlan::type() const
{
	return Plan::ShowTables;
}

ShowDatabasesPlan::ShowDatabasesPlan()
	:is_select_current_database(false)
{

}

ShowDatabasesPlan::~ShowDatabasesPlan()
{

}

Plan_s ShowDatabasesPlan::make_show_databases_plan(const Stmt_s& lex_show_stmt)
{
	ShowDatabasesPlan* plan = new ShowDatabasesPlan();
	plan->set_lex_stmt(lex_show_stmt);
	return Plan_s(plan);
}

u32 ShowDatabasesPlan::execute()
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	Vector<String> databases;
	u32 ret = checker->show_database(databases);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	RowDesc row_desc(1);
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	result_title = Row::make_row(row_desc);
	if (is_select_current_database) {
		Object_s database = Varchar::make_object("database()");
		result_title->set_cell(0, database);
		Row_s row = Row::make_row(row_desc);
		database = Varchar::make_object(thd->cur_database);
		row->set_cell(0, database);
		query_result->add_row(row);
	}
	else {
		Object_s database = Varchar::make_object("database");
		result_title->set_cell(0, database);
		for (u32 i = 0; i < databases.size(); ++i) {
			Row_s row = Row::make_row(row_desc);
			Object_s database = Varchar::make_object(databases[i]);
			row->set_cell(0, database);
			query_result->add_row(row);
		}
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 ShowDatabasesPlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::ShowDatabases)
	{
		Log(LOG_ERR, "ShowDatabasesPlan", "error lex stmt when build show databases plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	ShowDatabasesStmt* lex = dynamic_cast<ShowDatabasesStmt*>(lex_stmt.get());
	is_select_current_database = lex->is_select_current_database;
	return SUCCESS;
}

u32 ShowDatabasesPlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType ShowDatabasesPlan::type() const
{
	return Plan::ShowDatabases;
}

DescTablePlan::DescTablePlan()
{

}

DescTablePlan::~DescTablePlan()
{

}

Plan_s DescTablePlan::make_desc_table_plan(const Stmt_s& lex_show_stmt)
{
	DescTablePlan* plan = new DescTablePlan();
	plan->set_lex_stmt(lex_show_stmt);
	return Plan_s(plan);
}

String change_to_mysql_type(const String& type)
{
	if (type == "number") {
		return "double";
	}
	else {
		return type;
	}
}

u32 DescTablePlan::execute()
{
	if (database.empty() || table.empty()) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	Vector<Pair<String,String>> columns;
	u32 ret = checker->desc_table(database, table, columns);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	RowDesc row_desc(6);
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	result_title = Row::make_row(row_desc);
	Object_s name = Varchar::make_object("Field");
	result_title->set_cell(0, name);
	Object_s type = Varchar::make_object("type");
	result_title->set_cell(1, type);
	Object_s Null = Varchar::make_object("Null");
	result_title->set_cell(2, Null);
	Object_s key = Varchar::make_object("Key");
	result_title->set_cell(3, key);
	Object_s Default = Varchar::make_object("Default");
	result_title->set_cell(4, Default);
	Object_s extra = Varchar::make_object("Extra");
	result_title->set_cell(5, extra);
	
	for (u32 i = 0; i < columns.size(); ++i) {
		Row_s row = Row::make_row(row_desc);
		Object_s name = Varchar::make_object( change_to_mysql_type(columns[i].first) );
		row->set_cell(0, name);
		Object_s type = Varchar::make_object( change_to_mysql_type(columns[i].second) );
		row->set_cell(1, type);
		Object_s Null = Varchar::make_object("YES");
		row->set_cell(2, Null);
		Object_s key = Varchar::make_object("");
		row->set_cell(3, key);
		Object_s Default = Varchar::make_object("NULL");
		row->set_cell(4, Default);
		Object_s extra = Varchar::make_object("");
		row->set_cell(5, extra);
		query_result->add_row(row);
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 DescTablePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::DescTable)
	{
		Log(LOG_ERR, "DescTablePlan", "error lex stmt when build desc table plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	DescTableStmt* lex = dynamic_cast<DescTableStmt*>(lex_stmt.get());
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

u32 DescTablePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType DescTablePlan::type() const
{
	return Plan::DescTable;
}

UseDatabasePlan::UseDatabasePlan()
{

}

UseDatabasePlan::~UseDatabasePlan()
{

}

Plan_s UseDatabasePlan::make_use_database_plan(const Stmt_s& lex_show_stmt)
{
	UseDatabasePlan* plan = new UseDatabasePlan();
	plan->set_lex_stmt(lex_show_stmt);
	return Plan_s(plan);
}

u32 UseDatabasePlan::execute()
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
	thd->cur_database = database;
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 UseDatabasePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::UseDatabase)
	{
		Log(LOG_ERR, "UseDatabasePlan", "error lex stmt when build use database plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	UseDatabaseStmt* lex = dynamic_cast<UseDatabaseStmt*>(lex_stmt.get());
	database = lex->database;
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 UseDatabasePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType UseDatabasePlan::type() const
{
	return Plan::UseDatabase;
}

AnalyzePlan::AnalyzePlan()
{
}

AnalyzePlan::~AnalyzePlan()
{
}

Plan_s AnalyzePlan::make_analyze_plan(const Stmt_s & lex_show_stmt)
{
	AnalyzePlan* plan = new AnalyzePlan();
	plan->set_lex_stmt(lex_show_stmt);
	return Plan_s(plan);
}

u32 AnalyzePlan::execute()
{
	if (database.empty() || table.empty()) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 id;
	u32 ret = checker->analyze_table(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 AnalyzePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Analyze)
	{
		Log(LOG_ERR, "AnalyzePlan", "error lex stmt when build analyze table plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	AnalyzeStmt* lex = dynamic_cast<AnalyzeStmt*>(lex_stmt.get());
	database = lex->database;
	table = lex->table;
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 AnalyzePlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType AnalyzePlan::type() const
{
	return Plan::Analyze;
}
