#include "schema_checker.h"
#include "schema_guard.h"
#include "session_info.h"
#include "statis_manager.h"
#include "table_space.h"
#include "sql_engine.h"
#include "object.h"
#include "cmd_plan.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "cmd_stmt.h"
#include "select_stmt.h"
#include "request_handle.h"
#include "memory_monitor.h"
#include "dml_resolver.h"
#include "obj_varchar.h"
#include "session_info.h"
#include "global_context.h"
#include "package_manager.h"
#include "package_executor.h"
#include "server.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Optimizer;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Storage;
using namespace CatDB::Server;
using namespace CatDB::Share;
using namespace CatDB::Package;

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
	MY_ASSERT(lex_stmt, DoCMD == lex_stmt->stmt_type());
	CMDStmt_s lex = lex_stmt;
	switch (lex->cmd_type) {
        case CreateTable:
			CHECK(do_cmd_create_table());
			break;
		case DropTable:
			CHECK(do_cmd_drop_table());
			break;
		case CreateDatabase:
			CHECK(do_cmd_create_database());
			break;
		case DropDatabase:
			CHECK(do_cmd_drop_database());
			break;
		case ShowTables:
			CHECK(do_cmd_show_tables(query_result));
			break;
		case ShowDatabases:
			CHECK(do_cmd_show_databases(query_result));
			break;
		case DescTable:
			CHECK(do_cmd_desc_table(query_result));
			break;
		case UseDatabase:
			CHECK(do_cmd_use_database());
			break;
		case ShowProcesslist:
			CHECK(do_show_processlist(query_result));
			break;
		case Kill:
			CHECK(do_kill_process());
			break;
		case ShowMemory:
			CHECK(do_show_memory(query_result));
			break;
		case CreateView:
			CHECK(do_create_view());
			break;
		case DropView:
			CHECK(do_drop_view());
			break;
		case CreatePackage:
			CHECK(do_create_package());
			break;
		case ExecFunction:
			CHECK(do_exec_package(query_result));
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
	CreateTableParam_s param = stmt->param;
	String &database = param->database_name;
	String &table = param->table_name;
	Vector<ColumnDefineStmt_s> &columns = param->column_define_list;
	Vector<String> &engine_args = param->engine_args;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	TableInfo_s info;
	ret = guard->find_table_info(database, table, info);
	if (SUCC(ret)) {
		if (USER_VIEW_TABLE == info->type) {
			String msg = "view " + database + "." + table + " exists!";
			QUERY_CTX->set_error_msg(msg);
			ret = VIEW_EXISTS;
		} else {
			String msg = "table " + database + "." + table + " exists!";
			QUERY_CTX->set_error_msg(msg);
			ret = TABLE_EXISTS;
		}
		return ret;
	} else {
		ret = SUCCESS;
	}
	engine_args.push_back(std::to_string(columns.size()));
	CHECK(guard->add_table(database, 
						   table, 
						   USER_PHY_TABLE,
						   columns, 
						   engine_args));
	CHECK(TableSpace::create_table(database, table));
	return ret;
}

u32 CMDPlan::do_cmd_drop_table()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	DropTableParam_s param = stmt->param;
	String &database = param->database_name;
	String &table = param->table_name;
	bool ignore_not_exists = param->ignore_not_exists;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	TableInfo_s info;
	ret = guard->find_table_info(database, table, info);
	if (FAIL(ret) && ignore_not_exists) {
		ret = SUCCESS;
		return ret;
	} else if (FAIL(ret)) {
		String msg = "table " + database + "." + table + " not exists!";
		QUERY_CTX->set_error_msg(msg);
		return ret;
	} else if (USER_VIEW_TABLE == info->type) {
		String msg = table + " is view not table!";
		QUERY_CTX->set_error_msg(msg);
		ret = TABLE_NOT_EXISTS;
		return ret;
	} else if (SYS_INNER_TABLE == info->type) {
		String msg = "can not drop system inner table:" + table;
		QUERY_CTX->set_error_msg(msg);
		ret = TABLE_NOT_EXISTS;
		return ret;
	}
	CHECK(TableSpace::delete_table(database, table));
	CHECK(guard->del_table(database, table));
	return ret;
}

u32 CMDPlan::do_cmd_create_database()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	CreateDBParam_s param = stmt->param;
	String &database = param->database;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	DatabaseInfo_s info;
	ret = guard->find_database_info(database, info);
	if (SUCC(ret)) {
		String msg = "database " + database + " exists!";
		QUERY_CTX->set_error_msg(msg);
		ret = DATABASE_EXISTS;
		return ret;
	} else {
		ret = SUCCESS;
	}
	CHECK(TableSpace::create_database(database));
	CHECK(guard->add_database(database));
	return ret;
}

u32 CMDPlan::do_cmd_drop_database()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	DropDBParam_s param = stmt->param;
	String &database = param->database;
	bool ignore_not_exists = param->ignore_not_exists;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	DatabaseInfo_s info;
	ret = guard->find_database_info(database, info);
	if (FAIL(ret) && ignore_not_exists) {
		ret = SUCCESS;
		return ret;
	} else if (FAIL(ret)) {
		String msg = "database " + database + " not exists!";
		QUERY_CTX->set_error_msg(msg);
		return ret;
	}
	CHECK(guard->del_database(database));
	CHECK(TableSpace::delete_database(database));
	return ret;
}

u32 CMDPlan::do_cmd_show_tables(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	ShowTablesParam_s param = stmt->param;
	String &database = param->database;
	Vector<String> tables;
	
	if (database.empty()) {
		QUERY_CTX->set_error_msg("not use database!");
		ret = ERR_UNEXPECTED;
		return ret;
	}
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->get_all_table(database, tables));

	Vector<String> title;
	title.push_back(String("tables_in_") + database);
	CHECK(init_command_result_head(title, query_result));
	for (u32 i = 0; i < tables.size(); ++i) {
		Object_s table = Varchar::make_object(tables[i]);
		Vector<Object_s> objs;
		objs.push_back(table);
		CHECK(add_objects_to_result_set(objs, query_result));
	}
	return ret;
}

u32 CMDPlan::do_cmd_show_databases(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	Vector<String> databases;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->get_all_database(databases));
	Vector<String> title;
	title.push_back(String("databases"));
	CHECK(init_command_result_head(title, query_result));
	for (u32 i = 0; i < databases.size(); ++i) {
		Object_s database = Varchar::make_object(databases[i]);
		Vector<Object_s> objs;
		objs.push_back(database);
		CHECK(add_objects_to_result_set(objs, query_result));
	}
	return ret;
}

u32 CMDPlan::do_cmd_desc_table(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	DescTableParam_s param = stmt->param;
	String &database = param->database_name;
	String &table = param->table_name;

	if (database.empty()) {
		QUERY_CTX->set_error_msg("not use database!");
		ret = ERR_UNEXPECTED;
		return ret;
	}
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	TableInfo_s info;
	ret = guard->find_table_info(database, table, info);
	if (FAIL(ret)) {
		String msg = "table " + database + "." + table + " not exists!";
		QUERY_CTX->set_error_msg(msg);
		return ret;
	}

	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	Vector<ColumnInfo_s> columns;
	u32 table_id = INVALID_ID;
	CHECK(checker->get_table_id(database, table, table_id));
	CHECK(checker->get_all_columns(table_id, columns));
	
	Vector<String> title;
	title.push_back(String("Field"));
	title.push_back(String("Type"));
	title.push_back(String("Null"));
	title.push_back(String("Key"));
	title.push_back(String("Default"));
	title.push_back(String("Extra"));
	CHECK(init_command_result_head(title, query_result));

	for (u32 i = 0; i < columns.size(); ++i) {
		Vector<Object_s> cells;
		Object_s name = Varchar::make_object(columns[i]->column_name);
		cells.push_back(name);
		Object_s type = Varchar::make_object(columns[i]->column_type.to_kv_string());
		cells.push_back(type);
		Object_s Null = Varchar::make_object("YES");
		cells.push_back(Null);
		Object_s key = Varchar::make_object("");
		cells.push_back(key);
		Object_s Default = Varchar::make_object("NULL");
		cells.push_back(Default);
		Object_s extra = Varchar::make_object("");
		cells.push_back(extra);
		CHECK(add_objects_to_result_set(cells, query_result));
	}
	return ret;
}

u32 CMDPlan::do_cmd_use_database()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	UseDBParam_s param = stmt->param;
	String &database = param->database;
	u32 id;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	DatabaseInfo_s info;
	ret = guard->find_database_info(database, info);
	if (FAIL(ret)) {
		String msg = "database " + database + " not exists!";
		QUERY_CTX->set_error_msg(msg);
		return ret;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	CHECK(checker->get_database_id(database, id));
	SESSION_CTX->set_cur_database(database);
	return ret;
}

u32 CMDPlan::do_show_processlist(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	Vector<String> title;
	title.push_back(String("session id"));
	title.push_back(String("trace id"));
	title.push_back(String("time(ms)"));
	title.push_back(String("status"));
	title.push_back(String("sql"));
	CHECK(init_command_result_head(title, query_result));
	for (auto iter = GTX->get_all_processlist().begin(); iter != GTX->get_all_processlist().end(); ++iter) {
		SessionInfo_s &session_info = iter->second->get_session_info();
		Vector<Object_s> cells;
		Object_s session_id = Varchar::make_object(std::to_string(session_info->get_session_id()));
		cells.push_back(session_id);
		Object_s trace_id = Varchar::make_object(session_info->get_trace_id());
		cells.push_back(trace_id);
		Object_s time = Varchar::make_object(std::to_string(session_info->get_query_time()));
		cells.push_back(time);
		Object_s status = Varchar::make_object(session_info->get_session_status());
		cells.push_back(status);
		Object_s sql = Varchar::make_object(session_info->get_query_sql());
		cells.push_back(sql);
		CHECK(add_objects_to_result_set(cells, query_result));
	}
	return ret;
}

u32 CMDPlan::do_kill_process()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	KillSessionParam_s param = stmt->param;
	int session_id = param->session_id;
	SessionInfo_s session_info;
	GTX->get_session_info(session_id, session_info);
	if (session_info) {
		session_info->kill_query();
	} else {
		//kill self
		SESSION_CTX->kill_query();
	}
	return ret;
}

String ptr2str(void *ptr)
{
	char tmp[128] = {0};
	sprintf(tmp, "%p", ptr);
	return String(tmp);
}

u32 CMDPlan::do_show_memory(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	String mode = MemoryMonitor::make_memory_monitor().get_trace_mode();
	MemoryMonitor::make_memory_monitor().set_trace_mode("");
	String query = "delete from system.memory_use;";
	CHECK(SqlEngine::handle_inner_sql(query, query_result));
	query = "delete from system.memory_trace;";
	CHECK(SqlEngine::handle_inner_sql(query, query_result));
	String sql_insert_use = "insert into system.memory_use values";
	bool has_use_info = false;
	for (auto iter = MemoryMonitor::make_memory_monitor().mem_used.begin(); iter != MemoryMonitor::make_memory_monitor().mem_used.end(); ++iter) {
		MemoryMonitor::MemInfo *info = iter->second;
		sql_insert_use += "(";
		sql_insert_use += "`" + iter->first + "`";
		sql_insert_use += ", " + std::to_string(iter->second->use_count);
		sql_insert_use += "),";
		has_use_info = true;

		String sql_insert_trace = "insert into system.memory_trace values";
		bool has_trace_info = false;
		for (auto iter2 = info->release_trace.begin(); iter2 != info->release_trace.end(); ++iter2) {
			for (u32 j = 0; j < iter2->second.size(); ++j) {
				sql_insert_trace += "(";
				//mode
				sql_insert_trace += "`" + iter->first + "`";
				//ptr
				sql_insert_trace += ", `" + ptr2str(iter2->first) + "`";
				//release
				sql_insert_trace += ", 1";
				//trace_info
				sql_insert_trace += ", `" + iter2->second[j] + "`";
				sql_insert_trace += "),";
				has_trace_info = true;
			}
		}
		for (auto iter2 = info->reference_trace.begin(); iter2 != info->reference_trace.end(); ++iter2) {
			for (u32 j = 0; j < iter2->second.size(); ++j) {
				sql_insert_trace += "(";
				//mode
				sql_insert_trace += "`" + iter->first + "`";
				//ptr
				sql_insert_trace += ", `" + ptr2str(iter2->first) + "`";
				//release
				sql_insert_trace += ", 0";
				//trace info
				sql_insert_trace += ", `" + iter2->second[j] + "`";
				sql_insert_trace += "),";
				has_trace_info = true;
			}
		}
		if (has_trace_info) {
			sql_insert_trace[sql_insert_trace.size()-1]=';';
			CHECK(SqlEngine::handle_inner_sql(sql_insert_trace, query_result));
		}
	}
	if (has_use_info) {
		sql_insert_use[sql_insert_use.size()-1]=';';
		CHECK(SqlEngine::handle_inner_sql(sql_insert_use, query_result));
	}
	
	query = "select * from system.memory_use order by 2;";
	CHECK(SqlEngine::handle_inner_sql(query, query_result));
	MemoryMonitor::make_memory_monitor().set_trace_mode(mode);
	return ret;
}

u32 CMDPlan::do_create_view()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	CreateViewParam_s param = stmt->param;
	String &database = param->database;
	String &view_name = param->view_name;
	Vector<String> &column_define = param->column_define;
	String &view_define_sql = param->view_define_sql;
	SelectStmt_s &ref_query = param->ref_query;
	Vector<ColumnDefineStmt_s> columns;
	Vector<String> engine_args;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	TableInfo_s info;
	ret = guard->find_table_info(database, view_name, info);
	if (SUCC(ret)) {
		if (USER_PHY_TABLE == info->type) {
			String msg = "table " + view_name + " exists!";
			QUERY_CTX->set_error_msg(msg);
			ret = TABLE_EXISTS;
		} else {
			String msg = "view " + view_name + " exists!";
			QUERY_CTX->set_error_msg(msg);
			ret = VIEW_EXISTS;
		}
		return ret;
	} else {
		ret = SUCCESS;
	}
	ResolveCtx resolve_ctx;
	CHECK(DMLResolver::resolve_stmt(ref_query, resolve_ctx));
	CHECK(ref_query->formalize());
	for (u32 i = 0; i < ref_query->select_expr_list.size(); ++i) {
		ExprStmt_s &expr = ref_query->select_expr_list[i];
		ColumnDefineStmt_s column = ColumnDefineStmt::make_column_define_stmt(expr->alias_name, expr->res_type);
		columns.push_back(column);
	}
	engine_args.push_back(view_define_sql);
	CHECK(guard->add_table(database, 
						   view_name, 
						   USER_VIEW_TABLE,
						   columns, 
						   engine_args));
	return ret;
}

u32 CMDPlan::do_drop_view()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	DropViewParam_s param = stmt->param;
	String &database = param->database;
	String &view_name = param->view_name;
	bool ignore_not_exists = param->ignore_not_exists;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	TableInfo_s info;
	ret = guard->find_table_info(database, view_name, info);
	if (FAIL(ret) && ignore_not_exists) {
		ret = SUCCESS;
		return ret;
	} else if (FAIL(ret)) {
		String msg = "view " + view_name + " not exists!";
		QUERY_CTX->set_error_msg(msg);
		ret = VIEW_NOT_EXISTS;
		return ret;
	} else if (USER_PHY_TABLE == info->type) {
		String msg = view_name + " is table not view!";
		QUERY_CTX->set_error_msg(msg);
		ret = VIEW_NOT_EXISTS;
		return ret;
	}
	CHECK(guard->del_table(database, view_name));
	return ret;
}

u32 CMDPlan::do_create_package()
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	PackageManager_s &manager = PackageManager::get_package_manager();
	CHECK(manager->add_package(stmt->param));
	return ret;
}

u32 CMDPlan::do_exec_package(ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	CMDStmt_s stmt = lex_stmt;
	CHECK(PackageExecutor::execute_function(stmt->param, query_result));
	return ret;
}

u32 CMDPlan::init_command_result_head(const Vector<String> &title, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
    for (u32 i = 0; i < title.size(); ++i) {
		query_result->add_result_title(title[i], T_VARCHAR);
	}
    return ret;
}

u32 CMDPlan::add_objects_to_result_set(Vector<Object_s> &objs, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	Row_s row = Row::make_row(objs.size());
	for (u32 i = 0; i < objs.size(); ++i) {
		row->set_cell(i, objs[i]);
	}
	query_result->add_row(row);
	return ret;
}