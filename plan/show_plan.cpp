#include "schema_checker.h"
#include "request_handle.h"
#include "query_result.h"
#include "sql_driver.h"
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
using CatDB::SqlDriver;

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
	:is_show_table_statis(false),
	is_show_column_statis(false)
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
	if (is_show_table_statis) {
		return show_table_statis();
	}
	else if (is_show_column_statis) {
		return show_column_statis();
	}
	else {
		return desc_table();
	}
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
	is_show_column_statis = lex->is_show_column_statis;
	is_show_table_statis = lex->is_show_table_statis;
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

u32 DescTablePlan::desc_table()
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	Vector<Pair<String, String>> columns;
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
		Object_s name = Varchar::make_object(change_to_mysql_type(columns[i].first));
		row->set_cell(0, name);
		Object_s type = Varchar::make_object(change_to_mysql_type(columns[i].second));
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

u32 DescTablePlan::show_table_statis()
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 tid = checker->get_table_id(database, table);
	SqlDriver parser;
	Plan_s plan;
	String query = R"(select row_count,space_size,analyze_time
				from `system`.`table_statis`  
				where `tid` = )" + std::to_string(tid) +
		R"( order by `analyze_time` desc;)";
	int ret = parser.parse_sql(query);
	//为保证客户端兼容，不支持的SQL语法返回OK包
	if (parser.is_sys_error()) {
		return ERR_UNEXPECTED;
	}
	else if (parser.is_syntax_error()) {
		return ERR_UNEXPECTED;
	}
	else if (!parser.parse_result()) {
		return ERR_UNEXPECTED;
	}
	else {
		plan = Plan::make_plan(parser.parse_result());
		u32 ret = plan->optimizer();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return ERR_UNEXPECTED;
			}
			else {
				return ERR_UNEXPECTED;
			}
		}
		ret = plan->build_plan();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return ERR_UNEXPECTED;
			}
			else {
				return ERR_UNEXPECTED;
			}
		}
		ret = plan->execute();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return ERR_UNEXPECTED;
			}
			else {
				return ERR_UNEXPECTED;
			}
		}
		else {
				result = plan->get_result();
				result_title = plan->get_result_title();
				return SUCCESS;
		}
	}
}

u32 DescTablePlan::show_column_statis()
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 tid = checker->get_table_id(database, table);
	SqlDriver parser;
	Plan_s plan;
	String query = R"(select col.name as COLUMN_NAME, col.type as TYPE, cs.ndv as NDV,cs.null_count as NULL_COUNT,
				cs.max_value as MAX_VALUE, cs.min_value as MIN_VALUE, cs.analyze_time as ANALYZE_TIME
				from system.sys_databases as db, system.sys_tables as tb, 
				system.sys_columns as col, `system`.`column_statis` as cs  
				where db.id=tb.db_id and tb.id = col.table_id and col.id=cs.cid and 
				db.name=")" + database + R"(" and tb.name=")" + table + R"(" and cs.`tid` = )" + 
				std::to_string(tid) + R"( order by `analyze_time` desc;)";
	int ret = parser.parse_sql(query);
	//为保证客户端兼容，不支持的SQL语法返回OK包
	if (parser.is_sys_error()) {
		return ERR_UNEXPECTED;
	}
	else if (parser.is_syntax_error()) {
		return ERR_UNEXPECTED;
	}
	else if (!parser.parse_result()) {
		return ERR_UNEXPECTED;
	}
	else {
		plan = Plan::make_plan(parser.parse_result());
		u32 ret = plan->optimizer();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return ERR_UNEXPECTED;
			}
			else {
				return ERR_UNEXPECTED;
			}
		}
		ret = plan->build_plan();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return ERR_UNEXPECTED;
			}
			else {
				return ERR_UNEXPECTED;
			}
		}
		ret = plan->execute();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return ERR_UNEXPECTED;
			}
			else {
				return ERR_UNEXPECTED;
			}
		}
		else {
			result = plan->get_result();
			result_title = plan->get_result_title();
			return SUCCESS;
		}
	}
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
	if (thd) {
		thd->cur_database = database;
	}
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
