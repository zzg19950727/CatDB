#include "delete_stmt.h"
using namespace CatDB::Parser;

DeleteStmt::DeleteStmt()
{
}

DeleteStmt::~DeleteStmt()
{
}

Stmt::StmtType DeleteStmt::stmt_type() const
{
	return Stmt::Delete;
}

Stmt_s DeleteStmt::make_delete_stmt()
{
	return Stmt_s(new DeleteStmt());
}
