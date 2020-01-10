#include "show_stmt.h"
using namespace CatDB::Parser;

ShowTablesStmt::ShowTablesStmt(const String& database)
	:database(database)
{
}

ShowTablesStmt::~ShowTablesStmt()
{
}

Stmt::StmtType ShowTablesStmt::stmt_type() const
{
	return Stmt::ShowTables;
}

Stmt_s ShowTablesStmt::make_show_tables_stmt(const String& database)
{
	return Stmt_s(new ShowTablesStmt(database));
}

ShowDatabasesStmt::ShowDatabasesStmt(bool is_select_current_database)
	:is_select_current_database(is_select_current_database)
{
}

ShowDatabasesStmt::~ShowDatabasesStmt()
{

}

Stmt::StmtType ShowDatabasesStmt::stmt_type() const
{
	return Stmt::ShowDatabases;
}

Stmt_s ShowDatabasesStmt::make_show_databases_stmt(bool is_select_current_database)
{
	return Stmt_s(new ShowDatabasesStmt(is_select_current_database));
}

DescTableStmt::DescTableStmt()
	:is_show_table_statis(false),
	is_show_column_statis(false)
{
}

DescTableStmt::~DescTableStmt()
{
}

Stmt::StmtType DescTableStmt::stmt_type() const
{
	return Stmt::DescTable;
}

Stmt_s DescTableStmt::make_desc_table_stmt()
{
	return Stmt_s(new DescTableStmt());
}

UseDatabaseStmt::UseDatabaseStmt()
{
}

UseDatabaseStmt::~UseDatabaseStmt()
{
}

Stmt::StmtType UseDatabaseStmt::stmt_type() const
{
	return Stmt::UseDatabase;
}

Stmt_s UseDatabaseStmt::make_use_database_stmt()
{
	return Stmt_s(new UseDatabaseStmt);
}

AnalyzeStmt::AnalyzeStmt()
{
}

AnalyzeStmt::~AnalyzeStmt()
{
}

Stmt::StmtType AnalyzeStmt::stmt_type() const
{
	return Stmt::Analyze;
}

Stmt_s AnalyzeStmt::make_analyze_stmt()
{
	return Stmt_s(new AnalyzeStmt());
}
