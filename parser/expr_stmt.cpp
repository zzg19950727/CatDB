#include "expr_stmt.h"
#include "object.h"
using namespace::CatDB::Parser;
using namespace::CatDB::Common;

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

String ExprStmt::op_string(OperationType op_type)
{
	switch (op_type) {
		case OP_INVALID: return String(VAR_NAME(OP_INVALID));
		case OP_ADD: return String(VAR_NAME(OP_ADD));
		case OP_SUB: return String(VAR_NAME(OP_SUB));
		case OP_MUL: return String(VAR_NAME(OP_MUL));
		case OP_DIV: return String(VAR_NAME(OP_DIV));
		case OP_EQ: return String(VAR_NAME(OP_EQ));
		case OP_ANTI_EQ: return String(VAR_NAME(OP_ANTI_EQ));
		case OP_NE: return String(VAR_NAME(OP_NE));
		case OP_GE: return String(VAR_NAME(OP_GE));
		case OP_GT: return String(VAR_NAME(OP_GT));
		case OP_LE: return String(VAR_NAME(OP_LE));
		case OP_LT: return String(VAR_NAME(OP_LT));
		case OP_BETWEEN: return String(VAR_NAME(OP_BETWEEN));
		case OP_NOT_BETWEEN: return String(VAR_NAME(OP_NOT_BETWEEN));
		case OP_IS_NULL: return String(VAR_NAME(OP_IS_NULL));
		case OP_IS_NOT_NULL: return String(VAR_NAME(OP_IS_NOT_NULL));
		case OP_IN: return String(VAR_NAME(OP_IN));
		case OP_NOT_IN: return String(VAR_NAME(OP_NOT_IN));
		case OP_EXISTS: return String(VAR_NAME(OP_EXISTS));
		case OP_NOT_EXISTS: return String(VAR_NAME(OP_NOT_EXISTS));
		case OP_AND: return String(VAR_NAME(OP_AND));
		case OP_OR: return String(VAR_NAME(OP_OR));
		case OP_NOT: return String(VAR_NAME(OP_NOT));
		case OP_UNION: return String(VAR_NAME(OP_UNION));
		case OP_UNION_ALL: return String(VAR_NAME(OP_UNION_ALL));
		case OP_INTERSECT: return String(VAR_NAME(OP_INTERSECT));
		case OP_EXCEPT: return String(VAR_NAME(OP_EXCEPT));
		case OP_MINUS: return String(VAR_NAME(OP_MINUS));
		case OP_LIKE: return String(VAR_NAME(OP_LIKE));
		case OP_NOT_LIKE: return String(VAR_NAME(OP_NOT_LIKE));
		defualt: return String("unknown");
	}
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

String ConstStmt::to_string() const
{
	if (value) {
		return value->to_string();
	}
	else {
		return "";
	}
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

String ColumnStmt::to_string() const
{
	if (table.size() && table != "*")
		return table + "." + column;
	else
		return column;
}

TableStmt::TableStmt()
	:join_type(0),
	select_rows(0),
	is_tmp_table(false)
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
	stmt->is_tmp_table = false;
	return Stmt_s(stmt);
}

String TableStmt::to_string() const
{
	return database + "." + table_name;
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

String QueryStmt::to_string() const
{
	return DateTime::CurrentDatetime();
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

String ListStmt::to_string() const
{
	return DateTime::CurrentDatetime();
}

AggrStmt::AggrStmt()
	:distinct(false)
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

String AggrStmt::to_string() const
{
	String func = aggr_func_name();
	String distinct_str;
	if (distinct) {
		distinct_str = "DISTINCT ";
	}
	ExprStmt* expr = dynamic_cast<ExprStmt*>(aggr_expr.get());
	String expr_str;
	if (expr) {
		expr_str = expr->to_string();
	}
	return func + "(" + distinct_str + expr_str + ")";
}

String AggrStmt::aggr_func_name()const
{
	String func;
	switch (aggr_func)
	{
	case AggrStmt::COUNT:
		func = "COUNT";
		break;
	case AggrStmt::SUM:
		func = "SUM";
		break;
	case AggrStmt::AVG:
		func = "AVG";
		break;
	case AggrStmt::MIN:
		func = "MIN";
		break;
	case AggrStmt::MAX:
		func = "MAX";
		break;
	default:
		func = "unknown";
	}
	return func;
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

String UnaryExprStmt::to_string() const
{
	String str1, op_str;
	ExprStmt* expr1 = dynamic_cast<ExprStmt*>(expr_stmt.get());
	if (expr1) {
		str1 = expr1->to_string();
	}

	if (op_type == ExprStmt::OP_NOT) {
		op_str = "NOT ";
		return op_str + str1;
	}
	else if (op_type == ExprStmt::OP_IS_NULL) {
		op_str = " IS NULL";
		return str1 + op_str;
	}
	else if (op_type == ExprStmt::OP_IS_NOT_NULL) {
		op_str = " IS NOT NULL";
		return str1 + op_str;
	}
	else {
		return str1;
	}
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

String BinaryExprStmt::to_string() const
{
	String str1, str2,op_str;
	ExprStmt* expr1 = dynamic_cast<ExprStmt*>(first_expr_stmt.get());
	if (expr1) {
		str1 = expr1->to_string();
	}
	ExprStmt* expr2 = dynamic_cast<ExprStmt*>(second_expr_stmt.get());
	if (expr2) {
		str2 = expr2->to_string();
	}

	switch (op_type)
	{
		case OP_ADD:
			op_str = "+";
			break;
		case OP_SUB:
			op_str = "-";
			break;
		case OP_MUL:
			op_str = "*";
			break;
		case OP_DIV:
			op_str = "/";
			break;
		case OP_EQ:
			op_str = "=";
			break;
		case OP_NE:
			op_str = "!=";
			break;
		case OP_GE:
			op_str = ">=";
			break;
		case OP_GT:
			op_str = ">";
			break;
		case OP_LE:
			op_str = "<=";
			break;
		case OP_LT:
			op_str = "<";
			break;
		case OP_AND:
			op_str = " and ";
			break;
		case OP_OR:
			op_str = " or ";
			break;
		case OP_LIKE:
			op_str = " like ";
			break;
		case OP_NOT_LIKE:
			op_str = " not like ";
			break;
	}
	return str1 + op_str + str2;
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

String TernaryExprStmt::to_string() const
{
	String str1, str2, str3, op_str;
	ExprStmt* expr1 = dynamic_cast<ExprStmt*>(first_expr_stmt.get());
	if (expr1) {
		str1 = expr1->to_string();
	}
	ExprStmt* expr2 = dynamic_cast<ExprStmt*>(second_expr_stmt.get());
	if (expr2) {
		str2 = expr2->to_string();
	}
	ExprStmt* expr3 = dynamic_cast<ExprStmt*>(third_expr_stmt.get());
	if (expr3) {
		str3 = expr3->to_string();
	}

	if (op_type == ExprStmt::OP_BETWEEN) {
		op_str = "between";
	}
	else if (op_type == ExprStmt::OP_NOT_BETWEEN) {
		op_str = "not between";
	}
	return str1 + op_str + str2 + "and" + str3;
}
