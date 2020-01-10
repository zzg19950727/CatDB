#include "insert_plan.h"
#include "delete_plan.h"
#include "update_plan.h"
#include "select_plan.h"
#include "query_result.h"
#include "phy_operator.h"
#include "show_plan.h"
#include "create_plan.h"
#include "drop_plan.h"
#include "plan.h"
#include "stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Sql;

Plan::Plan()
	:affect_rows_(0),
	thd(nullptr),
	is_explain(false)
{
}

Plan::~Plan()
{
}

Plan_s Plan::make_plan(const Stmt_s& lex_stmt)
{
	if (!lex_stmt) {
		return Plan_s();
	}
	switch (lex_stmt->stmt_type())
	{
	case Stmt::Insert:
		return InsertPlan::make_insert_plan(lex_stmt);
	case Stmt::Delete:
		return DeletePlan::make_delete_plan(lex_stmt);
	case Stmt::Update:
		return UpdatePlan::make_update_plan(lex_stmt);
	case Stmt::Expr:
	case Stmt::Select:
		return SelectPlan::make_select_plan(lex_stmt);
	case Stmt::CreateTable:
		return CreateTablePlan::make_create_table_plan(lex_stmt);
	case Stmt::CreateDatabase:
		return CreateDatabasePlan::make_create_database_plan(lex_stmt);
	case Stmt::DropTable:
		return DropTablePlan::make_drop_table_plan(lex_stmt);
	case Stmt::DropDatabase:
		return DropDatabasePlan::make_drop_database_plan(lex_stmt);
	case Stmt::ShowTables:
		return ShowTablesPlan::make_show_tables_plan(lex_stmt);
	case Stmt::ShowDatabases:
		return ShowDatabasesPlan::make_show_databases_plan(lex_stmt);
	case Stmt::DescTable:
		return DescTablePlan::make_desc_table_plan(lex_stmt);
	case Stmt::UseDatabase:
		return UseDatabasePlan::make_use_database_plan(lex_stmt);
	case Stmt::Analyze:
		return AnalyzePlan::make_analyze_plan(lex_stmt);
	default:
		return Plan_s();
	}
}

void Plan::set_lex_stmt(const Stmt_s & stmt)
{
	lex_stmt = stmt;
}

Stmt_s Plan::get_lex_stmt() const
{
	return lex_stmt;
}

Object_s Plan::get_result()
{
	return result;
}

Row_s Plan::get_result_title() const
{
	return result_title;
}

u32 CatDB::Sql::Plan::affect_rows() const
{
	return affect_rows_;
}

void Plan::set_error_code(u32 code)
{
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query = dynamic_cast<QueryResult*>(result.get());
	query->set_error_code(code);
}

PhyOperator_s Plan::get_root_operator()
{
	return root_operator;
}

void Plan::set_thd(RequestHandle_s & thd)
{
	this->thd = thd;
}

u32 Plan::explain_plan()
{
	//创建查询结果对象
	if (!result) {
		result = QueryResult::make_query_result();
	}
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	//设置输出title
	query_result->init_title_for_explain(result_title);
	root_operator->explain_operator(0, query_result);
	return SUCCESS;
}
