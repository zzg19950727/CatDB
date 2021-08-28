#include "cmd_stmt.h"
#include "expr_stmt.h"
#include "stmt.h"
#include "log.h"

using namespace CatDB::Parser;

ColumnDefineStmt::ColumnDefineStmt()
{

}

ColumnDefineStmt::~ColumnDefineStmt()
{

}

ColumnDefineStmt_s ColumnDefineStmt::make_column_define_stmt(const String& name, u32 data_type)
{
	ColumnDefineStmt *stmt = new ColumnDefineStmt();
	stmt->column_name = name;
	stmt->data_type = data_type;
	return ColumnDefineStmt_s(stmt);
}

CMDStmt::CMDStmt()
{
    cmd_type = CMDStmt::NONE;
}

CMDStmt::~CMDStmt()
{
}

Stmt::StmtType CMDStmt::stmt_type() const
{
	return Stmt::DoCMD;
}

Stmt_s CMDStmt::make_cmd_stmt(CMDType cmd_type)
{
	CMDStmt* stmt = new CMDStmt();
	stmt->cmd_type = cmd_type;
	return Stmt_s(stmt);
}

u32 CMDStmt::get_create_table_params(String &database, String &table, Vector<Pair<String, String>> &columns)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cmd_type == CMDStmt::CreateTable,
				params.create_table_params.table);
	BasicTableStmt_s &table_stmt = params.create_table_params.table;
	database = table_stmt->database;
	table = table_stmt->table_name;
	Vector<ColumnDefineStmt_s> &list = params.create_table_params.column_define_list;
	for (u32 i = 0; i < list.size(); ++i) {
		ColumnDefineStmt_s &column_define = list[i];
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
			return ERROR_LEX_STMT;
		}
		columns.push_back(Pair<String, String>(column_define->column_name, type));
	}
	return ret;
}

u32 CMDStmt::get_drop_table_params(String &database, String &table)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cmd_type == CMDStmt::DropTable,
				params.drop_table_params.table);
	BasicTableStmt_s &table_stmt = params.drop_table_params.table;
	database = table_stmt->database;
	table = table_stmt->table_name;
	return ret;
}

u32 CMDStmt::get_create_database_params(String &database)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CMDStmt::CreateDatabase);
	database = params.create_database_params.database;
	return ret;
}

u32 CMDStmt::get_drop_database_params(String &database)
{
    u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CMDStmt::DropDatabase);
	database = params.drop_database_params.database;
	return ret;
}

u32 CMDStmt::get_show_tables_params(String &database)
{
    u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CMDStmt::ShowTables);
	database = params.show_tables_params.database;
	return ret;
}

u32 CMDStmt::get_show_databases_params(bool &is_select_current_database)
{
    u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CMDStmt::ShowDatabases);
	is_select_current_database = params.show_databases_params.is_select_current_database;
	return ret;
}

u32 CMDStmt::get_desc_table_params(String &database, String &table, bool &is_show_table_statis, bool &is_show_column_statis)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cmd_type == CMDStmt::DescTable,
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
    MY_ASSERT(cmd_type == CMDStmt::UseDatabase);
	database = params.use_database_params.database;
	return ret;
}

u32 CMDStmt::get_analyze_params(String &database, String &table, double &sample_size)
{
	u32 ret = SUCCESS;
    MY_ASSERT(cmd_type == CMDStmt::Analyze);
	database = params.analyze_params.database;
    table = params.analyze_params.table;
	sample_size = params.analyze_params.sample_size;
	return ret;
}

String CMDStmt::get_cmd_type()const
{
	switch(cmd_type) {
		case NONE: return String(VAR_NAME(NONE));
		case CreateTable: return String(VAR_NAME(CreateTable));
		case DropTable: return String(VAR_NAME(DropTable));
		case CreateDatabase: return String(VAR_NAME(CreateDatabase));
		case DropDatabase: return String(VAR_NAME(DropDatabase));
		case ShowTables: return String(VAR_NAME(ShowTables));
		case ShowDatabases: return String(VAR_NAME(ShowDatabases));
		case DescTable: return String(VAR_NAME(DescTable));
		case UseDatabase: return String(VAR_NAME(UseDatabase));
		case Analyze: return String(VAR_NAME(Analyze));
		default: return String("UNKNOWN");
	}
}

u32 CMDStmt::formalize()
{
	return SUCCESS;
}