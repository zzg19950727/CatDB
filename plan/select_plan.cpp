#include "schema_checker.h"
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
	asc(true),
	have_limit(false)
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

u32 SelectPlan::execute()
{
	if (!root_operator) {
		return_result(PLAN_NOT_BUILD);
	}
	u32 ret;
	ret = root_operator->open();
	if (ret != SUCCESS) {
		return_result(ret);
	}

	affect_rows_ = 0;
	Row_s row;
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	RowDesc row_desc(select_list_name.size());
	result_title = Row::make_row(row_desc);
	for (u32 i = 0; i < select_list_name.size(); ++i) {
		Object_s label = Varchar::make_object(select_list_name[i]);
		result_title->set_cell(i, label);
	}
	
	while ((ret = root_operator->get_next_row(row)) == SUCCESS)
	{
		query_result->add_row(row);
		++affect_rows_;
	}
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
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Select)
	{
		if (lex_stmt && lex_stmt->stmt_type() == Stmt::Expr)
		{
			Log(LOG_ERR, "SelectPlan", "set operation not support yet");
		}
		Log(LOG_ERR, "SelectPlan", "error lex stmt when build select plan");
		return_result(ERROR_LEX_STMT);
	}
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	SelectStmt* lex = dynamic_cast<SelectStmt*>(lex_stmt.get());
	is_distinct = lex->is_distinct;
	asc = lex->asc_desc;
	if (lex->limit_stmt)
		have_limit = true;
	else
		have_limit = false;
	u32 ret = get_ref_tables(lex->from_stmts);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in from stmts");
		return_result( ret );
	}
	ret = resolve_select_list(lex->select_expr_list);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in select list");
		return_result(ret);
	}
	ret = resolve_where_stmt(lex->where_stmt);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in where stmts");
		return_result(ret);
	}
	ret = resolve_group_stmt(lex->group_columns);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in group stmts");
		return_result(ret);
	}
	ret = resolve_having_stmt(lex->having_stmt);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in having stmts");
		return_result(ret);
	}
	ret = resolve_sort_stmt(lex->order_columns);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in sort stmts");
		return_result(ret);
	}
	ret = resolve_limit_stmt(lex->limit_stmt);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "error stmt in limit stmts");
		return_result(ret);
	}
	ret = make_access_row_desc();
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "make access row desc for from tables failed");
		return_result(ret);
	}
	ret = choos_best_join_order();
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "choose best join order failed");
		return_result(ret);
	}
	ret = make_join_plan(root_operator);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "make join plan failed");
		return_result(ret);
	}
	ret = make_group_pan(root_operator);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "make group plan failed");
		return_result(ret);
	}
	if (!is_sort_query_result) {
		ret = make_sort_plan(root_operator);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "make sort plan failed");
			return_result(ret);
		}
		ret = make_query_plan(root_operator);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "make expression plan failed");
			return_result(ret);
		}
	}
	else {
		ret = make_query_plan(root_operator);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "make expression plan failed");
			return_result(ret);
		}
		ret = make_sort_plan(root_operator);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "make sort plan failed");
			return_result(ret);
		}
	}
	ret = make_distinct_plan(root_operator);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "make distinct plan failed");
		return_result(ret);
	}
	ret = make_limit_plan(root_operator);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "make limit plan failed");
		return_result(ret);
	}
	return_result(SUCCESS);
}

u32 SelectPlan::optimizer()
{
	//TODO:add later
	return SUCCESS;
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

PhyOperator_s SelectPlan::get_root_operator()
{
	return root_operator;
}

/*
 * 解析where子句块，切分成and连接的谓词分析
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
	//如果是and连接的谓词
	if (expr_stmt->expr_stmt_type() == ExprStmt::Binary) {
		BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		if (binary_stmt->op_type == ExprStmt::OP_AND) {
			//递归解析左语句块
			u32 ret = resolve_where_stmt(binary_stmt->first_expr_stmt);
			if (ret != SUCCESS) {
				Log(LOG_ERR, "SelectPlan", "create binary expression`s first expression failed");
				return ret;
			}
			//递归解析右语句块
			ret = resolve_where_stmt(binary_stmt->second_expr_stmt);
			if (ret != SUCCESS) {
				Log(LOG_ERR, "SelectPlan", "create binary expression`s second expression failed");
				return ret;
			}
			return SUCCESS;
		}
	}
	//分割到最小块了
	return resolve_simple_stmt(where_stmt);
}

/*
 * 解析最小块语句，分成四种情况：
 * 两表等值连接谓词、两表普通连接谓词
 * 基表过滤谓词、普通过滤谓词
 */
u32 SelectPlan::resolve_simple_stmt(const Stmt_s& stmt)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}

	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	if (expr_stmt->expr_stmt_type() == ExprStmt::Binary) {
		BinaryExprStmt* binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		ColumnStmt* first_column = resolve_column_stmt(binary_stmt->first_expr_stmt);
		ColumnStmt* second_column = resolve_column_stmt(binary_stmt->second_expr_stmt);
		//可能是join条件
		if (first_column && second_column) {
			TableStmt* first_table = nullptr;
			TableStmt* second_table = nullptr;
			u32 ret = who_have_column(first_column, first_table);
			if (ret != SUCCESS) {
				return ret;
			}
			ret = who_have_column(second_column, second_table);
			if (ret != SUCCESS) {
				return ret;
			}
			//两张表的列构成连接条件
			if (first_table->alias_name != second_table->alias_name) {
				if (binary_stmt->op_type == ExprStmt::OP_EQ) {
					Expression_s expr;
					u32 ret = resolve_expr(stmt, expr);
					if (ret != SUCCESS) {
						return ret;
					}
					add_join_equal_cond(JoinableTables(first_table, second_table), expr);
					add_join_cond(JoinableTables(first_table, second_table), expr);
					return SUCCESS;
				}
				else {
					Expression_s expr;
					u32 ret = resolve_expr(stmt, expr);
					if (ret != SUCCESS) {
						return ret;
					}
					add_join_cond(JoinableTables(first_table, second_table), expr);
					return SUCCESS;
				}
			}
		}
	}

	//检查是否是单张表的过滤谓词
	Expression_s expr;
	u32 ret = resolve_expr(stmt, expr);
	if (ret != SUCCESS) {
		return ret;
	}
	TableStmt* table = nullptr;
	if (is_table_filter(stmt, table)) {
		add_table_filter(table, expr);
	}
	else {
		//普通过滤谓词在完成所有join后使用
		make_and_expression(filter_after_join, expr);
	}
	return SUCCESS;
}

/*
 * 如果当前谓词是列则后返回列，否则返回null
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
 * 将谓词语句块转换为表达式
 */
u32 SelectPlan::resolve_expr(const Stmt_s& stmt, Expression_s& expr)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return ERROR_LEX_STMT;
	}

	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	Expression_s first_expr, second_expr, third_expr;
	ConstStmt* const_stmt = nullptr;
	ColumnStmt* column_stmt = nullptr;
	ColumnDesc col_desc;
	AggrStmt* agg_stmt = nullptr;
	UnaryExprStmt* unary_stmt = nullptr;
	BinaryExprStmt* binary_stmt = nullptr;
	TernaryExprStmt* ternary_stmt = nullptr;
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
		if (!resolve_select_list_or_having && column_stmt->is_all_column()) {
			Log(LOG_ERR, "SelectPlan", "* can only exists in select list or count(*)");
			return ERROR_LEX_STMT;
		}
		else if (column_stmt->is_all_column()) {
			return HAVE_ALL_COLUMN_STMT;
		}
		else {
			ret = resolve_column_desc(column_stmt, col_desc);
			if (ret != SUCCESS) {
				break;
			}
			expr = ColumnExpression::make_column_expression(col_desc);
			ret = SUCCESS;
		}
		break;
	case ExprStmt::Query:
		Log(LOG_ERR, "SelectPlan", "subquery in select`s where stmt not support yet");
		ret = ERROR_LEX_STMT;
		break;
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
		if (!resolve_select_list_or_having) {
			Log(LOG_ERR, "SelectPlan", "aggregate function can only exist in select list or having stmt");
			return ERROR_LEX_STMT;
		}
		
		agg_stmt = dynamic_cast<AggrStmt*>(expr_stmt);
		ret = resolve_expr(agg_stmt->aggr_expr, expr);
		if (ret == HAVE_ALL_COLUMN_STMT) {
			if (agg_stmt->aggr_func != AggrStmt::COUNT) {
				Log(LOG_ERR, "SelectPlan", "only count aggregation can have * expression");
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
		expr = AggregateExpression::make_aggregate_expression(expr, agg_stmt->aggr_func);
		aggr_exprs.push_back(expr);
		//为了统一表达式计算框架把聚合表达式替换成group算子输出列描述，因为聚合函数本身由group算子计算
		col_desc.set_tid_cid(alias_table_id, aggr_exprs.size() - 1);
		expr = ColumnExpression::make_column_expression(col_desc);
		ret = SUCCESS;
		break;
	case ExprStmt::Unary:
		unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		ret = resolve_expr(unary_stmt->expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "create unary expression`s first expression failed");
			break;
		}
		expr = UnaryExpression::make_unary_expression(first_expr, unary_stmt->op_type);
		ret = SUCCESS;
		break;
	case ExprStmt::Binary:
		binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);

		ret = resolve_expr(binary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "create binary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(binary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "create binary expression`s second expression failed");
			break;
		}
		expr = BinaryExpression::make_binary_expression(first_expr, second_expr, binary_stmt->op_type);
		ret = SUCCESS;
		break;
	case ExprStmt::Ternary:
		ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
		ret = resolve_expr(ternary_stmt->first_expr_stmt, first_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "create ternary expression`s first expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->second_expr_stmt, second_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "create ternary expression`s second expression failed");
			break;
		}
		ret = resolve_expr(ternary_stmt->third_expr_stmt, third_expr);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "create ternary expression`s third expression failed");
			break;
		}
		expr = TernaryExpression::make_ternary_expression(first_expr, second_expr, third_expr, ternary_stmt->op_type);
		ret = SUCCESS;
		break;
	default:
		Log(LOG_ERR, "SelectPlan", "unknown expr stmt in select`s where stmt");
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
				SelectPlan* plan = tmp_table_list[table->table_name];
				if (!plan || plan->get_query_row_desc(row_desc) != SUCCESS) {
					return COLUMN_NOT_EXISTS;
				}
				for (u32 i = 0; i < row_desc.get_column_num(); ++i) {
					ColumnDesc col_desc;
					row_desc.get_column_desc(i, col_desc);
					Expression_s expr = ColumnExpression::make_column_expression(col_desc);
					select_list.push_back(expr);
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
			SelectPlan* plan = tmp_table_list[table->table_name];
			if (!plan || plan->get_query_row_desc(row_desc) != SUCCESS) {
				return COLUMN_NOT_EXISTS;
			}
			for (u32 i = 0; i < row_desc.get_column_num(); ++i) {
				ColumnDesc col_desc;
				row_desc.get_column_desc(i, col_desc);
				Expression_s expr = ColumnExpression::make_column_expression(col_desc);
				select_list.push_back(expr);
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
	ColumnStmt* column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	if (column_stmt->table == "*") {
		assert(table_list.size());
		TableStmt* table = table_list[0];
		if (!table->is_tmp_table) {
			RowDesc row_desc;
			u32 ret = checker->get_row_desc(table->database, table->table_name, row_desc);
			if (ret != SUCCESS) {
				return ret;
			}
			assert(row_desc.get_column_num());
			ColumnDesc col_desc;
			row_desc.get_column_desc(0, col_desc);
			u32 tid, cid;
			col_desc.get_tid_cid(tid, cid);
			tid = checker->get_table_id(table->database, table->alias_name);
			col_desc.set_tid_cid(tid, cid);
			expr = ColumnExpression::make_column_expression(col_desc);
			add_access_column(table, col_desc);
		}
	}
	else {
		TableStmt* table = nullptr;
		u32 ret = find_table(column_stmt->table, table);
		if (ret != SUCCESS) {
			return ret;
		}
		if (!table->is_tmp_table) {
			RowDesc row_desc;
			ret = checker->get_row_desc(table->database, table->table_name, row_desc);
			if (ret != SUCCESS) {
				return ret;
			}
			assert(row_desc.get_column_num());
			ColumnDesc col_desc;
			row_desc.get_column_desc(0, col_desc);
			u32 tid, cid;
			col_desc.get_tid_cid(tid, cid);
			tid = checker->get_table_id(table->database, table->alias_name);
			col_desc.set_tid_cid(tid, cid);
			expr = ColumnExpression::make_column_expression(col_desc);
			add_access_column(table, col_desc);
		}
	}
	return SUCCESS;
}
/*
 * 当前谓词语句块是否是基表过滤谓词：
 * 如果谓词中只出现了一张表的列或者都是常量，
 * 则表示是基表过滤谓词，并返回基表
 */
bool SelectPlan::is_table_filter(const Stmt_s & stmt, TableStmt *& table)
{
	if (stmt->stmt_type() != Stmt::Expr) {
		return false;
	}

	ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
	ColumnStmt* column_stmt;
	UnaryExprStmt* unary_stmt;
	BinaryExprStmt* binary_stmt;
	TernaryExprStmt* ternary_stmt;
	bool ret = true;

	switch (expr_stmt->expr_stmt_type())
	{
	case ExprStmt::Const:
		ret = true;
		break;
	case ExprStmt::Column:
		column_stmt = dynamic_cast<ColumnStmt*>(expr_stmt);
		//在此之前没有出现过列相关谓词
		if (table == nullptr) {
			u32 rt = who_have_column(column_stmt, table);
			if (rt != SUCCESS) {
				ret = false;
			}
			else {
				ret = true;
			}
		}
		//在此之前出现过列相关谓词
		else {
			TableStmt* tb = nullptr;
			u32 rt = who_have_column(column_stmt, tb);
			//如果列相关谓词不属于同一张表，表示不是基表过滤谓词
			if (rt != SUCCESS || table != tb) {
				ret = false;
			}
			else {
				ret = true;
			}
		}
		break;
	case ExprStmt::List:
		ret = true;
		break;
	case ExprStmt::Unary:
		unary_stmt = dynamic_cast<UnaryExprStmt*>(expr_stmt);
		ret = is_table_filter(unary_stmt->expr_stmt, table);
		break;
	case ExprStmt::Binary:
		binary_stmt = dynamic_cast<BinaryExprStmt*>(expr_stmt);
		ret = is_table_filter(binary_stmt->first_expr_stmt, table);
		if (!ret) {
			break;
		}
		else {
			ret = is_table_filter(binary_stmt->second_expr_stmt, table);
		}
		break;
	case ExprStmt::Ternary:
		ternary_stmt = dynamic_cast<TernaryExprStmt*>(expr_stmt);
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
	default:
		ret = false;
	}
	return ret;
}

/*
 * 获取列描述
 */
u32 SelectPlan::resolve_column_desc(ColumnStmt * column_stmt, ColumnDesc & col_desc)
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	TableStmt* table = nullptr;
	u32 ret = who_have_column(column_stmt, table);
	if (ret != SUCCESS) {
		return ret;
	}
	if (table->is_tmp_table) {
		SelectPlan* plan = tmp_table_list[table->table_name];
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
 * 从from list中搜索指定表，重复出现表示from list出错
 */
u32 SelectPlan::find_table(const String & table_name, TableStmt*& table)
{
	u32 find = 0;
	for (u32 i = 0; i < table_list.size(); ++i) {
		//如果引用的表没有使用别名，在解析的时候会使用真实表明表示别名
		//所以这里只需要搜索表的别名
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

/*
 * 从from list中搜索包含指定列的表
 */
u32 SelectPlan::who_have_column(ColumnStmt * column_stmt, TableStmt *& table)
{
	//SQL中没有指定列所属的表
	if (column_stmt->table.empty()) {
		u32 ret = who_have_column(column_stmt->column, table);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "parse column define error in where stmt:%s", err_string(ret));
			return ret;
		}
	}
	//SQL中指定了列所属的表
	else {
		u32 ret = find_table(column_stmt->table, table);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SelectPlan", "parse column define error in where stmt:%s", err_string(ret));
			return ret;
		}
	}
	return SUCCESS;
}
/*
 * 从from list中搜索包含指定列的表
 */
u32 SelectPlan::who_have_column(const String & column_name, TableStmt*& table)
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	assert(checker);
	u32 find = 0;
	for (u32 i = 0; i < table_list.size(); ++i) {
		if (table_list[i]->is_tmp_table) {
			SelectPlan* plan = tmp_table_list[table_list[i]->table_name];
			ColumnDesc col_desc;
			if (plan && plan->get_column_from_select_list(column_name, col_desc) == SUCCESS) {
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
/*
 * 从from 语句块中解析出所有引用了的表
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
				Log(LOG_ERR, "SelectPlan", "same alias table %s in from list", table->alias_name.c_str());
				return NOT_UNIQUE_TABLE;
			}
			else {
				table_list.push_back(table);
			}
		}
	}
	return SUCCESS;
}

u32 SelectPlan::get_ref_table_from_query(QueryStmt * subquery)
{
	Plan_s plan = Plan::make_plan(subquery->query_stmt);
	if (!plan || plan->type() != Plan::SELECT) {
		return ERROR_LEX_STMT;
	}
	tmp_plans.push_back(plan);
	u32 ret = plan->build_plan();
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SelectPlan", "resolve subquery in from list failed");
		return ret;
	}
	TableStmt* table = nullptr;
	if (find_table(subquery->alias_name, table) == SUCCESS) {
		return TABLE_EXISTS;
	}
	tmp_table_list[subquery->alias_name] = dynamic_cast<SelectPlan*>(plan.get());
	Stmt_s stmt = TableStmt::make_table_stmt(subquery->alias_name);
	tmp_table_handle.push_back(stmt);
	table = dynamic_cast<TableStmt*>(stmt.get());
	table->is_tmp_table = true;
	table_list.push_back(table);
	return SUCCESS;
}
/*
 * 搜索指定两张表的连接谓词
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
 * 添加指定两张表的连接谓词
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
 * 添加指定两张表的等值连接谓词
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
 * 添加基表过滤谓词
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
 * 功能：expr = expr and other
 */
u32 SelectPlan::make_and_expression(Expression_s & expr, const Expression_s & other)
{
	if (!expr) {
		expr = other;
		return SUCCESS;
	}
	Expression_s and_expr = BinaryExpression::make_binary_expression(expr, other, ExprStmt::OP_AND);
	expr = and_expr;
	return SUCCESS;
}

u32 SelectPlan::choos_best_join_order()
{
	//TODO
	return SUCCESS;
}

u32 SelectPlan::resolve_group_stmt(const Stmt_s & group_stmt)
{
	if (!group_stmt) {
		return SUCCESS;
	}
	else if (group_stmt->stmt_type() != Stmt::Expr) {
		Log(LOG_ERR, "SelectPlan", "error stmt for sort columns");
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
		u32 ret = resolve_expr(list_stmt->stmt_list[i], column);
		if (ret != SUCCESS) {
			return ret;
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
	u32 ret = resolve_expr(having_stmt, having_filter);
	resolve_select_list_or_having = 0;
	return ret;
}

u32 SelectPlan::resolve_limit_stmt(const Stmt_s& limit_stmt)
{
	if (!limit_stmt) {
		return SUCCESS;
	}
	else if (limit_stmt->stmt_type() != Stmt::Limit) {
		Log(LOG_ERR, "SelectPlan", "error stmt for limit");
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
		Log(LOG_ERR, "SelectPlan", "error stmt for sort columns");
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
		//在select list中查找排序列
		ret = resolve_column_from_select_list(list_stmt->stmt_list[i], column);
		if (ret == SUCCESS) {
			is_sort_query_result = true;
		}
		else {
			//如果在select list中没有找到排序列，再从基表中查找
			ret = resolve_expr(list_stmt->stmt_list[i], column);
			if (ret != SUCCESS) {
				return ret;
			}
			else if (is_sort_query_result) {
				Log(LOG_ERR, "SelectPlan", "暂时不支持排序同时出现在基表和select list");
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
		Log(LOG_ERR, "SelectPlan", "select list can not be empty");
		return ERROR_LEX_STMT;
	}
	else if (select_list->stmt_type() != Stmt::Expr) {
		Log(LOG_ERR, "SelectPlan", "error stmt for select columns");
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
		ret = resolve_expr(list_stmt->stmt_list[i], expr);
		if (ret == HAVE_ALL_COLUMN_STMT) {
			ret = resolve_all_column_in_select_list(list_stmt->stmt_list[i]);
			if (ret != SUCCESS) {
				break;
			}
		}
		else if (ret != SUCCESS) {
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
	if (table_access_column.find(table) == table_access_column.cend()) {
		Vector<ColumnDesc> access_columns;
		access_columns.push_back(col_desc);
		table_access_column[table] = access_columns;
	}
	else {
		bool find = false;
		Vector<ColumnDesc>& access_columns = table_access_column[table];
		for (u32 i = 0; i <access_columns.size(); ++i) {
			if (access_columns[i] == col_desc) {
				find = true;
				break;
			}
		}
		if (!find) {
			access_columns.push_back(col_desc);
		}
	}
	return SUCCESS;
}

/*
 * 为每一张需要访问的表建立访问行描述
 * 即需要访问哪些列
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
	if (table_list.size() == 0) {
		Log(LOG_ERR, "SelectPlan", "from list must have more than one table");
		return NO_TABLE_FOR_SELECT;
	}
	else if (table_list.size() == 1) {
		return make_table_scan(table_list[0], op);
	}
	else {
		PhyOperator_s left_root_operator;
		u32 ret = make_table_scan(table_list[0], left_root_operator);
		if (ret != SUCCESS) {
			return ret;
		}
		//为table list中每一张表生成join计划
		for (u32 i = 1; i < table_list.size(); ++i) {
			TableStmt* right_table = table_list[i];
			Expression_s join_cond, join_equal_cond;
			//搜索当前表与已生成计划的表是否有连接条件
			for (u32 j = 0; j < i; ++j) {
				TableStmt* left_table = table_list[j];
				JoinableTables joinable_table(left_table, right_table);
				JoinConditions condition;
				u32 ret = search_jon_info(joinable_table, condition);
				//两张表没有join条件
				if (ret != SUCCESS) {
					continue;
				}
				//合并连接条件
				make_and_expression(join_cond, condition.first);
				make_and_expression(join_equal_cond, condition.second);
			}
			PhyOperator_s right_op;
			u32 ret = make_table_scan(right_table, right_op);
			if (ret != SUCCESS) {
				Log(LOG_ERR, "SelectPlan","make table scan for table failed");
				return ret;
			}
			//没有等值连接条件的连接只能选择nested loop算法
			if (!join_equal_cond) {
				left_root_operator = NestedLoopJoin::make_nested_loop_join(left_root_operator, right_op, join_cond);
			}
			else {
				SchemaChecker_s checker = SchemaChecker::make_schema_checker();
				assert(checker);
				u32 id = checker->get_table_id(right_table->database, right_table->alias_name);
				left_root_operator = HashJoin::make_hash_join(left_root_operator, 
					right_op, join_equal_cond, join_cond, id);
			}
		}
		if (filter_after_join) {
			Filter_s filter = Filter::make_filter(filter_after_join);
			left_root_operator = PlanFilter::make_plan_filter(left_root_operator, filter);
		}
		op = left_root_operator;
		return SUCCESS;
	}
}

u32 SelectPlan::make_table_scan(TableStmt * table, PhyOperator_s & op)
{
	Filter_s filter;
	if (table_filters.find(table) != table_filters.cend()) {
		filter = Filter::make_filter(table_filters[table]);
	}
	if (table->is_tmp_table) {
		SelectPlan* plan = tmp_table_list[table->table_name];
		if (!plan) {
			return ERROR_LEX_STMT;
		}
		if (!filter) {
			op = plan->get_root_operator();
		}
		else {
			op = PlanFilter::make_plan_filter(plan->get_root_operator(), filter);
		}
	}
	else {
		op = TableScan::make_table_scan(table->database, table->table_name, table_access_row_desc[table], filter);
	}
	return SUCCESS;
}

u32 SelectPlan::make_group_pan(PhyOperator_s & op)
{
	//scalar group by
	if (group_cols.size() == 0) {
		//没有任何需要计算的聚合函数
		if (aggr_exprs.size() == 0) {
			//不需要聚合算子
			if (having_filter) {
				Log(LOG_ERR, "SelectPlan", "can not have having filter when there is no group operation");
				return HAVING_ERROR;
			}
			else {
				return SUCCESS;
			}
		}
		else {
			Filter_s filter = Filter::make_filter(having_filter);
			op = ScalarGroup::make_scalar_group(op, filter);
			ScalarGroup* scalar_group = dynamic_cast<ScalarGroup*>(op.get());
			for (u32 i = 0; i < aggr_exprs.size(); ++i) {
				scalar_group->add_agg_expr(aggr_exprs[i]);
			}
			scalar_group->set_alias_table_id(alias_table_id);
			return SUCCESS;
		}
	}
	else {
		Filter_s filter = Filter::make_filter(having_filter);
		op = HashGroup::make_hash_group(op, group_cols, filter);
		HashGroup* hash_group = dynamic_cast<HashGroup*>(op.get());
		for (u32 i = 0; i < aggr_exprs.size(); ++i) {
			hash_group->add_agg_expr(aggr_exprs[i]);
		}
		hash_group->set_agg_table_id(alias_table_id);
		return SUCCESS;
	}
}

u32 SelectPlan::make_sort_plan(PhyOperator_s & op)
{
	if (sort_cols.size() == 0) {
		return SUCCESS;
	}
	else {
		//limit 30000以内行可以用top-N排序,如果有distinct则不能改写
		if (!is_distinct && have_limit && limit_size + limit_offset < 30000) {
			op = TopNSort::make_topn_sort(op, sort_cols, limit_size + limit_offset, asc);
			//limit没有偏移则不需要再执行一次limit操作
			if (limit_offset == 0) {
				have_limit = false;
			}
		}
		else {
			op = Sort::make_sort(op, sort_cols, asc);
		}
		return SUCCESS;
	}
}

u32 SelectPlan::make_query_plan(PhyOperator_s & op)
{
	op = Query::make_query(op, select_list);
	Query* query = dynamic_cast<Query*>(op.get());
	query->set_alias_table_id(alias_table_id);
	return SUCCESS;
}

u32 SelectPlan::make_distinct_plan(PhyOperator_s & op)
{
	if (is_distinct) {
		op = HashDistinct::make_hash_distinct(op);
	}
	return SUCCESS;
}

u32 SelectPlan::make_limit_plan(PhyOperator_s & op)
{
	if (have_limit) {
		op = Limit::make_limit(op, limit_size, limit_offset);
	}
	return SUCCESS;
}
