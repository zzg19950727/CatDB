#include "schema_checker.h"
#include "table_space.h"
#include "request_handle.h"
#include "query_result.h"
#include "sql_driver.h"
#include "object.h"
#include "cmd_plan.h"
#include "expr_stmt.h"
#include "cmd_stmt.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Storage;

CMDPlan::CMDPlan()
{

}

CMDPlan::~CMDPlan()
{

}

Plan_s CMDPlan::make_cmd_plan(const Stmt_s& lex_create_stmt)
{
	CMDPlan* plan = new CMDPlan();
	plan->set_lex_stmt(lex_create_stmt);
	return Plan_s(plan);
}

bool CMDPlan::send_plan_result() const
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::DoCMD)
	{
		LOG_ERR("error lex stmt when build cmd plan", K(lex_stmt));
		return false;
	}
	bool ret = false;
	CMDStmt* lex = dynamic_cast<CMDStmt*>(lex_stmt.get());
	switch (lex->cmd_type) {
        case CMDStmt::CreateTable:
		case CMDStmt::DropTable:
		case CMDStmt::CreateDatabase:
		case CMDStmt::DropDatabase:
			break;
		case CMDStmt::ShowTables:
			ret = true;
			break;
		case CMDStmt::ShowDatabases:
			ret = true;
			break;
		case CMDStmt::DescTable:
			ret = true;
			break;
		case CMDStmt::UseDatabase:
		case CMDStmt::Analyze:
			ret = false;
			break;
        default:
			ret = false;
    }
	return ret;
}

u32 CMDPlan::execute()
{
	u32 ret = SUCCESS;
    if (!lex_stmt || lex_stmt->stmt_type() != Stmt::DoCMD)
	{
		LOG_ERR("error lex stmt when build cmd plan", K(lex_stmt));
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	CMDStmt* lex = dynamic_cast<CMDStmt*>(lex_stmt.get());
	switch (lex->cmd_type) {
        case CMDStmt::CreateTable:
			do_cmd_create_table();
			break;
		case CMDStmt::DropTable:
			do_cmd_drop_table();
			break;
		case CMDStmt::CreateDatabase:
			do_cmd_create_database();
			break;
		case CMDStmt::DropDatabase:
			do_cmd_drop_database();
			break;
		case CMDStmt::ShowTables:
			do_cmd_show_tables();
			break;
		case CMDStmt::ShowDatabases:
			do_cmd_show_databases();
			break;
		case CMDStmt::DescTable:
			do_cmd_desc_table();
			break;
		case CMDStmt::UseDatabase:
			do_cmd_use_database();
			break;
		case CMDStmt::Analyze:
			do_cmd_analyze();
			break;
        default:
			ret = INVALID_CMD_TYPE;
    }
	set_error_code(ret);
	return ret;
}

u32 CMDPlan::build_plan()
{
	return SUCCESS;
}

u32 CMDPlan::optimizer()
{
	return SUCCESS;
}

Plan::PlanType CMDPlan::type() const
{
	return Plan::DoCMD;
}

u32 CMDPlan::get_stmt(CMDStmt *&stmt)
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::DoCMD)
	{
		LOG_ERR("error lex stmt", K(lex_stmt));
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	stmt = dynamic_cast<CMDStmt*>(lex_stmt.get());
	return SUCCESS;
}

u32 CMDPlan::do_cmd_create_table()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	String table;
	Vector<Pair<String, String>> columns;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_create_table_params(database, table, columns);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = checker->add_table(database, table, columns);
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

u32 CMDPlan::do_cmd_drop_table()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	String table;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_drop_table_params(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = TableSpace::delete_table(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = checker->delete_table(database, table);
	set_error_code(ret);
	return ret;
}

u32 CMDPlan::do_cmd_create_database()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_create_database_params(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = TableSpace::create_database(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = checker->add_database(database);
	set_error_code(ret);
	return ret;
}

u32 CMDPlan::do_cmd_drop_database()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	u32 id;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_drop_database_params(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = checker->get_database_id(database, id);
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

u32 CMDPlan::do_cmd_show_tables()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	Vector<String> tables;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_show_tables_params(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = checker->show_tables(database, tables);
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

u32 CMDPlan::do_cmd_show_databases()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	bool is_select_current_database;
	Vector<String> databases;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_show_databases_params(is_select_current_database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = checker->show_database(databases);
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

u32 CMDPlan::do_cmd_desc_table()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	String table;
	bool is_show_table_statis;
	bool is_show_column_statis;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_desc_table_params(database, table, is_show_table_statis, is_show_column_statis);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	if (is_show_table_statis) {
		return show_table_statis(database, table);
	}
	else if (is_show_column_statis) {
		return show_column_statis(database, table);
	}
	else {
		return desc_table(database, table);
	}
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

u32 CMDPlan::desc_table(const String &database, const String &table)
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

u32 CMDPlan::show_table_statis(const String &database, const String &table)
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
	//Ϊ��֤�ͻ��˼��ݣ���֧�ֵ�SQL�﷨����OK��
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

u32 CMDPlan::show_column_statis(const String &database, const String &table)
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
	//Ϊ��֤�ͻ��˼��ݣ���֧�ֵ�SQL�﷨����OK��
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

u32 CMDPlan::do_cmd_use_database()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	u32 id;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_use_database_params(database);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = checker->get_database_id(database, id);
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

u32 CMDPlan::do_cmd_analyze()
{
	u32 ret = SUCCESS;
	CMDStmt *stmt = NULL;
	String database;
	String table;
	ret = get_stmt(stmt);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	ret = stmt->get_analyze_params(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ret = checker->analyze_table(database, table);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}