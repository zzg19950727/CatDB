#include "insert_stmt.h"
using namespace CatDB::Parser;

InsertStmt::InsertStmt()
{
}

InsertStmt::~InsertStmt()
{
}

Stmt::StmtType InsertStmt::stmt_type() const
{
	return Stmt::Insert;
}

Stmt_s InsertStmt::make_insert_stmt()
{
	return Stmt_s(new InsertStmt());
}
