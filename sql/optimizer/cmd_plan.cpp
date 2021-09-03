#include "schema_checker.h"
#include "schema_guard.h"
#include "statis_manager.h"
#include "table_space.h"
#include "sql_engine.h"
#include "object.h"
#include "cmd_plan.h"
#include "expr_stmt.h"
#include "cmd_stmt.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Optimizer;
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

u32 CMDPlan::execute(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	MY_ASSERT(lex_stmt, Stmt::DoCMD == lex_stmt->stmt_type());
	CMDStmt_s lex = lex_stmt;
	switch (lex->cmd_type) {
        case CMDStmt::CreateTable:
			CHECK(do_cmd_create_table());
			break;
		case CMDStmt::DropTable:
			CHECK(do_cmd_drop_table());
			break;
		case CMDStmt::CreateDatabase:
			CHECK(do_cmd_create_database());
			break;
		case CMDStmt::DropDatabase:
			CHECK(do_cmd_drop_database());
			break;
		case CMDStmt::ShowTables:
			CHECK(do_cmd_show_tables(query_result));
			break;
		case CMDStmt::ShowDatabases:
			CHECK(do_cmd_show_databases(query_result));
			break;
		case CMDStmt::DescTable:
			CHECK(do_cmd_desc_table(query_result));
			break;
		case CMDStmt::UseDatabase:
			CHECK(do_cmd_use_database());
			break;
		case CMDStmt::Analyze:
			CHECK(do_cmd_analyze());
			break;
        default:
			ret = INVALID_CMD_TYPE;
    }
	return ret;
}

u32 CMDPlan::build_plan()
{
	return SUCCESS;
}

Plan::PlanType CMDPlan::type() const
{
	return Plan::CMD_PLAN;
}

u32 CMDPlan::do_cmd_create_table()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	String table;
	Vector<Pair<String, String>> columns;
	
	CHECK(stmt->get_create_table_params(database, table, columns));
	SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->add_table(database, table, columns));
	CHECK(TableSpace::create_table(database, table));
	return ret;
}

u32 CMDPlan::do_cmd_drop_table()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	String table;
	
	CHECK(stmt->get_drop_table_params(database, table));
	SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	MY_ASSERT(guard);
	CHECK(TableSpace::delete_table(database, table));
	CHECK(guard->del_table(database, table));
	return ret;
}

u32 CMDPlan::do_cmd_create_database()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	
	CHECK(stmt->get_create_database_params(database));
	SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	MY_ASSERT(guard);
	CHECK(TableSpace::create_database(database));
	CHECK(guard->add_database(database));
	return ret;
}

u32 CMDPlan::do_cmd_drop_database()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	
	CHECK(stmt->get_drop_database_params(database));
	SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->del_database(database));
	CHECK(TableSpace::delete_database(database));
	return ret;
}

u32 CMDPlan::do_cmd_show_tables(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	Vector<String> tables;
	
	CHECK(stmt->get_show_tables_params(database));
	SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->get_all_table(database, tables));
	String title = String("tables_in_") + database;
	query_result->set_column_num(1);
	query_result->set_result_title(0, title);
	query_result->set_result_type(0, T_VARCHAR);
	for (u32 i = 0; i < tables.size(); ++i) {
		Object_s table = Varchar::make_object(tables[i]);
		query_result->add_row(table);
	}
	return ret;
}

u32 CMDPlan::do_cmd_show_databases(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	bool is_select_current_database;
	Vector<String> databases;
	
	CHECK(stmt->get_show_databases_params(is_select_current_database));
	SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->get_all_database(databases));
	if (is_select_current_database) {
		String title = String("database()");
		query_result->set_column_num(1);
		query_result->set_result_title(0, title);
		query_result->set_result_type(0, T_VARCHAR);
		Object_s database = Varchar::make_object(query_ctx->cur_database);
		query_result->add_row(database);
	}
	else {
		String title = String("databases");
		query_result->set_column_num(1);
		query_result->set_result_title(0, title);
		query_result->set_result_type(0, T_VARCHAR);
		for (u32 i = 0; i < databases.size(); ++i) {
			Object_s database = Varchar::make_object(databases[i]);
			query_result->add_row(database);
		}
	}
	return ret;
}

u32 CMDPlan::do_cmd_desc_table(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	String table;
	bool is_show_table_statis;
	bool is_show_column_statis;
	
	CHECK(stmt->get_desc_table_params(database, table, is_show_table_statis, is_show_column_statis));
	if (is_show_table_statis) {
		CHECK(show_table_statis(database, table, query_result));
	} else if (is_show_column_statis) {
		CHECK(show_column_statis(database, table, query_result));
	} else {
		CHECK(desc_table(database, table, query_result));
	}
	return ret;
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

u32 CMDPlan::desc_table(const String &database, const String &table, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	Vector<ColumnInfo_s> columns;
	u32 table_id = INVALID_ID;
	CHECK(checker->get_table_id(database, table, table_id));
	CHECK(checker->get_all_columns(table_id, columns));
	query_result->set_column_num(6);
	
	String name = String("Field");
	query_result->set_result_title(0, name);
	query_result->set_result_type(0, T_VARCHAR);
	String type = String("type");
	query_result->set_result_title(1, type);
	query_result->set_result_type(1, T_VARCHAR);
	String Null = String("Null");
	query_result->set_result_title(2, Null);
	query_result->set_result_type(2, T_VARCHAR);
	String key = String("Key");
	query_result->set_result_title(3, key);
	query_result->set_result_type(3, T_VARCHAR);
	String Default = String("Default");
	query_result->set_result_title(4, Default);
	query_result->set_result_type(4, T_VARCHAR);
	String extra = String("Extra");
	query_result->set_result_title(5, extra);
	query_result->set_result_type(5, T_VARCHAR);

	for (u32 i = 0; i < columns.size(); ++i) {
		Vector<Object_s> cells;
		Object_s name = Varchar::make_object(change_to_mysql_type(columns[i]->column_name));
		cells.push_back(name);
		Object_s type = Varchar::make_object(change_to_mysql_type(columns[i]->column_type));
		cells.push_back(type);
		Object_s Null = Varchar::make_object("YES");
		cells.push_back(Null);
		Object_s key = Varchar::make_object("");
		cells.push_back(key);
		Object_s Default = Varchar::make_object("NULL");
		cells.push_back(Default);
		Object_s extra = Varchar::make_object("");
		cells.push_back(extra);
		query_result->add_row(cells);
	}
	return ret;
}

u32 CMDPlan::show_table_statis(const String &database, const String &table, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	u32 tid = INVALID_ID;
	CHECK(checker->get_table_id(database, table, tid));
	String query = R"(select row_count,space_size,analyze_time
				from `system`.`table_statis`  
				where `tid` = )" + std::to_string(tid) +
		R"( order by `analyze_time` desc;)";
	CHECK(SqlEngine::handle_inner_sql(query, *query_ctx, query_result));
	return ret;
}

u32 CMDPlan::show_column_statis(const String &database, const String &table, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	u32 tid = INVALID_ID;
	CHECK(checker->get_table_id(database, table, tid));
	String query = R"(select col.name as COLUMN_NAME, col.type as TYPE, cs.ndv as NDV,cs.null_count as NULL_COUNT,
				cs.max_value as MAX_VALUE, cs.min_value as MIN_VALUE, cs.analyze_time as ANALYZE_TIME
				from system.sys_databases as db, system.sys_tables as tb, 
				system.sys_columns as col, `system`.`column_statis` as cs  
				where db.id=tb.db_id and tb.id = col.table_id and col.id=cs.cid and 
				db.name=")" + database + R"(" and tb.name=")" + table + R"(" and cs.`tid` = )" + 
				std::to_string(tid) + R"( order by `analyze_time` desc;)";
	CHECK(SqlEngine::handle_inner_sql(query, *query_ctx, query_result));
	return ret;
}

u32 CMDPlan::do_cmd_use_database()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	u32 id;
	
	CHECK(stmt->get_use_database_params(database));
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	CHECK(checker->get_database_id(database, id));
	query_ctx->cur_database = database;
	return ret;
}

u32 CMDPlan::do_cmd_analyze()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	String database;
	String table;
	double sample_size;
	
	CHECK(stmt->get_analyze_params(database, table, sample_size));
	StatisManager_s manager = StatisManager::make_statis_manager();
	MY_ASSERT(manager);
	CHECK(manager->analyze_table(database, table, sample_size));
	return ret;
}