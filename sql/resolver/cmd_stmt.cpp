#include "cmd_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "stmt.h"
#include "log.h"

using namespace CatDB::Parser;
using namespace CatDB::Common;

CMDStmt::CMDStmt()
{
    cmd_type = NONE;
}

CMDStmt::~CMDStmt()
{
}

StmtType CMDStmt::stmt_type() const
{
	return DoCMD;
}

Stmt_s CMDStmt::make_cmd_stmt(CMDType cmd_type)
{
	CMDStmt* stmt = new CMDStmt();
	stmt->cmd_type = cmd_type;
	return Stmt_s(stmt);
}

u32 CMDStmt::get_create_table_params(String &database, 
									 String &table, 
									 Vector<ColumnDefineStmt_s> &columns,
									 Vector<String> &engine_args)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cmd_type == CreateTable,
				params.create_table_params.table);
	BasicTableStmt_s &table_stmt = params.create_table_params.table;
	database = table_stmt->database;
	table = table_stmt->table_name;
	columns = params.create_table_params.column_define_list;
	engine_args = params.create_table_params.engine_args;
	return ret;
}

u32 CMDStmt::get_drop_table_params(String &database, String &table, bool &ignore_not_exists)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cmd_type == DropTable,
				params.drop_table_params.table);
	BasicTableStmt_s &table_stmt = params.drop_table_params.table;
	database = table_stmt->database;
	table = table_stmt->table_name;
	ignore_not_exists = params.drop_table_params.ignore_not_exists;
	return ret;
}

u32 CMDStmt::get_create_database_params(String &database)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CreateDatabase);
	database = params.create_database_params.database;
	return ret;
}

u32 CMDStmt::get_drop_database_params(String &database, bool &ignore_not_exists)
{
    u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == DropDatabase);
	database = params.drop_database_params.database;
	ignore_not_exists = params.drop_database_params.ignore_not_exists;
	return ret;
}

u32 CMDStmt::get_show_tables_params(String &database)
{
    u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == ShowTables);
	database = params.show_tables_params.database;
	return ret;
}

u32 CMDStmt::get_show_databases_params(bool &is_select_current_database)
{
    u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == ShowDatabases);
	is_select_current_database = params.show_databases_params.is_select_current_database;
	return ret;
}

u32 CMDStmt::get_desc_table_params(String &database, 
								   String &table, 
								   bool &is_show_table_statis, 
								   bool &is_show_column_statis)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cmd_type == DescTable,
				params.desc_table_params.table);
	BasicTableStmt_s &table_stmt = params.desc_table_params.table;
	database = table_stmt->database;
	table = table_stmt->table_name;
	is_show_column_statis = params.desc_table_params.is_show_column_statis;
	is_show_table_statis = params.desc_table_params.is_show_table_statis;
	return ret;
}

u32 CMDStmt::get_use_database_params(String &database)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == UseDatabase);
	database = params.use_database_params.database;
	return ret;
}

u32 CMDStmt::get_analyze_params(String &database, String &table, double &sample_size)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == Analyze);
	database = params.analyze_params.database;
    table = params.analyze_params.table;
	sample_size = params.analyze_params.sample_size;
	return ret;
}

u32 CMDStmt::get_set_var_params(String &var_name, String &var_value)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == SetVar);
	var_name = params.set_var_params.var_name;
    var_value = params.set_var_params.var_value;
	return ret;
}

u32 CMDStmt::get_kill_params(int &thread_id)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == Kill);
	thread_id = params.kill_params.thread_id;
	return ret;
}

u32 CMDStmt::get_create_view_params(String &database,
									String &view_name,
                                    Vector<String> &column_define,
                                    String &view_define_sql,
                                    SelectStmt_s &ref_query)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CreateView);
	database = params.create_view_params.database;
	view_name = params.create_view_params.view_name;
	column_define = params.create_view_params.column_define;
	view_define_sql = params.create_view_params.view_define_sql;
	ref_query = params.create_view_params.ref_query;
	return ret;
}

u32 CMDStmt::get_drop_view_params(String &database,
								  String &view_name, 
								  bool &ignore_not_exists)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == DropView);
	database = params.drop_view_params.database;
	view_name = params.drop_view_params.view_name;
	ignore_not_exists = params.drop_view_params.ignore_not_exists;
	return ret;
}

u32 CMDStmt::formalize()
{
	return SUCCESS;
}