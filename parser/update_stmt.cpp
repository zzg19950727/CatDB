#include "update_stmt.h"
using namespace CatDB::Parser;

UpdateStmt::UpdateStmt()
{
}

UpdateStmt::~UpdateStmt()
{
}

Stmt::StmtType UpdateStmt::stmt_type() const
{
	return Stmt::Update;
}

Stmt_s UpdateStmt::make_update_stmt()
{
	return Stmt_s(new UpdateStmt());
}
