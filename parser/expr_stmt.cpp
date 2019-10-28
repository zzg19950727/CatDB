#include "expr_stmt.h"
using namespace::CatDB::Parser;

ExprStmt::ExprStmt()
{
}

ExprStmt::~ExprStmt()
{
}

Stmt::StmtType ExprStmt::stmt_type() const
{
	return Stmt::Expr;
}

ConstStmt::ConstStmt()
{
}

ConstStmt::~ConstStmt()
{
}

ExprStmt::ExprType ConstStmt::expr_stmt_type() const
{
	return ExprStmt::Const;
}

Stmt_s ConstStmt::make_const_stmt(const Object_s& value)
{
	ConstStmt* stmt = new ConstStmt();
	stmt->value = value;
	return Stmt_s(stmt);
}

ColumnStmt::ColumnStmt()
{
}

ColumnStmt::~ColumnStmt()
{
}

ExprStmt::ExprType ColumnStmt::expr_stmt_type() const
{
	return ExprStmt::Column;
}

Stmt_s ColumnStmt::make_column_stmt(const String & table, const String & column)
{
	ColumnStmt* stmt = new ColumnStmt;
	stmt->table = table;
	stmt->column = column;
	return Stmt_s(stmt);
}

Stmt_s CatDB::Parser::ColumnStmt::make_all_column_stmt()
{
	ColumnStmt* stmt = new ColumnStmt;
	stmt->table = "*";
	stmt->column = "*";
	return Stmt_s(stmt);
}

bool ColumnStmt::is_all_column() const
{
	return column == "*";
}

TableStmt::TableStmt()
{
}

TableStmt::~TableStmt()
{
}

ExprStmt::ExprType TableStmt::expr_stmt_type() const
{
	return ExprStmt::Table;
}

Stmt_s TableStmt::make_table_stmt(const String & database, const String & table_name)
{
	TableStmt* stmt = new TableStmt;
	stmt->database = database;
	stmt->table_name = table_name;
	stmt->alias_name = table_name;
	return Stmt_s(stmt);
}

Stmt_s CatDB::Parser::TableStmt::make_table_stmt(const String & table_name)
{
	TableStmt* stmt = new TableStmt;
	stmt->table_name = table_name;
	stmt->alias_name = table_name;
	return Stmt_s(stmt);
}

QueryStmt::QueryStmt()
{
}

QueryStmt::~QueryStmt()
{
}

ExprStmt::ExprType QueryStmt::expr_stmt_type() const
{
	return ExprStmt::Query;
}

Stmt_s QueryStmt::make_query_stmt()
{
	return Stmt_s(new QueryStmt());
}

ListStmt::ListStmt()
{
}

ListStmt::~ListStmt()
{
}

ExprStmt::ExprType ListStmt::expr_stmt_type() const
{
	return ExprStmt::List;
}

Stmt_s ListStmt::make_list_stmt()
{
	return Stmt_s(new ListStmt());
}

AggrStmt::AggrStmt()
{
}

AggrStmt::~AggrStmt()
{
}

ExprStmt::ExprType AggrStmt::expr_stmt_type() const
{
	return ExprStmt::Aggregate;
}

Stmt_s AggrStmt::make_aggr_stmt()
{
	return Stmt_s(new AggrStmt());
}

UnaryExprStmt::UnaryExprStmt()
{
}

UnaryExprStmt::~UnaryExprStmt()
{
}

ExprStmt::ExprType UnaryExprStmt::expr_stmt_type() const
{
	return ExprStmt::Unary;
}

Stmt_s UnaryExprStmt::make_unary_stmt()
{
	return Stmt_s(new UnaryExprStmt());
}

BinaryExprStmt::BinaryExprStmt()
{
}

BinaryExprStmt::~BinaryExprStmt()
{
}

ExprStmt::ExprType BinaryExprStmt::expr_stmt_type() const
{
	return ExprStmt::Binary;
}

Stmt_s BinaryExprStmt::make_binary_stmt()
{
	return Stmt_s(new BinaryExprStmt());
}

TernaryExprStmt::TernaryExprStmt()
{
}

TernaryExprStmt::~TernaryExprStmt()
{
}

ExprStmt::ExprType TernaryExprStmt::expr_stmt_type() const
{
	return ExprStmt::Ternary;
}

Stmt_s TernaryExprStmt::make_ternary_stmt()
{
	return Stmt_s(new TernaryExprStmt());
}
