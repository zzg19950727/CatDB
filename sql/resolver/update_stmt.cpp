#include "update_stmt.h"
#include "expr_stmt.h"
#include "error.h"
using namespace CatDB::Parser;

UpdateStmt::UpdateStmt()
	:DMLStmt()
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

u32 UpdateStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::formalize());
	CHECK(table->formalize());
	table_ids.add_members(table->table_ids);
	CHECK(row_id_col->formalize());
	for (u32 i = 0; i < update_assign_stmt.size(); ++i) {
		CHECK(update_assign_stmt[i]->formalize());
	}
	return ret;
}

u32 UpdateStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_get_stmt_exprs(exprs));
	append(exprs, update_assign_stmt);
	exprs.push_back(row_id_col);
	return ret;
}