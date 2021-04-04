#include "schema_checker.h"
#include "select_plan.h"
#include "delete_plan.h"
#include "delete_stmt.h"
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
		LOG_ERR("there is no plan for delete");
		set_error_code(PLAN_NOT_BUILD);
		return PLAN_NOT_BUILD;
	}
	//���ͼƻ�
	if (is_explain) {
		return explain_plan();
	}
	LOG_TRACE("start execute delete plan");
	u32 ret;
	ret = root_operator->open();
	if (ret != SUCCESS) {
		LOG_ERR("open delete plan failed");
		set_error_code(ret);
		return ret;
	}
	
	affect_rows_ = 0;
	Row_s row;
	while ((ret = root_operator->get_next_row(row)) == SUCCESS)
	{
		++affect_rows_;
		LOG_TRACE("delete row success", K(row));
	}
	u32 ret2 = root_operator->close();
	if (ret != NO_MORE_ROWS) {
		LOG_ERR("execute delete plan failed");
		set_error_code(ret);
		return ret;
	}
	else if (ret2 != SUCCESS) {
		LOG_ERR("close plan failed");
		set_error_code(ret2);
		return ret2;
	}
	else {
		LOG_TRACE("execute delete plan success");
		set_error_code(SUCCESS);
		return SUCCESS;
	}
}

u32 DeletePlan::build_plan()
{
	root_operator = Delete::make_delete(database, table_name, alias_table_name, filter);
	if (access_columns.size() > 0) {
		RowDesc row_desc(access_columns.size());
		for (u32 i = 0; i < access_columns.size(); ++i) {
			row_desc.set_column_desc(i, access_columns[i]);
		}
		root_operator->set_access_desc(row_desc);
		root_operator->set_access_desc(row_desc);
	}
	set_error_code(SUCCESS);
	return SUCCESS;
}

u32 DeletePlan::optimizer()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Delete)
	{
		LOG_ERR("error lex stmt when build delete plan", K(lex_stmt));
		set_error_code(ERROR_LEX_STMT);
		return ERROR_LEX_STMT;
	}
	LOG_TRACE("start build delete plan");
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	DeleteStmt* lex = dynamic_cast<DeleteStmt*>(lex_stmt.get());
	assert(lex->table);
	is_explain = lex->is_explain;
	//������������
	table = dynamic_cast<TableStmt*>(lex->table.get());
	database = table->database;
	table_name = table->table_name;
	alias_table_name = table->alias_name;
	//��where�Ӿ�ת��Ϊfilter
	u32 ret = resolve_filter(lex->where_stmt, filter);
	if (ret != SUCCESS) {
		LOG_ERR("create filter from where stmt error", K(lex->where_stmt));
		set_error_code(ret);
		return ret;
	}
	return SUCCESS;
}

Plan::PlanType DeletePlan::type() const
{
	return Plan::DELETE_PLAN;
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
	case ExprStmt::Column:
	{
		ColumnStmt* column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
		if (!checker->have_column(database, table_name, column_stmt->column)) {
			LOG_ERR("table do not have column", K(database), K(table_name), K(column_stmt));
			return COLUMN_NOT_EXISTS;
		}
		ColumnDesc col_desc;
		ret = checker->get_column_desc(database, table_name, column_stmt->column, col_desc);
		if (ret != SUCCESS) {
			break;
		}
		add_access_column(col_desc);
		expr = ColumnExpression::make_column_expression(col_desc);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Query:
	{
		QueryStmt* query_stmt = dynamic_cast<QueryStmt*>(expr_stmt);
		Vector<TableStmt*> parent_table_list;
		Vector<TableStmt*> table_list;
		table_list.push_back(table);
		Plan_s plan = SelectPlan::make_select_plan(parent_table_list, table_list, query_stmt->query_stmt);
		ret = plan->optimizer();
		if (ret != SUCCESS) {
			break;
		}
		ret = plan->build_plan();
		if (ret != SUCCESS) {
			break;
		}
		SelectPlan* subplan = dynamic_cast<SelectPlan*>(plan.get());
		bool is_correlated = !subplan->ref_parent_table_list.empty();
		//����Ӳ�ѯ�����˵�ǰ��ѯ�ı����У���ͬ����Ҫ���ӵ���ǰ��ѯ����������
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
			ConstStmt* const_stmt = dynamic_cast<ConstStmt*>(expr_stmt);
			obj_list->add_object(const_stmt->value);
		}
		expr = ConstExpression::make_const_expression(obj);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Aggregate:
	{
		LOG_ERR("aggregate stmt in delete`s where stmt not support");
		ret = ERROR_LEX_STMT;
		break;
	}
	case ExprStmt::Unary:
	{
		UnaryExprStmt* unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		Expression_s first_expr;
		ret = resolve_expr(unary_stmt->expr_stmt, first_expr);
		if (ret != SUCCESS) {
			LOG_ERR("create unary expression`s first expression failed", K(unary_stmt->expr_stmt));
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
			LOG_ERR("create binary expression`s first expression failed", K(binary_stmt->first_expr_stmt));
			break;
		}
		ret = resolve_expr(binary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			LOG_ERR("create binary expression`s second expression failed", K(binary_stmt->second_expr_stmt));
			break;
		}
		expr = BinaryExpression::make_binary_expression(first_expr, second_expr, binary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Ternary:
	{
		TernaryExprStmt* ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		Expression_s first_expr, second_expr, third_expr;
		ret = resolve_expr(ternary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			LOG_ERR("create ternary expression`s first expression failed", K(ternary_stmt->first_expr_stmt));
			break;
		}
		ret = resolve_expr(ternary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			LOG_ERR("create ternary expression`s second expression failed", K(ternary_stmt->second_expr_stmt));
			break;
		}
		ret = resolve_expr(ternary_stmt->third_expr_stmt, third_expr);
		if (ret != SUCCESS) {
			LOG_ERR("create ternary expression`s third expression failed", K(ternary_stmt->third_expr_stmt));
			break;
		}
		expr = TernaryExpression::make_ternary_expression(first_expr, second_expr, third_expr, ternary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	default:
		LOG_ERR("unknown expr stmt in delete`s where stmt", K(expr_stmt));
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
