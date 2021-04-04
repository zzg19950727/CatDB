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
	//JA�Ӳ�ѯ��д
	if (is_correlated && subquery_plan->is_simple_scalar_group()) {
		return rewrite_for_select_JA_semi(ret_expr);
	}
	//J���Ӳ�ѯ��д
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
	//N��IN��NOT IN�Ӳ�ѯ��д
	else if (op_type == ExprStmt::OP_IN) {
		return rewrite_for_select_N_semi(ret_expr);
	}
	else if (op_type == ExprStmt::OP_NOT_IN) {
		return rewrite_for_select_N_anti(ret_expr);
	}
	//N��EXISTS��NOT EXISTS��A���Ӳ�ѯ���ø�д��ֱ����Ϊ�Ӽƻ�ִ��
	else {
		return CAN_NOT_REWRITE;
	}
}

/*
 * ���IN�����Ĳ�����Ӳ�ѯ��д��
 * ֱ�Ӱ��Ӳ�ѯ��Ϊ��ʱ�����븸�ƻ�����
 * ��������ΪIN�ı���ʽ���Ӳ�ѯ��select list���
 */
u32 SqlRewriter::rewrite_for_select_N_semi(Expression_s& ret_expr)
{
	if (subquery_plan->select_list.size() != 1 
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs) {
		LOG_ERR("IN subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//������ʱ��
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//������ʱ��ID
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

	//Ϊ�Ӳ�ѯ������ʱ��ID
	table->join_type = JoinPhyOperator::SemiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}
/*
* ���NOT IN�����Ĳ�����Ӳ�ѯ��д��
* ֱ�Ӱ��Ӳ�ѯ��Ϊ��ʱ�����븸�ƻ�����
* ��������ΪNOT IN�ı���ʽ���Ӳ�ѯ��select list���
*/
u32 SqlRewriter::rewrite_for_select_N_anti(Expression_s& ret_expr)
{ 
	if (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs) {
		LOG_ERR("NOT IN subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//������ʱ��
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//������ʱ��ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	subquery_plan->set_alias_table_id(table_id);

	ColumnDesc col_desc;
	u32 ret = subquery_plan->get_column_from_select_list(subquery_plan->select_list_name[0], col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	// a not in (selct b...)��anti join������
	// a is null or b is null or a=b
	//Ŀǰֻ֧��NESTED LOOP��дAnti join
	Expression_s rhs = ColumnExpression::make_column_expression(col_desc);
	ret_expr = BinaryExpression::make_binary_expression(lhs, rhs, ExprStmt::OP_ANTI_EQ);
	table->anti_cond = ret_expr;

	//Ϊ�Ӳ�ѯ������ʱ��ID
	table->join_type = JoinPhyOperator::AntiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}
/*
* ���IN��EXISTS����������Ӳ�ѯ��д��
* ֱ�Ӱ��Ӳ�ѯ��Ϊ��ʱ�����븸�ƻ�����
* ��������ΪIN�ı���ʽ���Ӳ�ѯ��select list��ȣ��Լ�����Ӳ�ѯ�����ν��
*/
u32 SqlRewriter::rewrite_for_select_J_semi(Expression_s& ret_expr)
{
	if (op_type == ExprStmt::OP_IN && (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs )) {
		LOG_ERR("IN subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//������ʱ��
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//������ʱ��ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	subquery_plan->set_alias_table_id(table_id);

	//�����ν���ᵽ����ѯ��ͬʱ��Ҫ��д�Ӳ�ѯ��select list����
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

	//Ϊ�Ӳ�ѯ������ʱ��ID
	table->join_type = JoinPhyOperator::SemiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}
/*
* ���NOT IN��NOT EXISTS����������Ӳ�ѯ��д��
* ֱ�Ӱ��Ӳ�ѯ��Ϊ��ʱ�����븸�ƻ�����
* ��������ΪNOT IN�ı���ʽ���Ӳ�ѯ��select list��ȣ��Լ�����Ӳ�ѯ�����ν��
*/
u32 SqlRewriter::rewrite_for_select_J_anti(Expression_s& ret_expr)
{
	if (op_type == ExprStmt::OP_IN && (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs)) {
		LOG_ERR("NOT IN subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//������ʱ��
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//������ʱ��ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	subquery_plan->set_alias_table_id(table_id);

	ColumnDesc col_desc;
	u32 ret = subquery_plan->get_column_from_select_list(subquery_plan->select_list_name[0], col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	// a not in (selct b...)��anti join������
	// a is null or b is null or a=b
	//Ŀǰֻ֧��NESTED LOOP��дAnti join
	//�����ν���ᵽ����ѯ��ͬʱ��Ҫ��д�Ӳ�ѯ��select list����
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
		table->anti_cond = rhs;
		ret_expr = BinaryExpression::make_binary_expression(rhs, ret_expr, ExprStmt::OP_AND);
	}
	//Ϊ�Ӳ�ѯ������ʱ��ID
	table->join_type = JoinPhyOperator::AntiJoin;
	table->subplan = subquery;
	table->table_id = table_id;
	table->op_type = op_type;
	parent_query_plan->table_list.push_back(table);
	parent_query_plan->tmp_table_handle.push_back(stmt);
	return SUCCESS;
}

//ע��count��BUG��û�ƿ�
u32 SqlRewriter::rewrite_for_select_JA_semi(Expression_s& ret_expr)
{
	if (subquery_plan->select_list.size() != 1
		|| subquery_plan->select_list_name.size() != 1
		|| !lhs) {
		LOG_ERR("JA subquery not support multi column yet");
		return ERROR_LEX_STMT;
	}
	//������е����ν���Ƿ��������ν��
	for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
		if (!is_column_eq_expr(subquery_plan->corrected_predicates[i])) {
			return CAN_NOT_REWRITE;
		}
	}
	//������ʱ��
	TableStmt* table = nullptr;
	Stmt_s stmt = TableStmt::make_table_stmt(subquery_alias);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//������ʱ��ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	subquery_plan->set_alias_table_id(table_id);

	//���Ӳ�ѯ��scalar group��д�����ν�������е�group by����ʹ��N���Ӳ�ѯ��д
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

	//Ϊ�Ӳ�ѯ������ʱ��ID
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