#include "logical_operator.h"
#include "opt_est_info.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "query_ctx.h"
#include "query_ctx.h"
#include "error.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

u32 LogicalOperator::init(QueryCtx_s &query_ctx, EstInfo_s& est_info)
{
    this->query_ctx = query_ctx;
    this->est_info = est_info;
}

String LogicalOperator::get_op_name()
{
    switch(type()) {
        case LOG_TABLE_SCAN:
			return String(N(LOG_TABLE_SCAN));
        case LOG_JOIN:
			return String(N(LOG_JOIN));
        case LOG_SORT:
			return String(N(LOG_SORT));
        case LOG_DISTINCT:
			return String(N(LOG_DISTINCT));
        case LOG_GROUP_BY:
			return String(N(LOG_GROUP_BY));
        case LOG_SCALAR_GROUP:
			return String(N(LOG_SCALAR_GROUP));
        case LOG_SET:
			return String(N(LOG_SET));
        case LOG_LIMIT:
			return String(N(LOG_LIMIT));
        case LOG_VIEW:
			return String(N(LOG_VIEW));
        case LOG_INSERT:
			return String(N(LOG_INSERT));
        case LOG_UPDATE:
			return String(N(LOG_UPDATE));
        case LOG_DELETE:
			return String(N(LOG_DELETE));
        case LOG_EXPR_VALUE:
			return String(N(LOG_EXPR_VALUE));
        case LOG_DUAL_TABLE:
			return String(N(LOG_DUAL_TABLE));
        case LOG_SUBQUERY_EVALUATE:
			return String(N(LOG_SUBQUERY_EVALUATE));
    }
    return "";
}

u32 LogicalOperator::compute_property()
{
    u32 ret = SUCCESS;
    CHECK(est_row_count());
    CHECK(est_cost());
    return ret;
}

u32 LogicalOperator::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = 0.0;
    return ret;
}

u32 LogicalOperator::est_cost()
{
    u32 ret = SUCCESS;
    cost = 0.0;
    return ret;
}

u32 LogicalOperator::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    append(expr_ctx.expr_consume, filters);
    append(expr_ctx.expr_consume, output_exprs);
    for (u32 i = 0; i < childs.size(); ++i) {
        CHECK(childs[i]->allocate_expr_pre());
    }
    return ret;
}

u32 LogicalOperator::allocate_expr_post(Vector<ExprStmt_s> &expr_input, 
										Vector<ExprStmt_s> &expr_output)
{
    u32 ret = SUCCESS;
    bool can_be = false;
    Vector<ExprStmt_s> expr_child_output;
    Vector<ExprStmt_s> dummy_input;
    Vector<ExprStmt_s> expr_child_input = expr_ctx.expr_consume;
    append(expr_child_input, expr_input);
    for (u32 i = 0; i < childs.size(); ++i) {
        if (LOG_VIEW == type() || LOG_SET == type() || 
            (LOG_SUBQUERY_EVALUATE == type() && i > 0)) {
            CHECK(childs[i]->allocate_expr_post(dummy_input, expr_child_output));
        } else {
            CHECK(childs[i]->allocate_expr_post(expr_child_input, expr_child_output));
        }
    }
    append(access_exprs, expr_child_output);
    append(expr_child_output, expr_ctx.expr_produce);
    CHECK(expr_can_be_consumed(expr_ctx.expr_consume,
                               expr_child_output,
                               can_be));
    if (!can_be) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("expr can not be consumed", K(expr_ctx.expr_consume), K(expr_child_output), K(get_op_name()));
        return ret;
    }
    CHECK(produce_more_expr(expr_input,
                            expr_child_output, 
                            dummy_input));
    append(output_exprs, dummy_input);
    append(expr_output, dummy_input);
    return ret;
}

u32 LogicalOperator::expr_can_be_consumed(Vector<ExprStmt_s>& expr_consume, 
                                          Vector<ExprStmt_s>& expr_produce, 
                                          bool &can_be)
{
    u32 ret = SUCCESS;
    can_be = true;
    for (u32 i = 0; i < expr_consume.size() && can_be; ++i) {
        CHECK(expr_can_be_consumed(expr_consume[i], 
                                   expr_produce, 
                                   can_be));
    }
    return ret;
}

u32 LogicalOperator::expr_can_be_consumed(ExprStmt_s& expr_consume, 
                                          Vector<ExprStmt_s>& expr_produce, 
                                          bool &can_be)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr_consume);
    can_be = false;
    if (ExprUtils::find_equal_expr(expr_produce, expr_consume)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_CONST)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_EXEC_PARAM)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_SET_EXPR)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_AGG) && 
			   type() != LOG_SCALAR_GROUP && 
			   type() != LOG_GROUP_BY) {
		can_be = false;
	} else if (expr_consume->get_params().size() > 0) {
        can_be = true;
        for (u32 i = 0; i < expr_consume->get_params().size() && can_be; ++i) {
            CHECK(expr_can_be_consumed(expr_consume->get_params()[i], 
                                       expr_produce, 
                                       can_be));
        }
    } else if (expr_consume->has_flag(IS_SUBQUERY)) {
        //onetime subquery
        can_be = true;
    }
    return ret;
}

u32 LogicalOperator::produce_more_expr(Vector<ExprStmt_s>& expr_consume, 
                                       Vector<ExprStmt_s>& expr_produce, 
                                       Vector<ExprStmt_s>& expr_output)
{
    u32 ret = SUCCESS;
    bool can_be = true;
    for (u32 i = 0; i < expr_consume.size(); ++i) {
        CHECK(expr_can_be_consumed(expr_consume[i], 
                                   expr_produce, 
                                   can_be));
        if (can_be && !ExprUtils::find_equal_expr(expr_output, expr_consume[i])) {
            if (!expr_consume[i]->has_flag(IS_CONST)) {
                expr_output.push_back(expr_consume[i]);
            }
        } else if (!can_be) {
            CHECK(find_expr_will_use(expr_consume[i], 
                                     expr_produce, 
                                     expr_output));
        }
    }
    return ret;
}

u32 LogicalOperator::find_expr_will_use(ExprStmt_s& expr_consume, 
								        Vector<ExprStmt_s>& expr_produce, 
								        Vector<ExprStmt_s>& expr_output)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr_consume);
    if (ExprUtils::find_equal_expr(expr_produce, expr_consume)) {
        if (!ExprUtils::find_equal_expr(expr_output, expr_consume)) {
            expr_output.push_back(expr_consume);
        }
    } else if (expr_consume->get_params().size() > 0) {
        for (u32 i = 0; i < expr_consume->get_params().size(); ++i) {
            CHECK(find_expr_will_use(expr_consume->get_params()[i], 
                                    expr_produce, 
                                    expr_output));
        }
    }
    return ret;
}

u32 LogicalOperator::generate_operator_id_pre(u32 &id)
{
    u32 ret = SUCCESS;
    operator_id = id++;
    for (u32 i = 0; i < childs.size(); ++i) {
        CHECK(childs[i]->generate_operator_id_pre(id));
    }
    return ret;
}

u32 LogicalOperator::visit_plan()
{
    u32 ret = SUCCESS;
    CHECK(allocate_expr_pre());
    u32 start_id = 1;
    CHECK(generate_operator_id_pre(start_id));
    Vector<ExprStmt_s> dummy_input, dummy_output;
    CHECK(allocate_expr_post(dummy_input, dummy_output));
    return ret;
}

String double_to_string(double v)
{
    String ret;
    if (v < 1000000) {
        ret = std::to_string(int(v));
    } else {
        char tmp[100];
        sprintf(tmp, "%.3e", v);
        ret = String(tmp);
    }
    return ret;
}

void LogicalOperator::print_basic_info(u32 depth, PlanInfo& info, const String &type)
{
    info.depth = depth;
    info.rows = double_to_string(output_rows);
    info.cost = double_to_string(cost);
    if (!output_exprs.empty()) {
        if (type.empty()) {
            print_exprs(output_exprs, "output", info);
        } else {
            print_exprs(output_exprs, type, "output", info);
        }
    }
    if (!filters.empty()) {
        print_exprs(filters, "filters", info);
    }
}

void LogicalOperator::print_exprs(Vector<ExprStmt_s> &exprs, 
                                  const String &title, 
                                  PlanInfo& info)
{
    ExprInfo expr_info;
    expr_info.title = title;
    for (u32 i = 0; i < exprs.size(); ++i) {
        expr_info.exprs.push_back(exprs[i]->to_string());
    }
    info.expr_infos.push_back(expr_info);
}

void LogicalOperator::print_exprs(Vector<ExprStmt_s> &exprs, 
                                  const String& type, 
                                  const String &title, 
                                  PlanInfo& info)
{
    ExprInfo expr_info;
    expr_info.title = title;
    for (u32 i = 0; i < exprs.size(); ++i) {
        expr_info.exprs.push_back(type + "(" + exprs[i]->to_string() + ")");
    }
    info.expr_infos.push_back(expr_info);
}

void LogicalOperator::print_expr(ExprStmt_s &expr, String &out)
{
    out += expr->to_string();
}
