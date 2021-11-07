#include "delete_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "error.h"
using namespace CatDB::Parser;

DeleteStmt::DeleteStmt()
	:DMLStmt()
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

u32 DeleteStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::formalize());
	CHECK(row_id_col->formalize());
	return ret;
}

u32 DeleteStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_get_stmt_exprs(exprs));
	exprs.push_back(row_id_col);
	return ret;
}

u32 DeleteStmt::inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   		 const Vector<ExprStmt_s> &new_exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_replace_stmt_exprs(old_exprs, new_exprs));
	CHECK(table->replace_exprs(old_exprs, new_exprs));
	ExprStmt_s row_id = row_id_col;
	CHECK(ExprUtils::replace_expr(old_exprs, new_exprs, row_id));
	row_id_col = row_id;
	return ret;
}