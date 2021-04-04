#include "schema_checker.h"
#include "sql_rewrite.h"
#include "select_plan.h"
#include "select_stmt.h"
#include "expression.h"
#include "expr_stmt.h"
#include "filter.h"
#include "object.h"

#include "nested_loop_join.h"
#include "hash_distinct.h"
#include "scalar_group.h"
#include "plan_filter.h"
#include "hash_group.h"
#include "table_scan.h"
#include "hash_join.h"
#include "hash_join.h"
#include "hash_set.h"
#include "limit.h"
#include "sort.h"
#include "query_result.h"
#include "statis.h"
#include "query.h"
#include "stmt.h"
#include "row.h"
#include "error.h"
#include "log.h"

#define return_result(code) \
	set_error_code(code);\
	return code;

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;

SelectPlan::SelectPlan()
	:resolve_select_list_or_having(0),
	alias_table_id(0),
	is_distinct(false),
	is_sort_query_result(false),
	is_resolve_where(false),
	asc(true),
	have_limit(false),
	root_plan(this)
{

}

SelectPlan::~SelectPlan()
{

}

Plan_s SelectPlan::make_select_plan(const Stmt_s& lex_insert_stmt)
{
	SelectPlan* plan = new SelectPlan;
	plan->set_lex_stmt(lex_insert_stmt);
	return Plan_s(plan);
}

bool SelectPlan::send_plan_result() const
{
	return true;
}

Plan_s SelectPlan::make_select_plan(const Vector<TableStmt*>& grandparent_query_tables, 
	const Vector<TableStmt*>& parent_query_tables, 
	const Stmt_s & lex_insert_stmt)
{
	SelectPlan* plan = new SelectPlan;
	plan->set_lex_stmt(lex_insert_stmt);
	for (u32 i = 0; i < grandparent_query_tables.size(); ++i) {
		plan->parent_table_list.push_back(grandparent_query_tables[i]);
	}
	for (u32 i = 0; i < parent_query_tables.size(); ++i) {
		plan->parent_table_list.push_back(parent_query_tables[i]);
	}
	return Plan_s(plan);
}

/*ִ��select�ƻ�*/
u32 SelectPlan::execute()
{
	if (!root_operator) {
		return_result(PLAN_NOT_BUILD);
	}
	//���ͼƻ�
	if (is_explain) {
		return explain_plan();
	}
	//�򿪶�������
	u32 ret;
	ret = root_operator->open();
	if (ret != SUCCESS) {
		return_result(ret);
	}
	//�������title
	RowDesc row_desc(select_list_name.size());
	result_title = Row::make_row(row_desc);
	for (u32 i = 0; i < select_list_name.size(); ++i) {
		Object_s label = Varchar::make_object(select_list_name[i]);
		result_title->set_cell(i, label);
	}
	//������ѯ�������
	affect_rows_ = 0;
	Row_s row;
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	while ((ret = root_operator->get_next_row(row)) == SUCCESS)
	{
		query_result->add_row(row);
		++affect_rows_;
	}
	//�ر�����
	u32 ret2 = root_operator->close();
	if (ret != NO_MORE_ROWS) {
		return_result(ret);
	}
	else if (ret2 != SUCCESS) {
		return_result(ret2);
	}
	else {
		return_result(SUCCESS);
	}
}

u32 SelectPlan::build_plan()
{
	//����Ǽ������㣬���Ѿ����ɼƻ���ֱ�ӷ���
	if (root_operator) {
		return SUCCESS;
	}
	for (u32 i = 0; i < subquerys.size(); ++i) {
		subquerys[i]->build_plan();
	}
	//����ÿ�ű��ķ���������ÿ�ű���������
	u32 ret = make_access_row_desc();
	if (ret != SUCCESS) {
		LOG_ERR("make access row desc for from tables failed");
		return_result(ret);
	}
	//ѡ������join˳��
	ret = choos_best_join_order();
	if (ret != SUCCESS) {
		LOG_ERR("choose best join order failed");
		return_result(ret);
	}
	//����join�ƻ�
	ret = make_join_plan(root_operator);
	if (ret != SUCCESS) {
		LOG_ERR("make join plan failed");
		return_result(ret);
	}
	//����group�ƻ�
	ret = make_group_pan(root_operator);
	if (ret != SUCCESS) {
		LOG_ERR("make group plan failed");
		return_result(ret);
	}
	//����distinct�ƻ�
	ret = make_distinct_plan(root_operator);
	if (ret != SUCCESS) {
		LOG_ERR("make distinct plan failed");
		return_result(ret);
	}
	//ͶӰѡ����
	ret = make_query_plan(root_operator);
	if (ret != SUCCESS) {
		LOG_ERR("make expression plan failed");
		return_result(ret);
	}
	//�Ƿ���Ҫ����
	ret = make_sort_plan(root_operator);
	if (ret != SUCCESS) {
		LOG_ERR("make sort plan failed");
		return_result(ret);
	}
	//����limit�ƻ�
	ret = make_limit_plan(root_operator);
	if (ret != SUCCESS) {
		LOG_ERR("make limit plan failed");
		return_result(ret);
	}
	return_result(SUCCESS);
}

u32 SelectPlan::optimizer()
{
	if (!lex_stmt)
	{
		LOG_ERR("error lex stmt when build select plan");
		return_result(ERROR_LEX_STMT);
	}
	is_explain = lex_stmt->is_explain;
	if (lex_stmt->stmt_type() == Stmt::Expr)
	{
		ExprStmt* expr = dynamic_cast<ExprStmt*>(lex_stmt.get());

		if (expr->expr_stmt_type() == ExprStmt::Query) {
			LOG_TRACE("build subquery plan");
			QueryStmt* query = dynamic_cast<QueryStmt*>(expr);
			lex_stmt = query->query_stmt;
			return optimizer();
		}
		else if (expr->expr_stmt_type() == ExprStmt::Binary) {
			//���ɽ����������ƻ�
			u32 ret = make_set_plan(root_operator);
			return_result(ret);
		}
		else {
			LOG_ERR("error lex stmt when build select plan", K(expr));
			return_result(ERROR_LEX_STMT);
		}
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	//�Ƿ���distinct��limit
	SelectStmt* lex = dynamic_cast<SelectStmt*>(lex_stmt.get());
	is_distinct = lex->is_distinct;
	asc = lex->asc_desc;
	if (lex->limit_stmt)
		have_limit = true;
	else
		have_limit = false;
	//��ȡ���ñ�
	u32 ret = get_ref_tables(lex->from_stmts);
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in from stmts", K(lex->from_stmts));
		return_result(ret);
	}
	//����select�����
	ret = resolve_select_list(lex->select_expr_list);
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in select list", K(lex->select_expr_list));
		return_result(ret);
	}
	//����where�Ӿ�
	is_resolve_where = true;
	ret = resolve_where_stmt(lex->where_stmt);
	is_resolve_where = false;
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in where stmts");
		return_result(ret);
	}
	//����group�Ӿ�
	ret = resolve_group_stmt(lex->group_columns);
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in group stmts");
		return_result(ret);
	}
	//����having�Ӿ�
	ret = resolve_having_stmt(lex->having_stmt);
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in having stmts");
		return_result(ret);
	}
	//����sort�Ӿ�
	ret = resolve_sort_stmt(lex->order_columns);
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in sort stmts");
		return_result(ret);
	}
	//����limit�Ӿ�
	ret = resolve_limit_stmt(lex->limit_stmt);
	if (ret != SUCCESS) {
		LOG_ERR("error stmt in limit stmts");
		return_result(ret);
	}
	return ret;
}

Plan::PlanType SelectPlan::type() const
{
	return Plan::SELECT;
}

void SelectPlan::set_alias_table_id(u32 id)
{
	alias_table_id = id;
}

u32 SelectPlan::get_alias_table_id() const
{
	return alias_table_id;
}

u32 SelectPlan::get_column_from_select_list(const String & column_name, ColumnDesc & col_desc)
{
	for (u32 i = 0; i < select_list_name.size(); ++i) {
		if (column_name == select_list_name[i]) {
			col_desc.set_tid_cid(alias_table_id, i);
			return SUCCESS;
		}
	}
	return COLUMN_NOT_EXISTS;
}

u32 SelectPlan::get_query_row_desc(RowDesc & row_desc)
{
	ColumnDesc col_desc;
	for (u32 i = 0; i < select_list.size(); ++i) {
		col_desc.set_tid_cid(alias_table_id, i);
		row_desc.add_column_desc(col_desc);
	}
	return SUCCESS;
}

const Vector<String>& SelectPlan::get_all_output_column() const
{
	return select_list_name;
}

void SelectPlan::reset_for_correlated_subquery(const Row_s & row)
{
	if (!root_operator) {
		return;
	}
	result.reset();
	root_operator->reopen(row);
}

bool SelectPlan::is_simple_scalar_group() const
{
	return group_cols.empty() && (aggr_exprs.size() == 1);
}

bool SelectPlan::is_correlated_query() const
{
	return !ref_parent_table_list.empty();
}

double SelectPlan::get_select_rows()
{
	return root_operator->output_rows;
}

/*
 * ����where�Ӿ�飬�зֳ�and���ӵ�ν�ʷ���
 */
u32 SelectPlan::resolve_where_stmt(const Stmt_s& where_stmt)
{
	if (!where_stmt) {
		return SUCCESS;
	}
	if (where_stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}

	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(where_stmt.get());
	//�����and���ӵ�ν��
	if (expr_stmt->expr_stmt_type() == ExprStmt::Binary) {
		BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		if (binary_stmt->op_type == ExprStmt::OP_AND) {
			//�ݹ����������
			u32 ret = resolve_where_stmt(binary_stmt->first_expr_stmt);
			if (ret != SUCCESS) {
				LOG_ERR("create binary expression`s first expression failed");
				return ret;
			}
			//�ݹ����������
			ret = resolve_where_stmt(binary_stmt->second_expr_stmt);
			if (ret != SUCCESS) {
				LOG_ERR("create binary expression`s second expression failed");
				return ret;
			}
			return SUCCESS;
		}
	}
	//�ָ��С����
	return resolve_simple_stmt(where_stmt);
}

/*
 * ������С����䣬�ֳ����������
 * ������ֵ����ν�ʡ�������ͨ����ν��
 * ��������ν�ʡ���ͨ����ν��
 */
u32 SelectPlan::resolve_simple_stmt(const Stmt_s& stmt)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	bool have_parent_column = false;
	Expression_s expr;
	u32 ret = resolve_expr(stmt, expr, have_parent_column);
	if (ret != SUCCESS) {
		return ret;
	}
	//���������Ӳ�ѯ�����ν�ʣ���ȴ���д�������������
	if (have_parent_column) {
		corrected_predicates.push_back(expr);
		return SUCCESS;
	}
	if (expr->get_type() == Expression::Binary) {
		BinaryExpression* binary_expr = dynamic_cast<BinaryExpression*>(expr.get());
		if (binary_expr->op.get_type() == ExprStmt::OP_AND) {
			ret = resolve_simple_expr(binary_expr->first_expr);
			if (ret == SUCCESS) {
				ret = resolve_simple_expr(binary_expr->second_expr);
			}
			return ret;
		}
		//������join����
		else if (binary_expr->first_expr->get_type() == Expression::Column
			&& binary_expr->second_expr->get_type() == Expression::Column) {
			TableStmt* first_table = nullptr;
			TableStmt* second_table = nullptr;
			ColumnExpression* first_column = dynamic_cast<ColumnExpression*>(binary_expr->first_expr.get());
			ColumnExpression* second_column = dynamic_cast<ColumnExpression*>(binary_expr->second_expr.get());
			u32 ret = who_have_column(first_column->col_desc, first_table);
			if (ret != SUCCESS) {
				return ret;
			}
			ret = who_have_column(second_column->col_desc, second_table);
			if (ret != SUCCESS) {
				return ret;
			}
			//���ű����й�������������ǰ�������ű���������������ѯ���������ű�����ͬһ�ű�
			if(!find_table_from_parent(first_table)
				&& !find_table_from_parent(second_table)
				&& first_table->table_id != second_table->table_id) {
				if (binary_expr->op.get_type() == ExprStmt::OP_EQ) {
					add_join_equal_cond(JoinableTables(first_table, second_table), expr);
					add_join_cond(JoinableTables(first_table, second_table), expr);
					return SUCCESS;
				}
				else {
					add_join_cond(JoinableTables(first_table, second_table), expr);
					return SUCCESS;
				}
			}
		}
	}

	//����Ƿ��ǵ��ű��Ĺ���ν��
	TableStmt* table = nullptr;
	if (is_table_filter(expr, table)) {
		add_table_filter(table, expr);
	}
	else {
		//��ͨ����ν�����������join��ʹ��
		make_and_expression(filter_after_join, expr);
	}
	return SUCCESS;
}

u32 SelectPlan::resolve_simple_expr(const Expression_s & expr)
{
	if (expr->get_type() == Expression::Binary) {
		BinaryExpression* binary_expr = dynamic_cast<BinaryExpression*>(expr.get());
		if (binary_expr->op.get_type() == ExprStmt::OP_AND) {
			u32 ret = resolve_simple_expr(binary_expr->first_expr);
			if (ret == SUCCESS) {
				ret = resolve_simple_expr(binary_expr->second_expr);
			}
			return ret;
		}
		//������join����
		else if (binary_expr->first_expr->get_type() == Expression::Column
			&& binary_expr->second_expr->get_type() == Expression::Column) {
			TableStmt* first_table = nullptr;
			TableStmt* second_table = nullptr;
			ColumnExpression* first_column = dynamic_cast<ColumnExpression*>(binary_expr->first_expr.get());
			ColumnExpression* second_column = dynamic_cast<ColumnExpression*>(binary_expr->second_expr.get());
			u32 ret = who_have_column(first_column->col_desc, first_table);
			if (ret != SUCCESS) {
				return ret;
			}
			ret = who_have_column(second_column->col_desc, second_table);
			if (ret != SUCCESS) {
				return ret;
			}
			//���ű����й�������������ǰ�������ű���������������ѯ���������ű�����ͬһ�ű�
			if (!find_table_from_parent(first_table)
				&& !find_table_from_parent(second_table)
				&& first_table->table_id != second_table->table_id) {
				if (binary_expr->op.get_type() == ExprStmt::OP_EQ) {
					add_join_equal_cond(JoinableTables(first_table, second_table), expr);
					add_join_cond(JoinableTables(first_table, second_table), expr);
					return SUCCESS;
				}
				else {
					add_join_cond(JoinableTables(first_table, second_table), expr);
					return SUCCESS;
				}
			}
		}
	}

	//����Ƿ��ǵ��ű��Ĺ���ν��
	TableStmt* table = nullptr;
	if (is_table_filter(expr, table)) {
		add_table_filter(table, expr);
	}
	else {
		//��ͨ����ν�����������join��ʹ��
		make_and_expression(filter_after_join, expr);
	}
	return SUCCESS;
}

/*
 * �����ǰν��������󷵻��У����򷵻�null
 */
ColumnStmt * SelectPlan::resolve_column_stmt(const Stmt_s & stmt)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return nullptr;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	if (expr_stmt->expr_stmt_type() == ExprStmt::Column) {
		return dynamic_cast<ColumnStmt*>(expr_stmt);
	}
	else {
		return nullptr;
	}
}

/*
 * ��ν������ת��Ϊ����ʽ
 */
u32 SelectPlan::resolve_expr(const Stmt_s& stmt, Expression_s& expr, bool& have_parent_column)
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
	case ExprStmt::Column:
	{
		ColumnStmt* column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
		if (!resolve_select_list_or_having && column_stmt->is_all_column()) {
			LOG_ERR("* can only exists in select list or count(*)");
			return ERROR_LEX_STMT;
		}
		else if (column_stmt->is_all_column()) {
			return HAVE_ALL_COLUMN_STMT;
		}
		else {
			ColumnDesc col_desc;
			ret = resolve_column_desc(column_stmt, col_desc, have_parent_column);
			if (ret != SUCCESS) {
				break;
			}
			expr = ColumnExpression::make_column_expression(col_desc);
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
			TableStmt* table = nullptr;
			who_have_column(col_desc, table);
			col_expr->table = table;
			ret = SUCCESS;
		}
		break;
	}
	case ExprStmt::Query:
	{
		QueryStmt* query_stmt = dynamic_cast<QueryStmt*>(expr_stmt);
		Plan_s plan = SelectPlan::make_select_plan(parent_table_list, table_list,query_stmt->query_stmt);
		ret = plan->optimizer();
		if (ret != SUCCESS) {
			break;
		}
		subquerys.push_back(plan);
		SelectPlan* subplan = dynamic_cast<SelectPlan*>(plan.get());
		subplan->root_plan = root_plan;
		bool is_correlated = subplan->is_correlated_query();
		//����Ӳ�ѯ�����˵�ǰ��ѯ�ĸ���ѯ�ı�����ͬ����Ҫ���ӵ���ǰ��ѯ�������б���
		for (u32 i = 0; i < subplan->ref_parent_table_list.size(); ++i) {
			TableStmt* table = subplan->ref_parent_table_list[i];
			if (find_table_from_parent(table)) {
				ref_parent_table_list.push_back(table);
			}
		}
		//����Ӳ�ѯ�����˵�ǰ��ѯ�ı����У���ͬ����Ҫ���ӵ���ǰ��ѯ����������
		for (auto iter = subplan->parent_table_access_column.cbegin(); iter != subplan->parent_table_access_column.cend(); ++iter) {
			const Vector<ColumnDesc>& access_column = iter->second;
			for (u32 j = 0; j < access_column.size(); ++j) {
				add_access_column(iter->first, access_column[j]);
			}
		}
		expr = SubplanExpression::make_subplan_expression(plan, is_correlated, query_stmt->to_string());
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
			ConstStmt*const_stmt = dynamic_cast<ConstStmt*>(expr_stmt);
			obj_list->add_object(const_stmt->value);
		}
		expr = ConstExpression::make_const_expression(obj);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Aggregate:
	{
		if (!resolve_select_list_or_having) {
			LOG_ERR("aggregate function can only exist in select list or having stmt");
			return ERROR_LEX_STMT;
		}
		//scalar group��֧��having
		else if (resolve_select_list_or_having == 1 && group_cols.empty()) {
			LOG_ERR("aggregate function can only exist in select list or having stmt");
			return ERROR_LEX_STMT;
		}

		AggrStmt* agg_stmt = dynamic_cast<AggrStmt*>(expr_stmt);
		ret = resolve_expr(agg_stmt->aggr_expr, expr, have_parent_column);
		if (ret == HAVE_ALL_COLUMN_STMT) {
			if (agg_stmt->aggr_func != AggrStmt::COUNT) {
				LOG_ERR("only count aggregation can have * expression");
				return ERROR_LEX_STMT;
			}
			else {
				ret = resolve_all_column_in_count_agg(agg_stmt->aggr_expr, expr);
				if (ret != SUCCESS) {
					return ret;
				}
			}
		}
		else if (ret != SUCCESS) {
			return ret;
		}
		expr = AggregateExpression::make_aggregate_expression(expr, agg_stmt->aggr_func, agg_stmt->distinct);
		aggr_exprs.push_back(expr);
		//Ϊ��ͳһ����ʽ�����ܰѾۺϱ���ʽ�滻��group�����������������Ϊ�ۺϺ���������group���Ӽ���
		ColumnDesc col_desc;
		col_desc.set_tid_cid(alias_table_id, aggr_exprs.size() - 1);
		expr = ColumnExpression::make_column_expression(col_desc);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Unary:
	{
		UnaryExprStmt* unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		Expression_s first_expr;
		ret = resolve_expr(unary_stmt->expr_stmt, first_expr, have_parent_column);
		if (ret != SUCCESS) {
			LOG_ERR("create unary expression`s first expression failed");
			break;
		}
		//������Ӳ�ѯ������в��������͸�д
		if (first_expr->get_type() == Expression::Subplan) {
			SqlRewriter_s sql_rewriter = SqlRewriter::make_sql_rewriter(this, first_expr, Expression_s(), unary_stmt->op_type);
			ret = sql_rewriter->rewrite_for_select(expr);
			if (ret == SUCCESS) {
				break;
			}
			else if (ret == CAN_NOT_REWRITE) {
				//û�и�д�ɹ�����Ҫ���������Ӳ�ѯ�����ν��
				SubplanExpression* subplan_expr = dynamic_cast<SubplanExpression*>(first_expr.get());
				SelectPlan* subquery_plan = dynamic_cast<SelectPlan*>(subplan_expr->subplan.get());
				u32 ret;
				for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
					ret = subquery_plan->resolve_simple_expr(subquery_plan->corrected_predicates[i]);
					if (ret != SUCCESS) {
						break;
					}
				}
			}
			else {
				//meet error
				break;
			}
		}
		expr = UnaryExpression::make_unary_expression(first_expr, unary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Binary:
	{
		BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		Expression_s first_expr, second_expr;
		ret = resolve_expr(binary_stmt->first_expr_stmt, first_expr, have_parent_column);
		if (ret != SUCCESS) {
			LOG_ERR("create binary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(binary_stmt->second_expr_stmt, second_expr, have_parent_column);
		if (ret != SUCCESS) {
			LOG_ERR("create binary expression`s second expression failed");
			break;
		}
		//������Ӳ�ѯ������в��������͸�д
		if (first_expr->get_type() == Expression::Subplan
			&& second_expr->get_type() == Expression::Subplan) {
			LOG_TRACE("subquery op subquery rewrite not support yet");
		}
		else if (first_expr->get_type() == Expression::Subplan) {
			SqlRewriter_s sql_rewriter = SqlRewriter::make_sql_rewriter(this, first_expr, second_expr, binary_stmt->op_type);
			ret = sql_rewriter->rewrite_for_select(expr);
			if (ret == SUCCESS) {
				break;
			}
			else if (ret == CAN_NOT_REWRITE) {
				//û�и�д�ɹ�����Ҫ���������Ӳ�ѯ�����ν��
				SubplanExpression* subplan_expr = dynamic_cast<SubplanExpression*>(first_expr.get());
				SelectPlan* subquery_plan = dynamic_cast<SelectPlan*>(subplan_expr->subplan.get());
				u32 ret;
				for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
					ret = subquery_plan->resolve_simple_expr(subquery_plan->corrected_predicates[i]);
					if (ret != SUCCESS) {
						break;
					}
				}
			}
			else {
				//meet error
				break;
			}
		}
		else if (second_expr->get_type() == Expression::Subplan) {
			SqlRewriter_s sql_rewriter = SqlRewriter::make_sql_rewriter(this, second_expr, first_expr, binary_stmt->op_type);
			ret = sql_rewriter->rewrite_for_select(expr);
			if (ret == SUCCESS) {
				break;
			}
			else if (ret == CAN_NOT_REWRITE) {
				//û�и�д�ɹ�����Ҫ���������Ӳ�ѯ�����ν��
				SubplanExpression* subplan_expr = dynamic_cast<SubplanExpression*>(second_expr.get());
				SelectPlan* subquery_plan = dynamic_cast<SelectPlan*>(subplan_expr->subplan.get());
				u32 ret;
				for (u32 i = 0; i < subquery_plan->corrected_predicates.size(); ++i) {
					ret = subquery_plan->resolve_simple_expr(subquery_plan->corrected_predicates[i]);
					if (ret != SUCCESS) {
						break;
					}
				}
			}
			else {
				//meet error
				break;
			}
		}
		expr = BinaryExpression::make_binary_expression(first_expr, second_expr, binary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	case ExprStmt::Ternary:
	{
		TernaryExprStmt* ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		Expression_s first_expr, second_expr, third_expr;
		ret = resolve_expr(ternary_stmt->first_expr_stmt, first_expr, have_parent_column);
		if (ret != SUCCESS) {
			LOG_ERR("create ternary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->second_expr_stmt, second_expr, have_parent_column);
		if (ret != SUCCESS) {
			LOG_ERR("create ternary expression`s second expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->third_expr_stmt, third_expr, have_parent_column);
		if (ret != SUCCESS) {
			LOG_ERR("create ternary expression`s third expression failed");
			break;
		}
		expr = TernaryExpression::make_ternary_expression(first_expr, second_expr, third_expr, ternary_stmt->op_type);
		ret = SUCCESS;
		break;
	}
	default:
		LOG_ERR("unknown expr stmt in select`s where stmt");
		ret = ERROR_LEX_STMT;
	}
	return ret;
}
u32 SelectPlan::resolve_all_column_in_select_list(const Stmt_s & stmt)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Column) {
		return ERROR_LEX_STMT;
	}
	ColumnStmt* column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
	if (column_stmt->table == "*") {
		SchemaChecker_s checker = SchemaChecker::make_schema_checker();
		assert(checker);
		for (u32 i = 0; i < table_list.size(); ++i) {
			TableStmt* table = table_list[i];
			if (table->is_tmp_table) {
				RowDesc row_desc;
				SelectPlan* plan = dynamic_cast<SelectPlan*>(table->subplan.get());
				if (!plan || plan->get_query_row_desc(row_desc) != SUCCESS) {
					return COLUMN_NOT_EXISTS;
				}
				for (u32 i = 0; i < row_desc.get_column_num(); ++i) {
					ColumnDesc col_desc;
					row_desc.get_column_desc(i, col_desc);
					Expression_s expr = ColumnExpression::make_column_expression(col_desc);
					select_list.push_back(expr);
					ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
					TableStmt* table = nullptr;
					who_have_column(col_desc, table);
					col_expr->table = table;
				}
				const Vector<String>& columns = plan->get_all_output_column();
				for (u32 j = 0; j < columns.size(); ++j) {
					select_list_name.push_back(columns[j]);
				}
			}
			else {
				RowDesc row_desc;
				u32 ret = checker->get_row_desc(table->database, table->table_name, row_desc);
				if (ret != SUCCESS) {
					return ret;
				}
				for (u32 j = 0; j < row_desc.get_column_num(); ++j) {
					ColumnDesc col_desc;
					row_desc.get_column_desc(j, col_desc);
					u32 tid, cid;
					col_desc.get_tid_cid(tid, cid);
					tid = checker->get_table_id(table->database, table->alias_name);
					col_desc.set_tid_cid(tid, cid);
					Expression_s expr = ColumnExpression::make_column_expression(col_desc);
					select_list.push_back(expr);
					add_access_column(table, col_desc);
					ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
					TableStmt* table = nullptr;
					who_have_column(col_desc, table);
					col_expr->table = table;
				}
				Vector<Pair<String, String>> columns;
				ret = checker->desc_table(table->database, table->table_name, columns);
				if (ret != SUCCESS) {
					return ret;
				}
				for (u32 j = 0; j < columns.size(); ++j) {
					select_list_name.push_back(columns[j].first);
				}
			}
		}
	}
	else {
		TableStmt* table = nullptr;
		u32 ret = find_table(column_stmt->table, table);
		if (ret != SUCCESS) {
			return ret;
		}
		if (table->is_tmp_table) {
			RowDesc row_desc;
			SelectPlan* plan = dynamic_cast<SelectPlan*>(table->subplan.get());
			if (!plan || plan->get_query_row_desc(row_desc) != SUCCESS) {
				return COLUMN_NOT_EXISTS;
			}
			for (u32 i = 0; i < row_desc.get_column_num(); ++i) {
				ColumnDesc col_desc;
				row_desc.get_column_desc(i, col_desc);
				Expression_s expr = ColumnExpression::make_column_expression(col_desc);
				select_list.push_back(expr);
				ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
				TableStmt* table = nullptr;
				who_have_column(col_desc, table);
				col_expr->table = table;
			}
			const Vector<String>& columns = plan->get_all_output_column();
			for (u32 j = 0; j < columns.size(); ++j) {
				select_list_name.push_back(columns[j]);
			}
		}
		else {
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			assert(checker);
			RowDesc row_desc;
			ret = checker->get_row_desc(table->database, table->table_name, row_desc);
			if (ret != SUCCESS) {
				return ret;
			}
			for (u32 i = 0; i < row_desc.get_column_num(); ++i) {
				ColumnDesc col_desc;
				row_desc.get_column_desc(i, col_desc);
				u32 tid, cid;
				col_desc.get_tid_cid(tid, cid);
				tid = checker->get_table_id(table->database, table->alias_name);
				col_desc.set_tid_cid(tid, cid);
				Expression_s expr = ColumnExpression::make_column_expression(col_desc);
				select_list.push_back(expr);
				add_access_column(table, col_desc);
				ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
				TableStmt* table = nullptr;
				who_have_column(col_desc, table);
				col_expr->table = table;
			}
			Vector<Pair<String, String>> columns;
			ret = checker->desc_table(table->database, table->table_name, columns);
			if (ret != SUCCESS) {
				return ret;
			}
			for (u32 j = 0; j < columns.size(); ++j) {
				select_list_name.push_back(columns[j].first);
			}
		}
	}
	return SUCCESS;
}
u32 SelectPlan::resolve_all_column_in_count_agg(const Stmt_s & stmt, Expression_s& expr)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Column) {
		return ERROR_LEX_STMT;
	}
	expr.reset();
	return SUCCESS;
}
/*
 * ��ǰν�������Ƿ��ǻ�������ν�ʣ�
 * ���ν����ֻ������һ�ű����л��߶��ǳ�����
 * ���ʾ�ǻ�������ν�ʣ������ػ���
 */
bool SelectPlan::is_table_filter(const Stmt_s & stmt, TableStmt *& table)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return false;
	}

	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	bool ret = true;

	switch (expr_stmt->expr_stmt_type())
	{
	case ExprStmt::Const:
	{
		ret = true;
		break;
	}
	case ExprStmt::Column:
	{
		ColumnStmt* column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
		TableStmt *table_belong = nullptr;
		u32 code = who_have_column(column_stmt, table_belong);
		if (code != SUCCESS) {
			ret = false;
			break;
		}
		if (find_table_from_parent(table_belong)) {
			ret = false;
			break;
		}
		//�ڴ�֮ǰû�г��ֹ������ν��
		if (table == nullptr) {
			table = table_belong;
			ret = true;
		}
		//�ڴ�֮ǰ���ֹ������ν��
		else if(table != table_belong){
			ret = false;
		}
		else {
			ret = true;
		}
		break;
	}
	case ExprStmt::List:
	{
		ret = true;
		break;
	}
	case ExprStmt::Unary:
	{
		UnaryExprStmt* unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		ret = is_table_filter(unary_stmt->expr_stmt, table);
		break;
	}
	case ExprStmt::Binary:
	{
		BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		ret = is_table_filter(binary_stmt->first_expr_stmt, table);
		if (!ret) {
			break;
		}
		else {
			ret = is_table_filter(binary_stmt->second_expr_stmt, table);
		}
		break;
	}
	case ExprStmt::Ternary:
	{
		TernaryExprStmt* ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		ret = is_table_filter(ternary_stmt->first_expr_stmt, table);
		if (!ret) {
			break;
		}
		else {
			ret = is_table_filter(ternary_stmt->second_expr_stmt, table);
			if (!ret) {
				break;
			}
			else {
				ret = is_table_filter(ternary_stmt->third_expr_stmt, table);
			}
		}
		break;
	}
	default:
		ret = false;
	}
	return ret;
}

bool SelectPlan::is_table_filter(const Expression_s & expr, TableStmt *& table)
{
	bool ret = false;
	switch (expr->get_type())
	{
	case Expression::Const:
	{
		ret = true;
		break;
	}
	case Expression::Column:
	{
		ColumnExpression* column_expr = dynamic_cast<ColumnExpression*>(expr.get());
		TableStmt *table_belong = nullptr;
		u32 code = who_have_column(column_expr->col_desc, table_belong);
		if (code != SUCCESS) {
			ret = false;
			break;
		}
		if (find_table_from_parent(table_belong)) {
			ret = false;
			break;
		}
		//�ڴ�֮ǰû�г��ֹ������ν��
		if (table == nullptr) {
			table = table_belong;
			ret = true;
		}
		//�ڴ�֮ǰ���ֹ������ν��
		else if (table != table_belong) {
			ret = false;
		}
		else {
			ret = true;
		}
		break;
	}
	case Expression::Subplan:
	{
		SubplanExpression* subplan_expr = dynamic_cast<SubplanExpression*>(expr.get());
		if (subplan_expr->subplan->type() == Plan::SELECT) {
			SelectPlan* select_plan = dynamic_cast<SelectPlan*>(subplan_expr->subplan.get());
			ret = !select_plan->is_correlated_query();
		}
		else {
			ret = false;
		}
		break;
	}
	case Expression::Unary:
	{
		UnaryExpression* unary_expr = dynamic_cast<UnaryExpression*>(expr.get());
		ret = is_table_filter(unary_expr->expr, table);
		break;
	}
	case Expression::Binary:
	{
		BinaryExpression* binary_expr = dynamic_cast<BinaryExpression*>(expr.get());
		ret = is_table_filter(binary_expr->first_expr, table);
		if (!ret) {
			break;
		}
		else {
			ret = is_table_filter(binary_expr->second_expr, table);
		}
		break;
	}
	case Expression::Ternary:
	{
		TernaryExpression* ternary_expr = dynamic_cast<TernaryExpression*>(expr.get());
		ret = is_table_filter(ternary_expr->first_expr, table);
		if (!ret) {
			break;
		}
		else {
			ret = is_table_filter(ternary_expr->second_expr, table);
			if (!ret) {
				break;
			}
			else {
				ret = is_table_filter(ternary_expr->third_expr, table);
			}
		}
		break;
	}
	default:
		ret = false;
	}
	return ret;
}

/*
 * ��ȡ������
 */
u32 SelectPlan::resolve_column_desc(ColumnStmt * column_stmt, ColumnDesc & col_desc, bool& from_partent)
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	TableStmt* table = nullptr;
	u32 ret = who_have_column(column_stmt, table);
	if (ret != SUCCESS) {
		return ret;
	}
	//������õ�����������ѯ�ı���ע��
	if (find_table_from_parent(table)) {
		if (!is_resolve_where) {
			return ERROR_LEX_STMT;
		}
		ref_parent_table_list.push_back(table);
		from_partent = true;
	}
	if (table->is_tmp_table) {
		SelectPlan* plan = dynamic_cast<SelectPlan*>(table->subplan.get());
		if (!plan || plan->get_column_from_select_list(column_stmt->column, col_desc) != SUCCESS) {
			return COLUMN_NOT_EXISTS;
		}
	}
	else {
		ret = checker->get_column_desc(table->database, table->table_name, column_stmt->column, col_desc);
		if (ret != SUCCESS) {
			return ret;
		}
		u32 tid, cid;
		col_desc.get_tid_cid(tid, cid);
		tid = checker->get_table_id(table->database, table->alias_name);
		col_desc.set_tid_cid(tid, cid);
		add_access_column(table, col_desc);
	}
	return SUCCESS;
}

/*
 * ��from list������ָ�������ظ����ֱ�ʾfrom list����
 */
u32 SelectPlan::find_table(const String & table_name, TableStmt*& table)
{
	u32 find = 0;
	for (u32 i = 0; i < table_list.size(); ++i) {
		//������õı�û��ʹ�ñ������ڽ�����ʱ���ʹ����ʵ������ʾ����
		//��������ֻ��Ҫ�������ı���
		if (table_name == table_list[i]->alias_name) {
			++find;
			table = table_list[i];
		}
	}
	if (find == 1)
		return SUCCESS;
	else if (find == 0)
		return TABLE_NOT_EXISTS;
	else
		return TABLE_REDEFINE;
}

u32 SelectPlan::find_table_from_parent(const String & table_name, TableStmt *& table)
{
	u32 find = 0;
	//�ڸ���ѯ������
	for (u32 i = 0; i < parent_table_list.size(); ++i) {
		//������õı�û��ʹ�ñ������ڽ�����ʱ���ʹ����ʵ������ʾ����
		//��������ֻ��Ҫ�������ı���
		if (table_name == parent_table_list[i]->alias_name) {
			++find;
			table = parent_table_list[i];
		}
	}
	if (find == 1)
		return SUCCESS;
	else if (find == 0)
		return TABLE_NOT_EXISTS;
	else
		return TABLE_REDEFINE;
}

bool SelectPlan::find_table_from_parent(TableStmt * table)
{
	for (u32 i = 0; i < parent_table_list.size(); ++i) {
		//������õı�û��ʹ�ñ������ڽ�����ʱ���ʹ����ʵ������ʾ����
		//��������ֻ��Ҫ�������ı���
		if (table == parent_table_list[i]) {
			return true;
		}
	}
	return false;
}

/*
 * ��from list����������ָ���еı�
 */
u32 SelectPlan::who_have_column(ColumnStmt * column_stmt, TableStmt *& table)
{
	//SQL��û��ָ���������ı�
	if (column_stmt->table.empty()) {
		u32 ret = who_have_column(column_stmt->column, table);
		if (ret != SUCCESS) {
			ret = which_partent_table_have_column(column_stmt->column, table);
			if (ret != SUCCESS) {
				LOG_ERR("parse column define error in where stmt", K(column_stmt));
				return ret;
			}
		}
	}
	//SQL��ָ�����������ı�
	else {
		u32 ret = find_table(column_stmt->table, table);
		if (ret == TABLE_NOT_EXISTS) {
			ret = find_table_from_parent(column_stmt->table, table);
			if (ret != SUCCESS) {
				LOG_ERR("parse column define error in where stmt", K(column_stmt));
				return ret;
			}
		}
		else if (ret != SUCCESS) {
			LOG_ERR("parse column define error in where stmt", K(column_stmt));
			return ret;
		}
	}
	return SUCCESS;
}
/*
 * ��from list����������ָ���еı�
 */
u32 SelectPlan::who_have_column(const String & column_name, TableStmt*& table)
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 find = 0;
	for (u32 i = 0; i < table_list.size(); ++i) {
		if (table_list[i]->is_tmp_table) {
			SelectPlan* plan = dynamic_cast<SelectPlan*>(table_list[i]->subplan.get());
			ColumnDesc col_desc;
			if (plan && !find && plan->get_column_from_select_list(column_name, col_desc) == SUCCESS) {
				++find;
				table = table_list[i];
			}
		}
		else if (checker->have_column(table_list[i]->database, table_list[i]->table_name, column_name)) {
			++find;
			table = table_list[i];
		}
	}
	if (find == 1)
		return SUCCESS;
	else if (find == 0)
		return COLUMN_NOT_EXISTS;
	else
		return TABLE_REDEFINE;
}
u32 SelectPlan::who_have_column(const ColumnDesc & col_desc, TableStmt *& table)
{
	u32 tid, cid;
	col_desc.get_tid_cid(tid, cid);
	u32 find = 0;
	for (u32 i = 0; i < table_list.size(); ++i) {
		if (table_list[i]->table_id == tid) {
			++find;
			table = table_list[i];
		}
	}
	if (find == 1)
		return SUCCESS;
	else if (find > 1)
		return TABLE_REDEFINE;
	//��ǰ��ѯ��û���ҵ�����������ϲ㸸��ѯ����
	find = 0;
	for (u32 i = 0; i < parent_table_list.size(); ++i) {
		if (parent_table_list[i]->table_id == tid) {
			++find;
			table = parent_table_list[i];
		}
	}
	if (find == 1)
		return SUCCESS;
	else if (find == 0)
		return COLUMN_NOT_EXISTS;
	else
		return TABLE_REDEFINE;
}
u32 SelectPlan::which_partent_table_have_column(const String & column_name, TableStmt *& table)
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 find = 0;
	for (u32 i = 0; i < parent_table_list.size(); ++i) {
		if (parent_table_list[i]->is_tmp_table) {
			SelectPlan* plan = dynamic_cast<SelectPlan*>(parent_table_list[i]->subplan.get());
			ColumnDesc col_desc;
			if (plan && plan->get_column_from_select_list(column_name, col_desc) == SUCCESS) {
				++find;
				table = parent_table_list[i];
			}
		}
		else if (checker->have_column(parent_table_list[i]->database, parent_table_list[i]->table_name, column_name)) {
			++find;
			table = parent_table_list[i];
		}
	}
	if (find == 1)
		return SUCCESS;
	else if (find == 0)
		return COLUMN_NOT_EXISTS;
	else
		return TABLE_REDEFINE;
}
/*
 * ��from �����н��������������˵ı�
 */
u32 SelectPlan::get_ref_tables(const Stmt_s & from_stmt)
{
	if (from_stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(from_stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		return ERROR_LEX_STMT;
	}
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr_stmt);
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
		if (list_stmt->stmt_list[i]->stmt_type() != Stmt::Expr) {
			return ERROR_LEX_STMT;
		}
		ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(list_stmt->stmt_list[i].get());
		if (expr_stmt->expr_stmt_type() == ExprStmt::Query) {
			QueryStmt* query = dynamic_cast<QueryStmt*>(expr_stmt);
			u32 ret = get_ref_table_from_query(query);
			if (ret != SUCCESS) {
				return ret;
			}
		}
		else if (expr_stmt->expr_stmt_type() != ExprStmt::Table) {
			return ERROR_LEX_STMT;
		}
		else {
			TableStmt* table = dynamic_cast<TableStmt*>(expr_stmt);
			TableStmt* exist_table = nullptr;
			if (find_table(table->alias_name, exist_table) == SUCCESS) {
				LOG_ERR("same alias table in from list", K(table));
				return NOT_UNIQUE_TABLE;
			}
			else {
				table->table_id = checker->get_table_id(table->database, table->alias_name);
				table_list.push_back(table);
			}
		}
	}
	return SUCCESS;
}

u32 SelectPlan::get_ref_table_from_query(QueryStmt * subquery)
{
	//������ʱ��
	TableStmt* table = nullptr;
	if (find_table(subquery->alias_name, table) == SUCCESS) {
		return TABLE_EXISTS;
	}
	Stmt_s stmt = TableStmt::make_table_stmt(subquery->alias_name);
	tmp_table_handle.push_back(stmt);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	//������ʱ����ʵ�ʲ�ѯ�ƻ�
	Plan_s plan = SelectPlan::make_select_plan(subquery->query_stmt);
	if (!plan) {
		return ERROR_LEX_STMT;
	}
	//������ʱ��ID
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	SelectPlan* tmp_table = dynamic_cast<SelectPlan*>(plan.get());
	u32 table_id = checker->get_table_id(table->database, table->alias_name);
	tmp_table->root_plan = root_plan;
	tmp_table->set_alias_table_id(table_id);
	//�����Ӳ�ѯ
	u32 ret = plan->optimizer();
	if (ret != SUCCESS) {
		return ret;
	}
	ret = plan->build_plan();
	if (ret != SUCCESS) {
		LOG_ERR("resolve subquery in from list failed");
		return ret;
	}
	table->subplan = plan;
	table->table_id = table_id;
	table_list.push_back(table);
	return SUCCESS;
}
/*
 * ����ָ�����ű�������ν��
 */
u32 SelectPlan::search_jon_info(const JoinableTables & join_tables, JoinConditions & join_cond)
{
	u32 ret = JOIN_TABLES_NOT_EXISTS;
	for (auto iter = join_info.begin(); iter != join_info.end(); ++iter) {
		if (iter->first.first->alias_name == join_tables.first->alias_name
			&& iter->first.second->alias_name == join_tables.second->alias_name) {
			join_cond = iter->second;
			ret = SUCCESS;
			break;
		}
		else if (iter->first.second->alias_name == join_tables.first->alias_name
			&& iter->first.first->alias_name == join_tables.second->alias_name) {
			join_cond = iter->second;
			ret = SUCCESS;
			break;
		}
	}
	return ret;
}
u32 SelectPlan::search_jon_info(const JoinableTables & join_tables, JoinConditions *& join_cond)
{
	u32 ret = JOIN_TABLES_NOT_EXISTS;
	for (auto iter = join_info.begin(); iter != join_info.end(); ++iter) {
		if (iter->first.first->alias_name == join_tables.first->alias_name
			&& iter->first.second->alias_name == join_tables.second->alias_name) {
			join_cond = &(iter->second);
			ret = SUCCESS;
			break;
		}
		else if (iter->first.second->alias_name == join_tables.first->alias_name
			&& iter->first.first->alias_name == join_tables.second->alias_name) {
			join_cond = &(iter->second);
			ret = SUCCESS;
			break;
		}
	}
	return ret;
}
/*
 * ����ָ�����ű�������ν��
 */
u32 SelectPlan::add_join_cond(const JoinableTables & join_tables, const Expression_s & expr)
{
	JoinConditions* join_cond = nullptr;
	u32 ret = search_jon_info(join_tables, join_cond);
	if (ret == SUCCESS) {
		make_and_expression(join_cond->first, expr);
	}
	else {
		join_info[join_tables] = JoinConditions(expr, Expression_s());
	}
	return SUCCESS;
}
/*
 * ����ָ�����ű��ĵ�ֵ����ν��
 */
u32 SelectPlan::add_join_equal_cond(const JoinableTables & join_tables, const Expression_s & expr)
{
	JoinConditions* join_cond = nullptr;
	u32 ret = search_jon_info(join_tables, join_cond);
	if (ret == SUCCESS) {
		make_and_expression(join_cond->second, expr);
	}
	else {
		join_info[join_tables] = JoinConditions(Expression_s(), expr);
	}
	return SUCCESS;
}
/*
 * ���ӻ�������ν��
 */
u32 SelectPlan::add_table_filter(TableStmt * table, const Expression_s & filter)
{
	if (!table) {
		for (u32 i = 0; i < table_list.size(); ++i) {
			add_table_filter(table_list[i], filter);
		}
	}
	else {
		if (table_filters.find(table) == table_filters.cend()) {
			table_filters[table] = filter;
		}
		else {
			make_and_expression(table_filters[table], filter);
		}
	}
	return SUCCESS;
}
/*
 * ���ܣ�expr = expr and other
 */
u32 SelectPlan::make_and_expression(Expression_s & expr, const Expression_s & other)
{
	if (!other) {
		return SUCCESS;
	}
	else if (!expr) {
		expr = other;
		return SUCCESS;
	}
	Expression_s and_expr = BinaryExpression::make_binary_expression(expr, other, ExprStmt::OP_AND);
	expr = and_expr;
	return SUCCESS;
}

u32 SelectPlan::choos_best_join_order()
{
	u32 end = 0;
	while (end < table_list.size()) {
		if (table_list[end]->join_type != JoinPhyOperator::Join) {
			break;
		}
		++end;
	}

	for (u32 k = 1; k < end; ++k) {
		bool found = false;
		for (u32 i = k; i < end; ++i) {
			TableStmt* right_table = table_list[i];
			Expression_s join_cond, join_equal_cond;
			//������ǰ���������ɼƻ��ı��Ƿ�����������
			for (u32 j = 0; j < i; ++j) {
				TableStmt* left_table = table_list[j];
				JoinableTables joinable_table(left_table, right_table);
				JoinConditions condition;
				u32 ret = search_jon_info(joinable_table, condition);
				//���ű�û��join����
				if (ret != SUCCESS) {
					continue;
				}
				//�ϲ���������
				make_and_expression(join_cond, condition.first);
				make_and_expression(join_equal_cond, condition.second);
			}
			if (join_equal_cond) {
				TableStmt* tmp = table_list[k];
				table_list[k] = table_list[i];
				table_list[i] = tmp;
				found = true;
				break;
			}
		}
	}
	return SUCCESS;
}

u32 SelectPlan::resolve_group_stmt(const Stmt_s & group_stmt)
{
	if (!group_stmt) {
		return SUCCESS;
	}
	else if (group_stmt->stmt_type() != Stmt::Expr) {
		LOG_ERR("error stmt for sort columns");
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(group_stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		return ERROR_LEX_STMT;
	}
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr_stmt);
	for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
		if (list_stmt->stmt_list[i]->stmt_type() != Stmt::Expr) {
			return ERROR_LEX_STMT;
		}
		ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(list_stmt->stmt_list[i].get());
		if (expr_stmt->expr_stmt_type() != ExprStmt::Column) {
			return ERROR_LEX_STMT;
		}
		Expression_s column;
		bool have_parent_column = false;
		u32 ret = resolve_expr(list_stmt->stmt_list[i], column, have_parent_column);
		if (ret != SUCCESS) {
			return ret;
		}
		else if (have_parent_column) {
			return ERROR_LEX_STMT;
		}
		group_cols.push_back(column);
	}
	return SUCCESS;
}

u32 SelectPlan::resolve_having_stmt(const Stmt_s & having_stmt)
{
	if (!having_stmt) {
		return SUCCESS;
	}
	resolve_select_list_or_having = 1;
	bool have_parent_column = false;
	u32 ret = resolve_expr(having_stmt, having_filter, have_parent_column);
	resolve_select_list_or_having = 0;
	return ret;
}

u32 SelectPlan::resolve_limit_stmt(const Stmt_s& limit_stmt)
{
	if (!limit_stmt) {
		return SUCCESS;
	}
	else if (limit_stmt->stmt_type() != Stmt::Limit) {
		LOG_ERR("error stmt for limit");
		return ERROR_LEX_STMT;
	}
	LimitStmt* limit = dynamic_cast<LimitStmt*>(limit_stmt.get());
	limit_offset = limit->limit_offset;
	limit_size = limit->limit_size;
	return SUCCESS;
}

u32 SelectPlan::resolve_sort_stmt(const Stmt_s& sort_stmt)
{
	if (!sort_stmt) {
		return SUCCESS;
	}
	else if (sort_stmt->stmt_type() != Stmt::Expr) {
		LOG_ERR("error stmt for sort columns");
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(sort_stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		return ERROR_LEX_STMT;
	}
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr_stmt);
	u32 ret = SUCCESS;
	for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
		if (list_stmt->stmt_list[i]->stmt_type() != Stmt::Expr) {
			return ERROR_LEX_STMT;
		}
		ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(list_stmt->stmt_list[i].get());
		if (expr_stmt->expr_stmt_type() != ExprStmt::Column) {
			return ERROR_LEX_STMT;
		}
		Expression_s column;
		//��select list�в���������
		ret = resolve_column_from_select_list(list_stmt->stmt_list[i], column);
		if (ret == SUCCESS) {
			is_sort_query_result = true;
		}
		else {
			//�����select list��û���ҵ������У��ٴӻ����в���
			bool have_parent_column = false;
			ret = resolve_expr(list_stmt->stmt_list[i], column, have_parent_column);
			if (ret != SUCCESS) {
				return ret;
			}
			else if (is_sort_query_result) {
				return ERROR_LEX_STMT;
			}
			else if (have_parent_column) {
				return ERROR_LEX_STMT;
			}
		}
		sort_cols.push_back(column);
	}
	return ret;
}

u32 SelectPlan::resolve_select_list(const Stmt_s & select_list)
{
	if (!select_list) {
		LOG_ERR("select list can not be empty");
		return ERROR_LEX_STMT;
	}
	else if (select_list->stmt_type() != Stmt::Expr) {
		LOG_ERR("error stmt for select columns");
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(select_list.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::List) {
		return ERROR_LEX_STMT;
	}
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr_stmt);
	resolve_select_list_or_having = 2;
	u32 ret = SUCCESS;
	for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
		Expression_s expr;
		bool have_parent_column = false;
		ret = resolve_expr(list_stmt->stmt_list[i], expr, have_parent_column);
		if (ret == HAVE_ALL_COLUMN_STMT) {
			ret = resolve_all_column_in_select_list(list_stmt->stmt_list[i]);
			if (ret != SUCCESS) {
				break;
			}
		}
		else if (ret != SUCCESS) {
			break;
		}
		else if (have_parent_column) {
			ret = ERROR_LEX_STMT;
			break;
		}
		else {
			ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(list_stmt->stmt_list[i].get());
			select_list_name.push_back(expr_stmt->alias_name);
			this->select_list.push_back(expr);
		}
	}
	resolve_select_list_or_having = 0;
	return ret;
}

u32 SelectPlan::resolve_column_from_select_list(const Stmt_s& stmt, Expression_s& expr)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}
	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	if (expr_stmt->expr_stmt_type() != ExprStmt::Column) {
		return ERROR_LEX_STMT;
	}
	ColumnStmt*column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
	ColumnDesc col_desc;
	u32 ret = get_column_from_select_list(column_stmt->column, col_desc);
	if (ret != SUCCESS) {
		return ret;
	}
	expr = ColumnExpression::make_column_expression(col_desc);
	return SUCCESS;
}

u32 SelectPlan::add_access_column(TableStmt* table, const ColumnDesc& col_desc)
{
	if (find_table_from_parent(table)) {
		if (parent_table_access_column.find(table) == parent_table_access_column.cend()) {
			Vector<ColumnDesc> access_columns;
			access_columns.push_back(col_desc);
			parent_table_access_column[table] = access_columns;
		}
		else {
			bool find = false;
			Vector<ColumnDesc>& access_columns = parent_table_access_column[table];
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
	}
	else {
		if (table_access_column.find(table) == table_access_column.cend()) {
			Vector<ColumnDesc> access_columns;
			access_columns.push_back(col_desc);
			table_access_column[table] = access_columns;
		}
		else {
			bool find = false;
			Vector<ColumnDesc>& access_columns = table_access_column[table];
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
	}
	return SUCCESS;
}

/*
 * Ϊÿһ����Ҫ���ʵı���������������
 * ����Ҫ������Щ��
 */
u32 SelectPlan::make_access_row_desc()
{
	u32 size = table_list.size();
	for (u32 i = 0; i < size; ++i) {
		if (table_access_column.find(table_list[i]) != table_access_column.cend())  {
			const Vector<ColumnDesc>& access_columns = table_access_column[table_list[i]];
			RowDesc row_desc(access_columns.size());
			for (u32 j = 0; j < access_columns.size(); ++j) {
				row_desc.set_column_desc(j, access_columns[j]);
			}
			table_access_row_desc[table_list[i]] = row_desc;
		}
		else {
			table_access_row_desc[table_list[i]] = RowDesc();
		}
	}
	return SUCCESS;
}

u32 SelectPlan::make_join_plan(PhyOperator_s & op)
{
	u32 ret = SUCCESS;
	if (table_list.size() == 0) {
		LOG_ERR("from list must have more than one table");
		ret = NO_TABLE_FOR_SELECT;
	}
	else if (table_list.size() == 1) {
		ret = make_table_scan(table_list[0], op);
	}
	else {
		PhyOperator_s left_root_operator;
		u32 ret = make_table_scan(table_list[0], left_root_operator);
		if (ret != SUCCESS) {
			return ret;
		}
		//Ϊtable list��ÿһ�ű�����join�ƻ�
		for (u32 i = 1; i < table_list.size(); ++i) {
			TableStmt* right_table = table_list[i];
			Expression_s join_cond, join_equal_cond;
			//������ǰ���������ɼƻ��ı��Ƿ�����������
			for (u32 j = 0; j < i; ++j) {
				TableStmt* left_table = table_list[j];
				JoinableTables joinable_table(left_table, right_table);
				JoinConditions condition;
				u32 ret = search_jon_info(joinable_table, condition);
				//���ű�û��join����
				if (ret != SUCCESS) {
					continue;
				}
				//�ϲ���������
				make_and_expression(join_cond, condition.first);
				make_and_expression(join_equal_cond, condition.second);
			}
			PhyOperator_s right_op;
			u32 ret = make_table_scan(right_table, right_op);
			if (ret != SUCCESS) {
				LOG_ERR("make table scan for table failed");
				return ret;
			}
			double out_rows = 0;
			if (root_plan->is_explain) {
				out_rows = cal_join_select_rows(left_root_operator->output_rows, right_op->output_rows, join_cond);
			}
			//û�е�ֵ��������������ֻ��ѡ��nested loop�㷨
			//Ŀǰֻ��nested loop�㷨֧��anti join
			if (right_table->join_type == JoinPhyOperator::AntiJoin) {
				SchemaChecker_s checker = SchemaChecker::make_schema_checker();
				assert(checker);
				u32 id = checker->get_table_id(right_table->database, right_table->alias_name);
				left_root_operator = HashNullAwareAntiJoin::make_hash_null_aware_anti_join(
					left_root_operator,
					right_op,
					right_table->anti_cond,
					join_equal_cond,
					join_cond,
					id);
				//left_root_operator = NestedLoopJoin::make_nested_loop_join(left_root_operator, right_op, join_cond);
			}
			else if (!join_equal_cond) {
				left_root_operator = NestedLoopJoin::make_nested_loop_join(left_root_operator, right_op, join_cond);
			}
			else {
				SchemaChecker_s checker = SchemaChecker::make_schema_checker();
				assert(checker);
				u32 id = checker->get_table_id(right_table->database, right_table->alias_name);
				left_root_operator = HashJoin::make_hash_join(left_root_operator, 
					right_op, join_equal_cond, join_cond, id);
			}
			left_root_operator->output_rows = out_rows;
			JoinPhyOperator* join_op = dynamic_cast<JoinPhyOperator*>(left_root_operator.get());
			switch (right_table->join_type) {
			case JoinPhyOperator::Join:
				join_op->set_join_type(JoinPhyOperator::Join);
				break;
			case JoinPhyOperator::SemiJoin:
				join_op->set_join_type(JoinPhyOperator::SemiJoin);
				break;
			case JoinPhyOperator::AntiJoin:
				join_op->set_join_type(JoinPhyOperator::AntiJoin);
				break;
			case JoinPhyOperator::LeftOuterJoin:
				join_op->set_join_type(JoinPhyOperator::LeftOuterJoin);
				break;
			case JoinPhyOperator::RightOuterJoin:
				join_op->set_join_type(JoinPhyOperator::RightOuterJoin);
				break;
			case JoinPhyOperator::FullOuterJoin:
				join_op->set_join_type(JoinPhyOperator::FullOuterJoin);
				break;
			}
		}
		op = left_root_operator;
		ret = SUCCESS;
	}
	if (filter_after_join) {
		Filter_s filter = Filter::make_filter(filter_after_join);
		op = PlanFilter::make_plan_filter(op, filter);
	}
	return ret;
}

u32 SelectPlan::make_table_scan(TableStmt * table, PhyOperator_s & op)
{
	Expression_s filter_expr;
	Filter_s filter;
	if (table_filters.find(table) != table_filters.cend()) {
		filter_expr = table_filters[table];
		filter = Filter::make_filter(filter_expr);
	}
	if (root_plan->is_explain) {
		cal_table_select_rows(table, filter_expr);
	}
	if (table->is_tmp_table) {
		SelectPlan* plan = dynamic_cast<SelectPlan*>(table->subplan.get());
		if (!plan) {
			return ERROR_LEX_STMT;
		}
		if (!filter) {
			op = plan->get_root_operator();
		}
		else {
			op = plan->get_root_operator();
			op = PlanFilter::make_plan_filter(op, filter);
			op->output_rows = plan->get_select_rows();
		}
	}
	else {
		op = TableScan::make_table_scan(table->database,
										table->table_name,
										table_access_row_desc[table],
										filter,
										query_ctx.sample_size);
		TableScan* table_scan = dynamic_cast<TableScan*>(op.get());
		table_scan->set_alias_table_name(table->alias_name);
		table_scan->output_rows = table->select_rows;
	}
	return SUCCESS;
}

u32 SelectPlan::make_group_pan(PhyOperator_s & op)
{
	//scalar group by
	if (group_cols.size() == 0) {
		//û���κ���Ҫ����ľۺϺ���
		if (aggr_exprs.size() == 0) {
			//����Ҫ�ۺ�����
			if (having_filter) {
				LOG_ERR("can not have having filter when there is no group operation");
				return HAVING_ERROR;
			}
			else {
				return SUCCESS;
			}
		}
		else  {
			Filter_s filter = Filter::make_filter(having_filter);
			op = ScalarGroup::make_scalar_group(op, filter);
			ScalarGroup* scalar_group = dynamic_cast<ScalarGroup*>(op.get());
			for (u32 i = 0; i < aggr_exprs.size(); ++i) {
				scalar_group->add_agg_expr(aggr_exprs[i]);
			}
			scalar_group->set_alias_table_id(alias_table_id);
			op->output_rows = 1;
			return SUCCESS;
		}
	}
	else {
		Filter_s filter = Filter::make_filter(having_filter);
		double out_rows = op->output_rows;
		for (u32 i = 0; i < group_cols.size(); ++i) {
			out_rows = scale_ndv(group_cols[i], out_rows);
		}
		if (filter) {
			out_rows *= 1.0 / 3.0;
		}
		op = HashGroup::make_hash_group(op, group_cols, filter);
		HashGroup* hash_group = dynamic_cast<HashGroup*>(op.get());
		for (u32 i = 0; i < aggr_exprs.size(); ++i) {
			hash_group->add_agg_expr(aggr_exprs[i]);
		}
		hash_group->set_agg_table_id(alias_table_id);
		op->output_rows = out_rows;
		return SUCCESS;
	}
}

u32 SelectPlan::make_sort_plan(PhyOperator_s & op)
{
	if (sort_cols.size() == 0) {
		return SUCCESS;
	}
	else {
		//limit 30000�����п�����top-N����,�����distinct���ܸ�д
		if (!is_distinct && have_limit && limit_size + limit_offset < 30000) {
			op = TopNSort::make_topn_sort(op, sort_cols, limit_size + limit_offset, asc);
			op->output_rows = limit_size;
			//limitû��ƫ������Ҫ��ִ��һ��limit����
			if (limit_offset == 0) {
				have_limit = false;
			}
		}
		else {
			double out_rows = op->output_rows;
			op = Sort::make_sort(op, sort_cols, asc);
			op->output_rows = out_rows;
		}
		return SUCCESS;
	}
}

u32 SelectPlan::make_query_plan(PhyOperator_s & op)
{
	double out_rows = op->output_rows;
	op = Query::make_query(op, select_list);
	op->output_rows = out_rows;
	Query* query = dynamic_cast<Query*>(op.get());
	query->set_alias_table_id(alias_table_id);
	return SUCCESS;
}

u32 SelectPlan::make_distinct_plan(PhyOperator_s & op)
{
	if (is_distinct) {
		double out_rows = op->output_rows;
		for (u32 i = 0; i < select_list.size(); ++i) {
			out_rows = scale_ndv(select_list[i], out_rows);
		}
		op = HashDistinct::make_hash_distinct(op);
		op->output_rows = out_rows;
	}
	return SUCCESS;
}

u32 SelectPlan::make_limit_plan(PhyOperator_s & op)
{
	if (have_limit) {
		op = Limit::make_limit(op, limit_size, limit_offset);
		op->output_rows = limit_size;
	}
	return SUCCESS;
}

u32 SelectPlan::make_set_plan(PhyOperator_s & op)
{
	u32 ret = ERROR_LEX_STMT;
	ExprStmt* expr = dynamic_cast<ExprStmt*>(lex_stmt.get());
	if (expr->expr_stmt_type() != ExprStmt::Binary) {
		return ret;
	}
	BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr);
	first_plan = SelectPlan::make_select_plan(binary_stmt->first_expr_stmt);
	if (!first_plan) {
		return ret;
	}
	ret = first_plan->optimizer();
	if (ret != SUCCESS) {
		return ret;
	}
	ret = first_plan->build_plan();
	if (ret != SUCCESS) {
		return ret;
	}
	second_plan = SelectPlan::make_select_plan(binary_stmt->second_expr_stmt);
	if (!second_plan) {
		return ret;
	}
	ret = second_plan->optimizer();
	if (ret != SUCCESS) {
		return ret;
	}
	ret = second_plan->build_plan();
	if (ret != SUCCESS) {
		return ret;
	}
	SelectPlan* first_select = dynamic_cast<SelectPlan*>(first_plan.get());
	SelectPlan* second_select = dynamic_cast<SelectPlan*>(second_plan.get());
	RowDesc first_row_desc, second_row_desc;
	first_select->get_query_row_desc(first_row_desc);
	second_select->get_query_row_desc(second_row_desc);
	if (first_row_desc.get_column_num() != second_row_desc.get_column_num()) {
		return SET_ROW_DESC_ERROR;
	}

	PhyOperator_s first_op = first_plan->get_root_operator();
	PhyOperator_s second_op = second_plan->get_root_operator();
	switch (binary_stmt->op_type)
	{
	case ExprStmt::OP_UNION:
	{
		root_operator = HashUnion::make_hash_union(first_op, second_op);
		root_operator->output_rows = first_op->output_rows + second_op->output_rows;
		ret = SUCCESS;
		break;
	}
	case ExprStmt::OP_UNION_ALL:
	{
		root_operator = UnionAll::make_union_all(first_op, second_op);
		root_operator->output_rows = first_op->output_rows + second_op->output_rows;
		ret = SUCCESS;
		break;
	}
	case ExprStmt::OP_EXCEPT:
	{
		root_operator = HashExcept::make_hash_except(first_op, second_op);
		root_operator->output_rows =
			first_op->output_rows > second_op->output_rows ?
			first_op->output_rows - second_op->output_rows :
			second_op->output_rows - first_op->output_rows;
		ret = SUCCESS;
		break;
	}
	case ExprStmt::OP_INTERSECT:
	{
		root_operator = HashIntersect::make_hash_intersect(first_op, second_op);
		root_operator->output_rows = 
			first_op->output_rows > second_op->output_rows ? 
			second_op->output_rows : first_op->output_rows;
		ret = SUCCESS;
		break;
	}
	default:
		break;
	}
	
	select_list_name = first_select->select_list_name;
	for (u32 i = 0; i < select_list_name.size(); ++i) {
		ColumnDesc col_desc;
		col_desc.set_tid_cid(0, i);
		Expression_s col = ColumnExpression::make_column_expression(col_desc);
		select_list.push_back(col);
	}
	make_query_plan(root_operator);
	return ret;
}
