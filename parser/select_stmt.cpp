#include "select_stmt.h"
using namespace::CatDB::Parser;

SelectStmt::SelectStmt()
	:is_distinct(false),
	asc_desc(true)
{
}

SelectStmt::~SelectStmt()
{
}

Stmt::StmtType SelectStmt::stmt_type() const
{
	return Stmt::Select;
}

Stmt_s SelectStmt::make_select_stmt()
{
	return Stmt_s(new SelectStmt());
}

LimitStmt::LimitStmt()
	:limit_offset(0),
	limit_size(0)
{
}

LimitStmt::~LimitStmt()
{
}

Stmt::StmtType LimitStmt::stmt_type() const
{
	return Stmt::Limit;
}

Stmt_s LimitStmt::make_limit_stmt(u32 size, u32 offset)
{
	LimitStmt* stmt = new LimitStmt;
	stmt->limit_offset = offset;
	stmt->limit_size = size;
	return Stmt_s(stmt);
}
