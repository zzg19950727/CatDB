#include "insert_stmt.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "error.h"
using namespace CatDB::Parser;

InsertStmt::InsertStmt()
	:DMLStmt()
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

u32 InsertStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::formalize());
	CHECK(table->formalize());
	table_ids.add_members(table->table_ids);
	if (query_values) {
		CHECK(query_values->formalize());
	}
	for (u32 i = 0; i < value_list.size(); ++i) {
		CHECK(value_list[i]->formalize());
	}
	return ret;
}

u32 InsertStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_get_stmt_exprs(exprs));
	append(exprs, value_list);
	return ret;
}