#include "schema_checker.h"
#include "phy_operator.h"
#include "sql_rewrite.h"
#include "select_plan.h"
#include "update_plan.h"
#include "delete_plan.h"
#include "expression.h"
#include "plan.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

SqlRewriter::SqlRewriter(SelectPlan* parent_query, Expression_s& subquery_expr)
	:parent_query_plan(parent_query),
	subquery_expr(subquery_expr),
	subquery_plan(nullptr)
{

}

SqlRewriter::~SqlRewriter()
{

}

SqlRewriter_s SqlRewriter::make_sql_rewriter(SelectPlan* parent_query,
	Expression_s& subquery_expr,
	const Expression_s& lhs,
	ExprStmt::OperationType op_type)
{
	SqlRewriter* rewriter = new SqlRewriter(parent_query, subquery_expr);
	rewriter->lhs = lhs;
	rewriter->op_type = op_type;
	return SqlRewriter_s(rewriter);
}

u32 SqlRewriter::rewrite_for_select(Expression_s& ret_expr)
{
	if (subquery_expr->get_type() != Expression::Subplan) {
		return CAN_NOT_REWRITE;
	}
	SubplanExpression* expr = dynamic_cast<SubplanExpression*>(subquery_expr.get());
	subquery = expr->subplan;
	subquery_alias = expr->alias_name;
	if (subquery->type() != Plan::SELECT) {
		return CAN_NOT_REWRITE;
	}
	subquery_plan = dynamic_cast<SelectPlan*>(subquery.get());
	is_correlated = subquery_plan->is_correlated_query();
	//JA子查询改写
	if (is_correlated && subquery_plan->is_simple_scalar_group()) {
		return rewrite_for_select_JA_semi(ret_expr);
	}
	//J型子查询改写
	else if (is_correlated) {
		if (op_type == ExprStmt::OP_IN || op_type == ExprStmt::OP_EXISTS) {
			return rewrite_for_select_J_semi(ret_expr);
		}
		else if (op_type == ExprStmt::OP_NOT_IN || op_type == ExprStmt::OP_NOT_EXISTS) {
			return rewrite_for_select_J_anti(ret_expr);
		}
		else {
			return CAN_NOT_REWRITE;
		}
	}
	//N型IN、NOT IN子查询改写
	else if (op_type == ExprStmt::OP_IN) {
		return CAN_NOT_REWRITE;
		return rewrite_for_select_N_semi(ret_expr);
	}
	else if (op_type == ExprStmt::OP_NOT_IN) {
		return CAN_NOT_REWRITE;
		return rewrite_for_select_N_anti(ret_expr);
	}
	//N型EXISTS、NOT EXISTS，A型子查询不用改写，直接作为子计划执行
	else {
		return CAN_NOT_REWRITE;
	}
}

/*
 * 针对IN操作的不相关子查询改写，
 * 直接把子查询作为临时表，与父计划连接
 * 连接条件为IN的表达式与子查询的select list相等
 */
u32 SqlRewriter::rewrite_for_select_N_semi(Expression_s& ret_expr)
{
	if (subquery_plan->select_list.size() != 1 
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs) {
		Log(LOG_ERR, "SqlRewriter", "IN subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//构建临时表
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//生产临时表ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	subquery_plan->set_alias_table_id(table_id);

	ColumnDesc col_desc;
	u32 ret = subquery_plan->get_column_from_select_list(subquery_plan->select_list_name[0], col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	Expression_s rhs = ColumnExpression::make_column_expression(col_desc);
	ret_expr = BinaryExpression::make_binary_expression(lhs, rhs, ExprStmt::OP_EQ);

	//为子查询生成临时表ID
	table->join_type = JoinPhyOperator::SemiJoin;
	table->subplan = subquery;
	table->table_id = table_id;

	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}

u32 SqlRewriter::rewrite_for_select_N_anti(Expression_s& ret_expr)
{
	if (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs) {
		Log(LOG_ERR, "SqlRewriter", "NOT IN subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//构建临时表
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//生产临时表ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	subquery_plan->set_alias_table_id(table_id);

	ColumnDesc col_desc;
	u32 ret = subquery_plan->get_column_from_select_list(subquery_plan->select_list_name[0], col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	// a not in (selct b...)的anti join条件是
	// a is null or b is null or a=b
	//目前只支持NESTED LOOP改写Anti join
	Expression_s rhs = ColumnExpression::make_column_expression(col_desc);
	Expression_s first_exr = UnaryExpression::make_unary_expression(lhs, ExprStmt::OP_IS_NULL);
	Expression_s second_exr = UnaryExpression::make_unary_expression(rhs, ExprStmt::OP_IS_NULL);
	Expression_s third_exr = BinaryExpression::make_binary_expression(lhs, rhs, ExprStmt::OP_EQ);
	ret_expr = BinaryExpression::make_binary_expression(first_exr, second_exr, ExprStmt::OP_OR);
	ret_expr = BinaryExpression::make_binary_expression(ret_expr, third_exr, ExprStmt::OP_OR);

	//为子查询生成临时表ID
	table->join_type = JoinPhyOperator::AntiJoin;
	table->subplan = subquery;
	table->table_id = table_id;

	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}

u32 SqlRewriter::rewrite_for_select_J_semi(Expression_s& ret_expr)
{
	return CAN_NOT_REWRITE;
}

u32 SqlRewriter::rewrite_for_select_J_anti(Expression_s& ret_expr)
{
	return CAN_NOT_REWRITE;
}

u32 SqlRewriter::rewrite_for_select_JA_semi(Expression_s& ret_expr)
{
	return CAN_NOT_REWRITE;
}
