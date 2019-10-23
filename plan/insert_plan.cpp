#include "schema_checker.h"
#include "insert_plan.h"
#include "insert_stmt.h"
#include "table_space.h"
#include "expr_stmt.h"
#include "object.h"
#include "insert.h"
#include "stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Storage;

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
		return PLAN_NOT_BUILD;
	}
	u32 ret;
	ret = root_operator->open();
	if (ret != SUCCESS) {
		result = Error::make_object(ret);
		return ret;
	}
	for (u32 i = 0; i < values.size(); ++i) {
		ret = root_operator->get_next_row(values[i]);
		if (ret != SUCCESS) {
			result = Error::make_object(ret);
			break;
		}
	}
	
	u32 ret2 = root_operator->close();
	if (ret != SUCCESS) {
		result = Error::make_object(ret);
		return ret;
	}
	else if (ret2 != SUCCESS) {
		result = Error::make_object(ret2);
		return ret2;
	}
	else {
		affect_rows_ = values.size();
		result = Error::make_object(SUCCESS);
		return SUCCESS;
	}
}

u32 InsertPlan::build_plan()
{
	if (!lex_stmt || lex_stmt->stmt_type() != Stmt::Insert)
	{
		Log(LOG_ERR, "InsertPlan", "error lex stmt when build insert plan");
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
	TableSpace_s table_space = TableSpace::make_table_space(table_name, database);
	assert(table_space);
	root_operator = Insert::make_insert(table_space);
	//将解析的value转换为Row
	assert(lex->values->stmt_type() == Stmt::Expr);
	ExprStmt* expr = dynamic_cast<ExprStmt*>(lex->values.get());
	assert(expr->expr_stmt_type() == ExprStmt::List);
	ListStmt* list = dynamic_cast<ListStmt*>(expr);
	RowDesc row_desc = checker->get_row_desc(database, table_name);
	for (u32 i = 0; i < list->stmt_list.size(); ++i){
		u32 ret = check_column_value(list->stmt_list[i], row_desc);
		if (ret) {
			Log(LOG_ERR, "InsertPlan", "column value check failed in values:%u, column:%u", i, ret);
			return ERROR_COLUMN_VALUE;
		}
		Row_s row;
		ret = resolve_row(list->stmt_list[i], row_desc, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "InsertPlan", "create row error:%s", err_string(ret));
			return ret;
		}

		values.push_back(row);
	}
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

u32 InsertPlan::resolve_row(const Stmt_s& list, const RowDesc& row_desc, Row_s& row)
{
	row = Row::make_row(row_desc);
	ExprStmt* expr = dynamic_cast<ExprStmt*>(list.get());
	ListStmt* list_stmt = dynamic_cast<ListStmt*>(expr);
	for (u32 i = 0; i < list_stmt->stmt_list.size(); ++i) {
		ExprStmt* expr_value = dynamic_cast<ExprStmt*>(list_stmt->stmt_list[i].get());
		ConstStmt* value = dynamic_cast<ConstStmt*>(expr_value);
		row->set_cell(i, value->value);
	}
	return SUCCESS;
}