#include "cmd_stmt.h"
#include "expr_stmt.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Parser;

ColumnDefineStmt::ColumnDefineStmt()
{

}

ColumnDefineStmt::~ColumnDefineStmt()
{

}
Stmt::StmtType ColumnDefineStmt::stmt_type() const
{
	return Stmt::ColumnDefine;
}

Stmt_s ColumnDefineStmt::make_column_define_stmt(const String& name, u32 data_type)
{
	ColumnDefineStmt *stmt = new ColumnDefineStmt();
	stmt->column_name = name;
	stmt->data_type = data_type;
	return Stmt_s(stmt);
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
    if (cmd_type != CMDStmt::CreateTable)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	if (params.create_table_params.table->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(params.create_table_params.table.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Table) {
		return ERROR_LEX_STMT;
	}
	TableStmt* table_stmt = dynamic_cast<TableStmt*>(expr_stmt);
	database = table_stmt->database;
	table = table_stmt->table_name;

	if (params.create_table_params.column_define_list->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	expr_stmt = dynamic_cast<ExprStmt*>(params.create_table_params.column_define_list.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		return ERROR_LEX_STMT;
	}
	ListStmt* list = dynamic_cast<ListStmt*>(expr_stmt);
	for (u32 i = 0; i < list->stmt_list.size(); ++i) {
		if (list->stmt_list[i]->stmt_type() != Stmt::ColumnDefine) {
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
			return ERROR_LEX_STMT;
		}
		columns.push_back(Pair<String, String>(column_define->column_name, type));
	}
	return SUCCESS;
}

u32 CMDStmt::get_drop_table_params(String &database, String &table)
{
    if (cmd_type != CMDStmt::DropTable)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	if (params.drop_table_params.table->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(params.drop_table_params.table.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Table) {
		return ERROR_LEX_STMT;
	}
	TableStmt* table_stmt = dynamic_cast<TableStmt*>(expr_stmt);
	database = table_stmt->database;
	table = table_stmt->table_name;
	return SUCCESS;
}

u32 CMDStmt::get_create_database_params(String &database)
{
    if (cmd_type != CMDStmt::CreateDatabase)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	database = params.create_database_params.database;
	return SUCCESS;
}

u32 CMDStmt::get_drop_database_params(String &database)
{
    if (cmd_type != CMDStmt::DropDatabase)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	database = params.drop_database_params.database;
	return SUCCESS;
}

u32 CMDStmt::get_show_tables_params(String &database)
{
    if (cmd_type != CMDStmt::ShowTables)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	database = params.show_tables_params.database;
	return SUCCESS;
}

u32 CMDStmt::get_show_databases_params(bool &is_select_current_database)
{
    if (cmd_type != CMDStmt::ShowDatabases)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	is_select_current_database = params.show_databases_params.is_select_current_database;
	return SUCCESS;
}

u32 CMDStmt::get_desc_table_params(String &database, String &table, bool &is_show_table_statis, bool &is_show_column_statis)
{
    if (cmd_type != CMDStmt::DescTable)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	if (params.desc_table_params.table->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(params.desc_table_params.table.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Table) {
		return ERROR_LEX_STMT;
	}
	TableStmt* table_stmt = dynamic_cast<TableStmt*>(expr_stmt);
	database = table_stmt->database;
	table = table_stmt->table_name;
	is_show_column_statis = params.desc_table_params.is_show_column_statis;
	is_show_table_statis = params.desc_table_params.is_show_table_statis;
	return SUCCESS;
}

u32 CMDStmt::get_use_database_params(String &database)
{
    if (cmd_type != CMDStmt::UseDatabase)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	database = params.use_database_params.database;
	return SUCCESS;
}

u32 CMDStmt::get_analyze_params(String &database, String &table, double &sample_size)
{
    if (cmd_type != CMDStmt::Analyze)
	{
		Log(LOG_ERR, "CreateTablePlan", "error lex stmt");
		return ERROR_LEX_STMT;
	}
	database = params.analyze_params.database;
    table = params.analyze_params.table;
    sample_size = params.analyze_params.sample_size;
	return SUCCESS;
}