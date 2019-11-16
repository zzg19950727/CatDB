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
u32 SqlRewriter::tmp_table_id = 0;

SqlRewriter::SqlRewriter(SelectPlan* parent_query, Expression_s& subquery_expr)
	:parent_query_plan(parent_query),
	subquery_expr(subquery_expr),
	subquery_plan(nullptr)
{
	++tmp_table_id;
	subquery_alias = "tmp_table_" + std::to_string(tmp_table_id);
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
		return rewrite_for_select_N_semi(ret_expr);
	}
	else if (op_type == ExprStmt::OP_NOT_IN) {
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
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}
/*
* 针对NOT IN操作的不相关子查询改写，
* 直接把子查询作为临时表，与父计划连接
* 连接条件为NOT IN的表达式与子查询的select list相等
*/
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
	ret_expr = BinaryExpression::make_binary_expression(lhs, rhs, ExprStmt::OP_ANTI_EQ);

	//为子查询生成临时表ID
	table->join_type = JoinPhyOperator::AntiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}
/*
* 针对IN、EXISTS操作的相关子查询改写，
* 直接把子查询作为临时表，与父计划连接
* 连接条件为IN的表达式与子查询的select list相等，以及相关子查询的相关谓词
*/
u32 SqlRewriter::rewrite_for_select_J_semi(Expression_s& ret_expr)
{
	if (op_type == ExprStmt::OP_IN && (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs )) {
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

	//将相关谓词提到父查询，同时需要重写子查询的select list语句块
	u32 ret = rewrite_select_list(table_id);
	if (ret != SUCCESS) {
		return ret;
	}
	assert(subquery_plan->corrected_predicates.size());
	ret_expr = subquery_plan->corrected_predicates[0];
	for (u32 i = 1; i < subquery_plan->corrected_predicates.size(); ++i) {
		ret_expr = BinaryExpression::make_binary_expression(ret_expr, subquery_plan->corrected_predicates[i], ExprStmt::OP_AND);
	}
	if (op_type == ExprStmt::OP_IN) {
		ColumnDesc col_desc;
		u32 ret = subquery_plan->get_column_from_select_list(subquery_plan->select_list_name[0], col_desc);
		if (ret != SUCCESS) {
			return ret;
		}
		Expression_s rhs = ColumnExpression::make_column_expression(col_desc);
		Expression_s eq = BinaryExpression::make_binary_expression(lhs, rhs, ExprStmt::OP_EQ);
		ret_expr = BinaryExpression::make_binary_expression(ret_expr, eq, ExprStmt::OP_AND);
	}

	//为子查询生成临时表ID
	table->join_type = JoinPhyOperator::SemiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}
/*
* 针对NOT IN、NOT EXISTS操作的相关子查询改写，
* 直接把子查询作为临时表，与父计划连接
* 连接条件为NOT IN的表达式与子查询的select list相等，以及相关子查询的相关谓词
*/
u32 SqlRewriter::rewrite_for_select_J_anti(Expression_s& ret_expr)
{
	if (op_type == ExprStmt::OP_IN && (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs)) {
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
	//将相关谓词提到父查询，同时需要重写子查询的select list语句块
	ret = rewrite_select_list(table_id);
	if (ret != SUCCESS) {
		return ret;
	}
	assert(subquery_plan->corrected_predicates.size());
	ret_expr = subquery_plan->corrected_predicates[0];
	for (u32 i = 1; i < subquery_plan->corrected_predicates.size(); ++i) {
		ret_expr = BinaryExpression::make_binary_expression(ret_expr, subquery_plan->corrected_predicates[i], ExprStmt::OP_AND);
	}
	if (op_type == ExprStmt::OP_NOT_IN) {
		Expression_s rhs = ColumnExpression::make_column_expression(col_desc);
		rhs = BinaryExpression::make_binary_expression(lhs, rhs, ExprStmt::OP_ANTI_EQ);
		ret_expr = BinaryExpression::make_binary_expression(rhs, ret_expr, ExprStmt::OP_AND);
	}
	//为子查询生成临时表ID
	table->join_type = JoinPhyOperator::AntiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}

//注意count的BUG并没绕开
u32 SqlRewriter::rewrite_for_select_JA_semi(Expression_s& ret_expr)
{
	if (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs) {
		Log(LOG_ERR, "SqlRewriter", "JA subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//检查所有的相关谓词是否都是列相等谓词
	for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
		if (!is_column_eq_expr(subquery_plan->corrected_predicates[i])) {
			return CAN_NOT_REWRITE;
		}
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

	//将子查询的scalar group改写成相关谓词引用列的group by，再使用N型子查询改写
	u32 ret = rewrite_group_by_for_JA(table_id);
	if (ret != SUCCESS) {
		return ret;
	}
	assert(subquery_plan->corrected_predicates.size());
	ret_expr = subquery_plan->corrected_predicates[0];
	for (u32 i = 1; i < subquery_plan->corrected_predicates.size(); ++i) {
		ret_expr = BinaryExpression::make_binary_expression(ret_expr, subquery_plan->corrected_predicates[i], ExprStmt::OP_AND);
	}
	ColumnDesc col_desc;
	ret = subquery_plan->get_column_from_select_list(subquery_plan->select_list_name[0], col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	Expression_s rhs = ColumnExpression::make_column_expression(col_desc);
	Expression_s eq = BinaryExpression::make_binary_expression(lhs, rhs, op_type);
	ret_expr = BinaryExpression::make_binary_expression(ret_expr, eq, ExprStmt::OP_AND);

	//为子查询生成临时表ID
	table->join_type = JoinPhyOperator::SemiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}

u32 SqlRewriter::rewrite_select_list(u32 new_table_id)
{
	if (op_type == ExprStmt::OP_EXISTS || op_type == ExprStmt::OP_NOT_EXISTS) {
		subquery_plan->select_list.clear();
		subquery_plan->select_list_name.clear();
	}
	u32 ret = SUCCESS;
	for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
		ret = rewrite_expr(subquery_plan->corrected_predicates[i], new_table_id);
		if (ret != SUCCESS) {
			break;
		}
	}
	return ret;
}

u32 SqlRewriter::rewrite_expr(Expression_s& expr, u32 new_table_id)
{
	u32 ret = SUCCESS;
	switch (expr->get_type())
	{
	case Expression::Const:
	{
		break;
	}
	case Expression::Column:
	{
		ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
		TableStmt* table = nullptr;
		ret = subquery_plan->who_have_column(col_expr->col_desc, table);
		if (ret != SUCCESS) {
			ret = CAN_NOT_REWRITE;
			break;
		}
		if (!subquery_plan->find_table_from_parent(table)) {
			Expression_s item = ColumnExpression::make_column_expression(col_expr->col_desc);
			subquery_plan->select_list_name.push_back("");
			subquery_plan->select_list.push_back(item);
			col_expr->col_desc.set_tid_cid(new_table_id, subquery_plan->select_list.size()-1);
		}
		break;
	}
	case Expression::Subplan:
	{
		ret = CAN_NOT_REWRITE;
		break;
	}
	case Expression::Aggregate:
	{
		ret = CAN_NOT_REWRITE;
		break;
	}
	case Expression::Unary:
	{
		UnaryExpression* unary = dynamic_cast<UnaryExpression*>(expr.get());
		ret = rewrite_expr(unary->expr, new_table_id);
		break;
	}
	case Expression::Binary:
	{
		BinaryExpression* binary = dynamic_cast<BinaryExpression*>(expr.get());
		ret = rewrite_expr(binary->first_expr, new_table_id);
		if (ret == SUCCESS) {
			ret = rewrite_expr(binary->second_expr, new_table_id);
		}
		break;
	}
	case Expression::Ternary:
	{
		TernaryExpression* ternary = dynamic_cast<TernaryExpression*>(expr.get());
		ret = rewrite_expr(ternary->first_expr, new_table_id);
		if (ret == SUCCESS) {
			ret = rewrite_expr(ternary->second_expr, new_table_id);
			if (ret == SUCCESS) {
				ret = rewrite_expr(ternary->third_expr, new_table_id);
			}
		}
		break;
	}
	default:
		ret = CAN_NOT_REWRITE;
	}
	return ret;
}

bool SqlRewriter::is_column_eq_expr(const Expression_s& expr)
{
	if (expr->get_type() != Expression::Binary) {
		return false;
	}
	BinaryExpression* binary = dynamic_cast<BinaryExpression*>(expr.get());
	if (binary->op.get_type() != ExprStmt::OP_EQ) {
		return false;
	}
	else if (binary->first_expr->get_type() != Expression::Column
		|| binary->second_expr->get_type() != Expression::Column) {
		return false;
	}
	else {
		return true;
	}
}

u32 rewrite_agg_list(Expression_s& expr, u32 new_table_id)
{
	u32 ret = SUCCESS;
	switch (expr->get_type())
	{
		case Expression::Const:
		{
			break;
		}
		case Expression::Column:
		{
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
			col_expr->col_desc.set_tid(new_table_id);
			break;
		}
		case Expression::Unary:
		{
			UnaryExpression* unary = dynamic_cast<UnaryExpression*>(expr.get());
			ret = rewrite_agg_list(unary->expr, new_table_id);
			break;
		}
		case Expression::Binary:
		{
			BinaryExpression* binary = dynamic_cast<BinaryExpression*>(expr.get());
			ret = rewrite_agg_list(binary->first_expr, new_table_id);
			if (ret == SUCCESS) {
				ret = rewrite_agg_list(binary->second_expr, new_table_id);
			}
			break;
		}
		case Expression::Ternary:
		{
			TernaryExpression* ternary = dynamic_cast<TernaryExpression*>(expr.get());
			ret = rewrite_agg_list(ternary->first_expr, new_table_id);
			if (ret == SUCCESS) {
				ret = rewrite_agg_list(ternary->second_expr, new_table_id);
				if (ret == SUCCESS) {
					ret = rewrite_agg_list(ternary->third_expr, new_table_id);
				}
			}
			break;
		}
	}
	return ret;
}

u32 SqlRewriter::rewrite_group_by_for_JA(u32 new_table_id)
{
	for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
		BinaryExpression* binary = dynamic_cast<BinaryExpression*>(subquery_plan->corrected_predicates[i].get());
		ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(binary->first_expr.get());
		TableStmt* table = nullptr;
		u32 ret = subquery_plan->who_have_column(col_expr->col_desc, table);
		if (ret != SUCCESS) {
			ret = CAN_NOT_REWRITE;
			break;
		}
		if (!subquery_plan->find_table_from_parent(table)) {
			Expression_s item = ColumnExpression::make_column_expression(col_expr->col_desc);
			subquery_plan->select_list_name.push_back("");
			subquery_plan->select_list.push_back(item);
			subquery_plan->group_cols.push_back(item);
			col_expr->col_desc.set_tid_cid(new_table_id, subquery_plan->select_list.size() - 1);
		}
		else {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(binary->second_expr.get());
			TableStmt* table = nullptr;
			u32 ret = subquery_plan->who_have_column(col_expr->col_desc, table);
			if (ret != SUCCESS) {
				ret = CAN_NOT_REWRITE;
				break;
			}
			if (!subquery_plan->find_table_from_parent(table)) {
				Expression_s item = ColumnExpression::make_column_expression(col_expr->col_desc);
				subquery_plan->select_list_name.push_back("");
				subquery_plan->select_list.push_back(item);
				subquery_plan->group_cols.push_back(item);
				col_expr->col_desc.set_tid_cid(new_table_id, subquery_plan->select_list.size() - 1);
			}
			else {
				return CAN_NOT_REWRITE;
			}
		}
	}
	
	return rewrite_agg_list(subquery_plan->select_list[0], new_table_id);
}