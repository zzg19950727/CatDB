#include "insert_stmt.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "error.h"
using namespace CatDB::Parser;

DEFINE_KV_STRING(InsertStmt,
	KV(stmt_type, StmtTypeString[stmt_type()]),
	K(is_explain),
	K(stmt_hint),
	K(table),
	K(value_list),
	K(query_values)
);

InsertStmt::InsertStmt()
	:DMLStmt()
{
}

InsertStmt::~InsertStmt()
{
}

StmtType InsertStmt::stmt_type() const
{
	return Insert;
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
		for (u32 j = 0; j < value_list[i].size(); ++j) {
			CHECK(value_list[i][j]->formalize());
		}
	}
	CHECK(deduce_value_type());
	return ret;
}

u32 InsertStmt::deduce_value_type()
{
	u32 ret = SUCCESS;
	if (query_values) {

	} else {

	}
	return ret;
}

u32 InsertStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_get_stmt_exprs(exprs));
	for (u32 i = 0; i < value_list.size(); ++i) {
		append(exprs, value_list[i]);
	}
	return ret;
}

u32 InsertStmt::inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   		 const Vector<ExprStmt_s> &new_exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_replace_stmt_exprs(old_exprs, new_exprs));
	CHECK(table->replace_exprs(old_exprs, new_exprs));
	if (query_values) {
		CHECK(query_values->replace_stmt_exprs(old_exprs, new_exprs));
	} else {
		for (u32 i = 0; i < value_list.size(); ++i) {
			CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, value_list[i]));
		}
	}
	return ret;
}