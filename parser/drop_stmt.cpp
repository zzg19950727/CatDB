#include "drop_stmt.h"
using namespace CatDB::Parser;

DropTableStmt::DropTableStmt()
{
}

DropTableStmt::~DropTableStmt()
{
}

Stmt::StmtType DropTableStmt::stmt_type() const
{
	return Stmt::DropTable;
}

Stmt_s DropTableStmt::make_drop_table_stmt()
{
	return Stmt_s(new DropTableStmt());
}

DropDatabaseStmt::DropDatabaseStmt()
{
}

DropDatabaseStmt::~DropDatabaseStmt()
{
}

Stmt::StmtType DropDatabaseStmt::stmt_type() const
{
	return Stmt::DropDatabase;
}

Stmt_s DropDatabaseStmt::make_drop_database_stmt()
{
	return Stmt_s(new DropDatabaseStmt);
}
