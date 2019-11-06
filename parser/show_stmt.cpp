#include "show_stmt.h"
using namespace CatDB::Parser;

ShowTablesStmt::ShowTablesStmt()
{
}

ShowTablesStmt::~ShowTablesStmt()
{
}

Stmt::StmtType ShowTablesStmt::stmt_type() const
{
	return Stmt::ShowTables;
}

Stmt_s ShowTablesStmt::make_show_tables_stmt()
{
	return Stmt_s(new ShowTablesStmt());
}

ShowDatabasesStmt::ShowDatabasesStmt()
{
}

ShowDatabasesStmt::~ShowDatabasesStmt()
{
}

Stmt::StmtType ShowDatabasesStmt::stmt_type() const
{
	return Stmt::ShowDatabases;
}

Stmt_s ShowDatabasesStmt::make_show_databases_stmt()
{
	return Stmt_s(new ShowDatabasesStmt());
}

DescTableStmt::DescTableStmt()
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
