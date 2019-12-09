#include "schema_checker.h"
#include "insert_plan.h"
#include "insert_stmt.h"
#include "expr_stmt.h"
#include "expression.h"
#include "object.h"
#include "insert.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;

InsertPlan::InsertPlan()
{

}

InsertPlan::~InsertPlan()
{

}

Plan_s InsertPlan::make_insert_plan(const Stmt_s& lex_insert_stmt)
{
	InsertPlan* plan = new InsertPlan;
	plan->set_lex_stmt(lex_insert_stmt);
	return Plan_s(plan);
}

u32 InsertPlan::execute()
{
	if (!root_operator) {
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	u32 ret;
	ret = root_operator->open();
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	for (u32 i = 0; i < values.size(); ++i) {
		ret = root_operator->get_next_row(values[i]);
		if (ret != SUCCESS) {
			set_error_code(ret);
			break;
		}
	}
	
	u32 ret2 = root_operator->close();
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	else if (ret2 != SUCCESS) {
		set_error_code(ret2);
		return ret2;
	}
	else {
		affect_rows_ = values.size();
		set_error_code(SUCCESS);
		return SUCCESS;
	}
}

u32 InsertPlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Insert)
	{
		Log(LOG_ERR, "InsertPlan", "error lex stmt when build insert plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	InsertStmt* lex = dynamic_cast<InsertStmt*>(lex_stmt.get());
	assert(lex->table);
	assert(lex->values);
	//构建物理算子
	TableStmt* table = dynamic_cast<TableStmt*>(lex->table.get());
	const String& database = table->database;
	const String& table_name = table->table_name;
	root_operator = Insert::make_insert(database, table_name);
	//将解析的value转换为Row
	assert(lex->values->stmt_type() == Stmt::Expr);
	ExprStmt* expr = dynamic_cast<ExprStmt*>(lex->values.get());
	assert(expr->expr_stmt_type() == ExprStmt::List);
	ListStmt* list = dynamic_cast<ListStmt*>(expr);
	RowDesc row_desc;
	u32 ret = checker->get_row_desc(database, table_name, row_desc);
	if (ret != SUCCESS) {
		set_error_code(ret);
		return ret;
	}
	for (u32 i = 0; i < list->stmt_list.size(); ++i){
		u32 ret = check_column_value(list->stmt_list[i], row_desc);
		if (ret) {
			Log(LOG_ERR, "InsertPlan", "column value check failed in values:%u, column:%u", i, ret);
			set_error_code(ERROR_COLUMN_VALUE);
			return ERROR_COLUMN_VALUE;
		}
		Row_s row;
		ret = resolve_row(list->stmt_list[i], row_desc, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create row error:%s", err_string(ret));
			set_error_code(ret);
			return ret;
		}
		values.push_back(row);
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 InsertPlan::optimizer()
{
	//add later
	return SUCCESS;
}

Plan::PlanType InsertPlan::type() const
{
	return Plan::INSERT;
}

u32 InsertPlan::check_column_value(const Stmt_s& list, const RowDesc& row_desc)const
{
	if (list->stmt_type() != Stmt::Expr) {
		return 1;
	}
	else {
		ExprStmt* expr = dynamic_cast<ExprStmt*>(list.get());
		if (expr->expr_stmt_type() != ExprStmt::List) {
			return 1;
		}
		else {
			ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr);
			for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
				if (list_stmt->stmt_list[i]->stmt_type() == Stmt::Expr){
					ExprStmt* expr = dynamic_cast<ExprStmt*>(list_stmt->stmt_list[i].get());
					if (expr->expr_stmt_type() != ExprStmt::Const) {
						return i;
					}else{
						continue;
					}
				}else {
					return i;
				}
			}
			return list_stmt->stmt_list.size() != row_desc.get_column_num();
		}
	}
}

u32 InsertPlan::resolve_expr(const Stmt_s& stmt, Expression_s& expr)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	u32 ret;

	switch (expr_stmt->expr_stmt_type())
	{
	case ExprStmt::Const:
	{
		ConstStmt* const_stmt = dynamic_cast<ConstStmt*>(expr_stmt);
		expr = ConstExpression::make_const_expression(const_stmt->value);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Unary:
	{
		UnaryExprStmt* unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		Expression_s first_expr;
		ret = resolve_expr(unary_stmt->expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create unary expression`s first expression failed");
			break;
		}
		expr = UnaryExpression::make_unary_expression(first_expr, unary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Binary:
	{
		BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		Expression_s first_expr, second_expr;
		ret = resolve_expr(binary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create binary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(binary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create binary expression`s second expression failed");
			break;
		}
		expr = BinaryExpression::make_binary_expression(first_expr, second_expr, binary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Ternary:
	{
		Expression_s first_expr, second_expr, third_expr;
		TernaryExprStmt* ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		ret = resolve_expr(ternary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create ternary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create ternary expression`s second expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->third_expr_stmt, third_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create ternary expression`s third expression failed");
			break;
		}
		expr = TernaryExpression::make_ternary_expression(first_expr, second_expr, third_expr, ternary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	default:
		Log(LOG_ERR, "InsertPlan", "unknown expr stmt in insert`s where stmt");
		ret = ERROR_LEX_STMT;
	}
	return ret;
}

u32 InsertPlan::resolve_row(const Stmt_s& list, const RowDesc& row_desc, Row_s& row)
{
	ColumnDesc col_desc;
	row = Row::make_row(row_desc);
	ExprStmt* expr = dynamic_cast<ExprStmt*>(list.get());
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr);
	for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
		Expression_s expr;
		u32 ret = resolve_expr(list_stmt->stmt_list[i], expr);
		if (ret != SUCCESS) {
			return ret;
		}
		Object_s cell = expr->get_result(row);
		ret = row_desc.get_column_desc(i, col_desc);
		if (ret != SUCCESS) {
			return ret;
		}
		ret = cast_to(col_desc.get_data_type(), cell);
		if (ret != SUCCESS) {
			return ret;
		}
		row->set_cell(i, cell);
	}
	return SUCCESS;
}