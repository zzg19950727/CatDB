#include "schema_checker.h"
#include "delete_plan.h"
#include "delete_stmt.h"
#include "table_space.h"
#include "expression.h"
#include "expr_stmt.h"
#include "filter.h"
#include "object.h"
#include "delete.h"
#include "stmt.h"
#include "row.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Storage;

DeletePlan::DeletePlan()
{

}

DeletePlan::~DeletePlan()
{

}

Plan_s DeletePlan::make_delete_plan(const Stmt_s& lex_insert_stmt)
{
	DeletePlan* plan = new DeletePlan;
	plan->set_lex_stmt(lex_insert_stmt);
	return Plan_s(plan);
}
u32 DeletePlan::execute()
{
	if (!root_operator) {
		return PLAN_NOT_BUILD;
	}
	u32 ret;
	ret = root_operator->open();
	if (ret != SUCCESS) {
		result = Error::make_object(ret);
		return ret;
	}
	
	affect_rows_ = 0;
	Row_s row;
	while ((ret = root_operator->get_next_row(row)) == SUCCESS)
	{
		++affect_rows_;
	}
	u32 ret2 = root_operator->close();
	if (ret != NO_MORE_ROWS) {
		result = Error::make_object(ret);
		return ret;
	}
	else if (ret2 != SUCCESS) {
		result = Error::make_object(ret2);
		return ret2;
	}
	else {
		result = Error::make_object(SUCCESS);
		return SUCCESS;
	}
}

u32 DeletePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Delete)
	{
		Log(LOG_ERR, "DeletePlan", "error lex stmt when build delete plan");
		return ERROR_LEX_STMT;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	DeleteStmt* lex = dynamic_cast<DeleteStmt*>(lex_stmt.get());
	assert(lex->table);
	//构建物理算子
	TableStmt* table = dynamic_cast<TableStmt*>(lex->table.get());
	database = table->database;
	table_name = table->table_name;
	TableSpace_s table_space = TableSpace::make_table_space(table_name, database);
	assert(table_space);
	//将where子句转换为filter
	Filter_s filter;
	u32 ret = resolve_filter(lex->where_stmt, filter);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "DeletePlan", "create filter from where stmt error:%s", err_string(ret));
		return ret;
	}
	root_operator = Delete::make_delete(table_space, filter);
	if (access_columns.size() > 0) {
		RowDesc row_desc(access_columns.size());
		for (u32 i = 0; i < access_columns.size(); ++i) {
			row_desc.set_column_desc(i, access_columns[i]);
		}
		root_operator->set_access_desc(row_desc);
	}
	return SUCCESS;
}

u32 DeletePlan::optimizer()
{
	//TODO:add later
	return SUCCESS;
}

Plan::PlanType DeletePlan::type() const
{
	return Plan::DELETE;
}

u32 DeletePlan::resolve_filter(const Stmt_s& where_stmt, Filter_s& filter)
{
	if (!where_stmt) {
		filter.reset();
		return SUCCESS;
	}
	Expression_s expr;
	u32 ret = resolve_expr(where_stmt, expr);
	if (ret != SUCCESS) {
		return ret;
	}
	filter = Filter::make_filter(expr);
	return SUCCESS;
}

u32 DeletePlan::resolve_expr(const Stmt_s& stmt, Expression_s& expr)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	Expression_s first_expr, second_expr, third_expr;
	ConstStmt* const_stmt;
	ColumnStmt* column_stmt;
	ColumnDesc col_desc;
	UnaryExprStmt* unary_stmt;
	BinaryExprStmt* binary_stmt;
	TernaryExprStmt* ternary_stmt;
	u32 ret;

	switch (expr_stmt->expr_stmt_type())
	{
	case ExprStmt::Const:
		const_stmt = dynamic_cast<ConstStmt*>(expr_stmt);
		expr = ConstExpression::make_const_expression(const_stmt->value);
		ret = SUCCESS;
		break;
	case ExprStmt::Column:
		column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
		if (!checker->have_column(database, table_name, column_stmt->column)) {
			Log(LOG_ERR, "DeletePlan", "%s.%s do not have column %s",
				database.c_str(), table_name.c_str(), column_stmt->column.c_str());
			return COLUMN_NOT_EXISTS;
		}
		col_desc = checker->get_column_desc(database, table_name, column_stmt->column);
		add_access_column(col_desc);
		expr = ColumnExpression::make_column_expression(col_desc);
		ret = SUCCESS;
		break;
	case ExprStmt::Query:
		Log(LOG_ERR, "DeletePlan", "subquery in delete`s where stmt not support yet");
		ret = ERROR_LEX_STMT;
		break;
	case ExprStmt::List:
		Log(LOG_ERR, "DeletePlan", "list stmt in delete`s where stmt not support yet");
		ret = ERROR_LEX_STMT;
		break;
	case ExprStmt::Aggregate:
		Log(LOG_ERR, "DeletePlan", "aggregate stmt in delete`s where stmt not support");
		ret = ERROR_LEX_STMT;
		break;
	case ExprStmt::Unary:
		unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		ret = resolve_expr(unary_stmt->expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "DeletePlan", "create unary expression`s first expression failed");
			break;
		}
		expr = UnaryExpression::make_unary_expression(first_expr, unary_stmt->op_type);
		ret = SUCCESS;
		break;
	case ExprStmt::Binary:
		binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		ret = resolve_expr(binary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "DeletePlan", "create binary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(binary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "DeletePlan", "create binary expression`s second expression failed");
			break;
		}
		expr = BinaryExpression::make_binary_expression(first_expr, second_expr, binary_stmt->op_type);
		ret = SUCCESS;
		break;
	case ExprStmt::Ternary:
		ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		ret = resolve_expr(ternary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "DeletePlan", "create ternary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "DeletePlan", "create ternary expression`s second expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->third_expr_stmt, third_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "DeletePlan", "create ternary expression`s third expression failed");
			break;
		}
		expr = TernaryExpression::make_ternary_expression(first_expr, second_expr, third_expr, ternary_stmt->op_type);
		ret = SUCCESS;
		break;
	default:
		Log(LOG_ERR, "DeletePlan", "unknown expr stmt in delete`s where stmt");
		ret = ERROR_LEX_STMT;
	}
	return ret;
}

void DeletePlan::add_access_column(const ColumnDesc & col_desc)
{
	bool find = false;
	for (u32 i = 0; i < access_columns.size(); ++i) {
		if (access_columns[i] == col_desc) {
			find = true;
			break;
		}
	}
	if (!find) {
		access_columns.push_back(col_desc);
	}
}
