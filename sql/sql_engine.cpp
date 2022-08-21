#include "sql_engine.h"
#include "insert_plan.h"
#include "delete_plan.h"
#include "update_plan.h"
#include "select_plan.h"
#include "phy_operator.h"
#include "phy_expression.h"
#include "cmd_plan.h"
#include "stmt.h"
#include "sql_driver.h"
#include "dml_resolver.h"
#include "code_generator.h"
#include "expr_generator.h"
#include "transformer.h"
#include "session_info.h"
#include "plan_info.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;
using namespace CatDB::Transform;
using namespace CatDB::Sql;
using namespace CatDB::Server;
using CatDB::SqlDriver;

ResultSet::ResultSet()
    :have_user_data(false),
    is_explain(false)
{
}

ResultSet::~ResultSet()
{

}

ResultSet_s ResultSet::make_result_set()
{
    return ResultSet_s(new ResultSet());
}

u32 ResultSet::add_result_title(const String &title, OBJ_TYPE type)
{
    u32 ret = SUCCESS;
    have_user_data = true;
    result_title.push_back(title);
    result_type.push_back(type);
    return ret;
}

String ResultSet::get_result_title(u32 idx) const
{
    if (idx >= result_title.size()) {
        return String("");
    } else {
        return result_title[idx];
    }
}

OBJ_TYPE ResultSet::get_result_type(u32 idx) const
{
    if (idx >= result_type.size()) {
        return T_VARCHAR;
    } else {
        return result_type[idx];
    }
}

void ResultSet::add_row(const Row_s& row)
{
    cache_rows.push_back(row);
}

void ResultSet::set_op_root(PhyOperator_s &root)
{
    phy_root = root;
}

u32 ResultSet::open()
{
    u32 ret = SUCCESS;
    if (phy_root) {
        CHECK(phy_root->open());
    } else {
        row_idx = 0;
    }
    return ret;
}

u32 ResultSet::get_next_row(Row_s &row)
{
    u32 ret = SUCCESS;
    if (phy_root) {
        CHECK(phy_root->get_next_row(row));
    } else {
        if (row_idx >= cache_rows.size()) {
            ret = NO_MORE_ROWS;
        } else {
            row = cache_rows[row_idx++];
        }
    }
    return ret;
}

u32 ResultSet::close()
{
    u32 ret = SUCCESS;
    if (phy_root) {
        CHECK(phy_root->close());
    } else {
        row_idx = 0;
    }
    return ret;
}

SqlEngine::SqlEngine(const String& query)
            :query(query)
{

}

SqlEngine::~SqlEngine()
{

}

SqlEngine_s SqlEngine::make_sql_engine(const String& query)
{
    return SqlEngine_s(new SqlEngine(query));
}

u32 SqlEngine::handle_inner_sql(const String &query, ResultSet_s &result_set)
{
    u32 ret = SUCCESS;
    LOG_TRACE("inner sql:", K(query));
    SqlEngine engine(query);
    CHECK(engine.handle_query());
    result_set = engine.get_query_result();
    return ret;
}

u32 SqlEngine::handle_query()
{
    u32 ret = SUCCESS;
    SqlDriver parser;
	parser.set_global_database(GTX->get_cur_database());
	parser.parse_sql(query);
	if (parser.is_sys_error()) {
		ret = ERR_UNEXPECTED;
        QUERY_CTX->set_error_msg(parser.sys_error());
        return ret;
	} else if (parser.is_syntax_error()) {
        ret = ERR_UNEXPECTED;
        QUERY_CTX->set_error_msg(parser.syntax_error());
        return ret;
	} else if (!parser.parse_result()) {
		ret = ERR_UNEXPECTED;
        return ret;
	} else {
        query_result = ResultSet::make_result_set();
        lex_stmt = parser.parse_result();
        if (lex_stmt->stmt_type() != DoCMD) {
            ResolveCtx resolve_ctx;
            CHECK(DMLResolver::resolve_stmt(lex_stmt, resolve_ctx));
            QUERY_CTX->set_error_msg("");
            CHECK(lex_stmt->formalize());
            if (lex_stmt->is_select_stmt()) {
                SelectStmt_s stmt = lex_stmt;
                for (u32 i = 0; i < stmt->select_expr_list.size(); ++i) {
                    query_result->add_result_title(stmt->select_expr_list[i]->get_alias_name(), T_VARCHAR);
                }
            }
            QueryHint_s query_hint = QueryHint::make_query_hint();
            CHECK(query_hint->init(resolve_ctx.all_hints, resolve_ctx.has_outline));
            QUERY_CTX->query_hint = query_hint;
            LOG_TRACE("succeed to generate stmt", K(lex_stmt));
            LOG_TRACE("succeed to init query hint", K(query_hint));
            Transformer transformer;
            DMLStmt_s dml_stmt = lex_stmt;
            TransformCtx_s transform_ctx = TransformCtx::make_transform_ctx();
            CHECK(transformer.transform(dml_stmt, transform_ctx));
        }
		plan = Plan::make_plan(lex_stmt);
		MY_ASSERT(plan);
        //optimizer
		CHECK(plan->build_plan());
        if (lex_stmt->stmt_type() == DoCMD) {
            CMDPlan_s cmd = plan;
            CHECK(cmd->execute(query_result));
        } else {
            DMLPlan_s dml_plan = plan;
            DMLStmt_s dml_stmt = lex_stmt;
            String plan_info;
            CHECK(explain_plan(dml_plan->get_root_operator(), plan_info));
            LOG_TRACE("logical plan:", K(plan_info));
            if (dml_stmt->is_explain) {
                query_result->set_explain_info(plan_info);
            } else {
                log_root = dml_plan->get_root_operator();
                ExprGenerateCtx generate_ctx;
                generate_ctx.exec_ctx = ExecCtx::make_exec_ctx();
                CHECK(CodeGenerator::generate_phy_plan(generate_ctx, log_root, phy_root));
                CHECK(init_result_set());
            }
        }
	}
    return ret;
}

u32 SqlEngine::handle_user_view(const String &view,  ResolveCtx *ctx, SelectStmt_s &ref_query)
{
    u32 ret = SUCCESS;
    SqlDriver parser;
	parser.set_global_database(GTX->get_cur_database());
	parser.parse_sql(view);
	if (parser.is_sys_error()) {
		ret = ERR_UNEXPECTED;
        QUERY_CTX->set_error_msg(parser.sys_error());
        return ret;
	} else if (parser.is_syntax_error()) {
        ret = ERR_UNEXPECTED;
        QUERY_CTX->set_error_msg(parser.syntax_error());
        return ret;
	} else if (!parser.parse_result()) {
		ret = ERR_UNEXPECTED;
        return ret;
	} else {
        ref_query = parser.parse_result();
        CHECK(DMLResolver::resolve_stmt(ref_query, *ctx));
        QUERY_CTX->set_error_msg("");
        CHECK(ref_query->formalize());
    }
    return ret;
}

u32 SqlEngine::handle_const_expr(ExprStmt_s &expr, 
                                Object_s &obj_result, 
                                bool &bool_result)
{
    u32 ret = SUCCESS;
    bool_result = false;
    ExprGenerateCtx expr_ctx;
    Expression_s rt_expr;
    ExecCtx_s exec_ctx = ExecCtx::make_exec_ctx();
    CHECK(ExprGenerator::generate_expr(expr_ctx, expr, rt_expr));
    CHECK(rt_expr->get_result(exec_ctx));
    obj_result = exec_ctx->output_result;
    bool_result = exec_ctx->bool_result;
    return ret;
}

ResultSet_s SqlEngine::get_query_result()
{
    return query_result;
}

u32 SqlEngine::init_result_set()
{
    u32 ret = SUCCESS;
    query_result->set_op_root(phy_root);
    if (!lex_stmt->is_select_stmt()) {
        Row_s row;
        CHECK(query_result->open());
        while (SUCC(query_result->get_next_row(row))) {
        }
        int temp_ret = ret;
        if (NO_MORE_ROWS == ret) {
            temp_ret = SUCCESS;
        }
        CHECK(query_result->close());
        ret = temp_ret;
    }
    return ret;
}

u32 SqlEngine::explain_plan(LogicalOperator_s root, String &plan_info)
{
    u32 ret = SUCCESS;
    Vector<PlanInfo> plan_infos;
    String explain_info;
    root->print_plan(0, plan_infos);
    PlanInfo::formalize_plan_info(plan_infos);
    PlanInfo::print_plan_info(plan_infos, explain_info);
    explain_info += "\noutline:\n";
    String outline;
    CHECK(print_outline(outline));
    explain_info += outline;
    plan_info = explain_info;
    return ret;
}

u32 SqlEngine::print_outline(String &outline)
{
    u32 ret = SUCCESS;
    outline = "/*+\n";
    DMLStmt_s dml_stmt = lex_stmt;
    outline += QUERY_CTX->query_hint->print_outline();
    outline += "*/";
    return ret;
}