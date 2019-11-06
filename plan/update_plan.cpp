#include "schema_checker.h"
#include "select_plan.h"
#include "update_plan.h"
#include "update_stmt.h"
#include "expression.h"
#include "expr_stmt.h"
#include "filter.h"
#include "object.h"
#include "update.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;

UpdatePlan::UpdatePlan()
{

}

UpdatePlan::~UpdatePlan()
{

}

Plan_s UpdatePlan::make_update_plan(const Stmt_s& lex_insert_stmt)
{
	UpdatePlan* plan = new UpdatePlan;
	plan->set_lex_stmt(lex_insert_stmt);
	return Plan_s(plan);
}
u32 UpdatePlan::execute()
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

	affect_rows_ = 0;
	Row_s row;
	while ((ret = root_operator->get_next_row(row)) == SUCCESS)
	{
		++affect_rows_;
	}
	u32 ret2 = root_operator->close();
	if (ret != NO_MORE_ROWS) {
		set_error_code(ret);
		return ret;
	}
	else if (ret2 != SUCCESS) {
		set_error_code(ret2);
		return ret2;
	}
	else {
		set_error_code(SUCCESS);
		return SUCCESS;
	}
}

u32 UpdatePlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Update)
	{
		Log(LOG_ERR, "UpdatePlan", "error lex stmt when build update plan");
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	UpdateStmt* lex = dynamic_cast<UpdateStmt*>(lex_stmt.get());
	assert(lex->table);
	//构建物理算子
	table = dynamic_cast<TableStmt*>(lex->table.get());
	database = table->database;
	table_name = table->table_name;
	//将where子句转换为filter
	Filter_s filter;
	u32 ret = resolve_filter(lex->where_stmt, filter);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "UpdatePlan", "create filter from where stmt error:%s", err_string(ret));
		set_error_code(ret);
		return ret;
	}
	Row_s new_row;
	ret = resolve_update_row(lex->update_asgn_stmt, new_row);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "UpdatePlan", "create update row from asign stmt error:%s", err_string(ret));
		set_error_code(ret);
		return ret;
	}
	root_operator = Update::make_update(database, table_name, new_row, filter);
	if (access_columns.size() > 0) {
		RowDesc row_desc(access_columns.size());
		for (u32 i = 0; i < access_columns.size(); ++i) {
			row_desc.set_column_desc(i, access_columns[i]);
		}
		root_operator->set_access_desc(row_desc);
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 UpdatePlan::optimizer()
{
	//TODO:add later
	return SUCCESS;
}

Plan::PlanType UpdatePlan::type() const
{
	return Plan::DELETE;
}

u32 UpdatePlan::resolve_filter(const Stmt_s& where_stmt, Filter_s& filter)
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

u32 UpdatePlan::resolve_expr(const Stmt_s& stmt, Expression_s& expr)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());

	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	
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
			Log(LOG_ERR, "UpdatePlan", "%s.%s do not have column %s", 
				database.c_str(), table_name.c_str(), column_stmt->column.c_str());
			return COLUMN_NOT_EXISTS;
		}
		ret = checker->get_column_desc(database, table_name, column_stmt->column, col_desc);
		if (ret != SUCCESS) {
			break;
		}
		add_access_column(col_desc);
		expr = ColumnExpression::make_column_expression(col_desc);
		ret = SUCCESS;
		break;
	case ExprStmt::Query:
	{
		QueryStmt* query_stmt = dynamic_cast<QueryStmt*>(expr_stmt);
		Vector<TableStmt*> parent_table_list;
		Vector<TableStmt*> table_list;
		table_list.push_back(table);
		Plan_s plan = SelectPlan::make_select_plan(parent_table_list, table_list, query_stmt->query_stmt);
		ret = plan->build_plan();
		if (ret != SUCCESS) {
			break;
		}
		SelectPlan* subplan = dynamic_cast<SelectPlan*>(plan.get());
		bool is_correlated = !subplan->ref_parent_table_list.empty();
		//如果子查询引用了当前查询的表的列，则同样需要添加到当前查询的引用列中
		for (auto iter = subplan->parent_table_access_column.cbegin(); iter != subplan->parent_table_access_column.cend(); ++iter) {
			if (iter->first != table) {
				continue;
			}
			const Vector<ColumnDesc>& access_column = iter->second;
			for (u32 j = 0; j < access_column.size(); ++j) {
				add_access_column(access_column[j]);
			}
		}
		expr = SubplanExpression::make_subplan_expression(plan, is_correlated);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::List: 
	{
		ListStmt* list = dynamic_cast<ListStmt*>(expr_stmt);
		Object_s obj = ObjList::make_object();
		ObjList* obj_list = dynamic_cast<ObjList*>(obj.get());
		for (u32 i = 0; i < list->stmt_list.size(); ++i) {
			if (list->stmt_list[i]->stmt_type() != Stmt::Expr) {
				return ERROR_LEX_STMT;
			}
			expr_stmt = dynamic_cast<ExprStmt*>(list->stmt_list[i].get());
			if (expr_stmt->expr_stmt_type() != ExprStmt::Const) {
				return ERROR_LEX_STMT;
			}
			const_stmt = dynamic_cast<ConstStmt*>(expr_stmt);
			obj_list->add_object(const_stmt->value);
		}
		expr = ConstExpression::make_const_expression(obj); 
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Aggregate:
		Log(LOG_ERR, "UpdatePlan", "aggregate stmt in update`s where stmt not support");
		ret = ERROR_LEX_STMT;
		break;
	case ExprStmt::Unary:
		unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		ret = resolve_expr(unary_stmt->expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create unary expression`s first expression failed");
			break;
		}
		expr = UnaryExpression::make_unary_expression(first_expr, unary_stmt->op_type);
		ret = SUCCESS;
		break;
	case ExprStmt::Binary:
		binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		ret = resolve_expr(binary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create binary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(binary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create binary expression`s second expression failed");
			break;
		}
		expr = BinaryExpression::make_binary_expression(first_expr, second_expr, binary_stmt->op_type);
		ret = SUCCESS;
		break;
	case ExprStmt::Ternary:
		ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		ret = resolve_expr(ternary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create ternary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create ternary expression`s second expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->third_expr_stmt, third_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create ternary expression`s third expression failed");
			break;
		}
		expr = TernaryExpression::make_ternary_expression(first_expr, second_expr, third_expr, ternary_stmt->op_type);
		ret = SUCCESS;
		break;
	default:
		Log(LOG_ERR, "UpdatePlan", "unknown expr stmt in update`s where stmt");
		ret = ERROR_LEX_STMT;
	}
	return ret;
}

u32 UpdatePlan::resolve_update_row(const Stmt_s& asgn_stmt, Row_s& row)
{
	if (asgn_stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(asgn_stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		return ERROR_LEX_STMT;
	}
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr_stmt);
	u32 column_count = list_stmt->stmt_list.size();
	RowDesc row_desc(column_count);
	row = Row::make_row(row_desc);
	for (u32 i = 0; i < column_count; ++i) {
		ColumnDesc col_desc;
		Object_s cell;
		u32 ret = resolve_cell(list_stmt->stmt_list[i], col_desc, cell);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "UpdatePlan", "create %uth column failed", i);
			return ret;
		}
		row->get_row_desc().set_column_desc(i, col_desc);
		row->set_cell(i, cell);
	}
	return SUCCESS;
}

u32 UpdatePlan::resolve_cell(const Stmt_s& asign_stmt, ColumnDesc&col_desc, Object_s& cell)
{
	if (asign_stmt->stmt_type() != Stmt::Expr) {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must be expression stmt");
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(asign_stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Binary) {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must be binary expression stmt");
		return ERROR_LEX_STMT;
	}
	BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
	if (binary_stmt->op_type != ExprStmt::OP_EQ) {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must be equal expression stmt");
		return ERROR_LEX_STMT;
	}
	if (binary_stmt->first_expr_stmt->stmt_type() != Stmt::Expr
		|| binary_stmt->second_expr_stmt->stmt_type() != Stmt::Expr) {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must be expression stmt");
		return ERROR_LEX_STMT;
	}
	ColumnStmt* column_stmt = nullptr;
	ConstStmt* const_stmt = nullptr;
	ExprStmt* first_expr = dynamic_cast<ExprStmt*>(binary_stmt->first_expr_stmt.get());
	ExprStmt* second_expr = dynamic_cast<ExprStmt*>(binary_stmt->second_expr_stmt.get());
	if (first_expr->expr_stmt_type() == ExprStmt::Column) {
		column_stmt = dynamic_cast<ColumnStmt*>(first_expr);
	}
	else if (first_expr->expr_stmt_type() == ExprStmt::Const) {
		const_stmt = dynamic_cast<ConstStmt*>(first_expr);
	}
	else {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must have a column stmt and a const stmt");
		return ERROR_LEX_STMT;
	}

	if (second_expr->expr_stmt_type() == ExprStmt::Column) {
		column_stmt = dynamic_cast<ColumnStmt*>(second_expr);
	}
	else if (second_expr->expr_stmt_type() == ExprStmt::Const) {
		const_stmt = dynamic_cast<ConstStmt*>(second_expr);
	}
	else {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must have a column stmt and a const stmt");
		return ERROR_LEX_STMT;
	}
	if (!column_stmt || !const_stmt) {
		Log(LOG_ERR, "UpdatePlan", "update asign stmt must have a column stmt and a const stmt");
		return ERROR_LEX_STMT;
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	if (!checker->have_column(database, table_name, column_stmt->column)) {
		Log(LOG_ERR, "UpdatePlan", "%s.%s do not have column %s",
			database.c_str(), table_name.c_str(), column_stmt->column.c_str());
		return COLUMN_NOT_EXISTS;
	}
	u32 ret = checker->get_column_desc(database, table_name, column_stmt->column, col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	cell = const_stmt->value;
	ret = cast_to(col_desc.get_data_type(), cell);
	if (ret != SUCCESS) {
		return ret;
	}
	else {
		return SUCCESS;
	}
}

void UpdatePlan::add_access_column(const ColumnDesc & col_desc)
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
