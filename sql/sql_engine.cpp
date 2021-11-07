#include "sql_engine.h"
#include "insert_plan.h"
#include "delete_plan.h"
#include "update_plan.h"
#include "select_plan.h"
#include "query_result.h"
#include "phy_operator.h"
#include "cmd_plan.h"
#include "stmt.h"
#include "sql_driver.h"
#include "query_result.h"
#include "dml_resolver.h"
#include "code_generator.h"
#include "expr_generator.h"
#include "transformer.h"
#include "plan_info.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;
using namespace CatDB::Transform;
using namespace CatDB::Sql;
using CatDB::SqlDriver;

ResultSet::ResultSet(u32 column_count)
    :result_title(column_count, ""),
    result_type(column_count, T_NULL),
    desc(column_count),
    affect_rows(0),
    have_user_data(false)
{
    query_result = QueryResult::make_query_result();
}

ResultSet::~ResultSet()
{

}

ResultSet_s ResultSet::make_result_set(u32 column_count)
{
    return ResultSet_s(new ResultSet(column_count));
}

u32 ResultSet::get_column_num() const
{
    return desc.get_column_num();
}

void ResultSet::set_column_num(u32 column_count)
{
    result_title = Vector<String>(column_count, "");
    result_type = Vector<u32>(column_count, T_NULL);
    desc = RowDesc(column_count);
    have_user_data = true;
}

u32 ResultSet::set_result_title(u32 idx, const String &title)
{
    if (idx >= result_title.size()) {
        return ERROR_INDEX;
    } else {
        result_title[idx] = title;
        return SUCCESS;
    }
}

String ResultSet::get_result_title(u32 idx) const
{
    if (idx >= result_title.size()) {
        return String("");
    } else {
        return result_title[idx];
    }
}

u32 ResultSet::set_result_type(u32 idx, u32 type)
{
    if (idx >= result_type.size()) {
        return ERROR_INDEX;
    } else {
        result_type[idx] = type;
        return SUCCESS;
    }
}

u32 ResultSet::get_result_type(u32 idx) const
{
    if (idx >= result_type.size()) {
        return T_NULL;
    } else {
        return result_type[idx];
    }
}

u32 ResultSet::add_row(Row_s &row)
{
    ++affect_rows;
    return query_result->add_row(row);
}

u32 ResultSet::add_row(Vector<Object_s> &cells)
{
    u32 ret = SUCCESS;
    MY_ASSERT(desc.get_column_num() == cells.size());
    Row_s row = Row::make_row(desc);
    for (u32 i = 0; i < cells.size(); ++i) {
        CHECK(row->set_cell(i, cells[i]));
    }
    ++affect_rows;
    CHECK(query_result->add_row(row));
    return ret;
}

u32 ResultSet::add_row(Object_s &cell)
{
    u32 ret = SUCCESS;
    MY_ASSERT(desc.get_column_num() == 1);
    Row_s row = Row::make_row(desc);
    CHECK(row->set_cell(0, cell));
    ++affect_rows;
    CHECK(query_result->add_row(row));
    return ret;
}

QueryResult_s ResultSet::get_query_result()
{
    return query_result;
}

u32 ResultSet::get_affect_rows() const
{
    return affect_rows;
}

SqlEngine::SqlEngine(const String& query, QueryCtx& query_ctx)
            :query_ctx(query_ctx),
            query(query)
{

}

SqlEngine::~SqlEngine()
{

}

SqlEngine_s SqlEngine::make_sql_engine(const String& query, QueryCtx &query_ctx)
{
    return SqlEngine_s(new SqlEngine(query, query_ctx));
}

u32 SqlEngine::handle_inner_sql(const String &query, QueryCtx &query_ctx, ResultSet_s &result_set)
{
    u32 ret = SUCCESS;
    SqlEngine engine(query, query_ctx);
    CHECK(engine.handle_query());
    result_set = engine.get_query_result();
    return ret;
}

u32 SqlEngine::handle_subplan(PhyOperator_s root, Object_s &result)
{
    u32 ret = SUCCESS;
    QueryResult_s query_result = QueryResult::make_query_result();
    CHECK(root->open());
    Row_s row;
    while ((ret = root->get_next_row(row)) == SUCCESS) {
        query_result->add_row(row);
    }
    if (ret == NO_MORE_ROWS) {
        ret = SUCCESS;
    } else {
        return ret;
    }
    CHECK(root->close());
    result = query_result;
    return ret;
}

u32 SqlEngine::handle_query()
{
    u32 ret = SUCCESS;
    SqlDriver parser;
    ResolveCtx resolve_ctx;
	parser.set_global_database(query_ctx.cur_database);
	parser.parse_sql(query);
	if (parser.is_sys_error()) {
		ret = ERR_UNEXPECTED;
        err_msg = parser.sys_error();
        return ret;
	} else if (parser.is_syntax_error()) {
        ret = ERR_UNEXPECTED;
        err_msg = parser.syntax_error();
        return ret;
	} else if (!parser.parse_result()) {
		ret = ERR_UNEXPECTED;
        return ret;
	} else {
        lex_stmt = parser.parse_result();
        if (lex_stmt->stmt_type() !=  Stmt::DoCMD) {
            CHECK(DMLResolver::resolve_stmt(lex_stmt, query_ctx, resolve_ctx));
            Transformer transformer;
            DMLStmt_s dml_stmt = lex_stmt;
            TransformCtx_s transform_ctx = TransformCtx::make_transform_ctx();
            transform_ctx->query_ctx = &query_ctx;
            CHECK(transformer.transform(dml_stmt, transform_ctx));
        }
		plan = Plan::make_plan(lex_stmt, &query_ctx);
		MY_ASSERT(plan);
		CHECK(plan->build_plan());
        query_result = ResultSet::make_result_set();
        if (lex_stmt->stmt_type() ==  Stmt::DoCMD) {
            CMDPlan_s cmd = plan;
            CHECK(cmd->execute(query_result));
        } else {
            DMLPlan_s dml_plan = plan;
            DMLStmt_s dml_stmt = lex_stmt;
            if (dml_stmt->is_explain) {
                CHECK(explain_plan(dml_plan->get_root_operator()));
            } else {
                log_root = dml_plan->get_root_operator();
                ExprGenerateCtx generate_ctx;
                CHECK(CodeGenerator::generate_phy_plan(generate_ctx, log_root, phy_root));
                CHECK(execute_plan(phy_root));
            }
        }
	}
    return ret;
}

ResultSet_s SqlEngine::get_query_result()
{
    return query_result;
}

u32 SqlEngine::execute_plan(PhyOperator_s root)
{
    u32 ret = SUCCESS;
    u32 column_count = 0;
    if (Stmt::Select == lex_stmt->stmt_type() || 
        Stmt::SetOperation == lex_stmt->stmt_type()) {
        SelectStmt_s stmt = lex_stmt;   
        column_count = stmt->select_expr_list.size(); 
        query_result->set_column_num(column_count);
        for (u32 i = 0; i < stmt->select_expr_list.size(); ++i) {
            query_result->set_result_title(i, stmt->select_expr_list[i]->alias_name);
            query_result->set_result_type(i, T_VARCHAR);
        }
    }
    CHECK(root->open());
    Row_s row;
    while ((ret = root->get_next_row(row)) == SUCCESS) {
        query_result->add_row(row);
    }
    if (ret == NO_MORE_ROWS) {
        ret = SUCCESS;
    } else {
        return ret;
    }
    CHECK(root->close());
    return ret;
}

u32 SqlEngine::explain_plan(LogicalOperator_s root)
{
    u32 ret = SUCCESS;
    Vector<PlanInfo> plan_infos;
    String explain_info;
    root->print_plan(0, plan_infos);
    query_result->set_column_num(5);
    PlanInfo::formalize_plan_info(plan_infos);
    PlanInfo::print_plan_info(plan_infos, explain_info);
    explain_info += "\noutline:\n";
    String outline;
    CHECK(print_outline(outline));
    explain_info += outline;
    query_result->set_explain_info(explain_info);
    return ret;
}

u32 SqlEngine::print_outline(String &outline)
{
    u32 ret = SUCCESS;
    outline = "/*+\n";
    DMLStmt_s dml_stmt = lex_stmt;
    CHECK(print_stmt_outline(dml_stmt, true, outline));
    outline += "*/";
    return ret;
}

u32 SqlEngine::print_stmt_outline(DMLStmt_s stmt, bool print_global_hint, String &outline)
{
    u32 ret = SUCCESS;
    outline += stmt->stmt_hint.print_outline(print_global_hint);
    Vector<SelectStmt_s> child_stmts;
    CHECK(stmt->get_child_stmts(child_stmts));
    for (u32 i = 0; i < child_stmts.size(); ++i) {
        CHECK(print_stmt_outline(child_stmts[i], false, outline));
    }
    return ret;
}
