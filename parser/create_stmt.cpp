#include "create_stmt.h"

using namespace CatDB::Parser;

ColumnDefineStmt::ColumnDefineStmt()
	:data_type(NUMBER)
{
}

ColumnDefineStmt::~ColumnDefineStmt()
{
}

Stmt::StmtType ColumnDefineStmt::stmt_type() const
{
	return Stmt::ColumnDefine;
}

Stmt_s ColumnDefineStmt::make_column_define_stmt(const String & name, u32 data_type)
{
	ColumnDefineStmt* stmt = new ColumnDefineStmt();
	stmt->column_name = name;
	stmt->data_type = data_type;
	return Stmt_s(stmt);
}

CreateTableStmt::CreateTableStmt()
{
}

CreateTableStmt::~CreateTableStmt()
{
}

Stmt::StmtType CreateTableStmt::stmt_type() const
{
	return Stmt::CreateTable;
}

Stmt_s CreateTableStmt::make_create_table_stmt()
{
	return Stmt_s(new CreateTableStmt());
}

CreateDatabaseStmt::CreateDatabaseStmt()
{
}

CreateDatabaseStmt::~CreateDatabaseStmt()
{
}

Stmt::StmtType CreateDatabaseStmt::stmt_type() const
{
	return Stmt::CreateDatabase;
}

Stmt_s CreateDatabaseStmt::make_create_database_stmt()
{
	return Stmt_s(new CreateDatabaseStmt);
}
