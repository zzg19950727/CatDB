#include "logical_operator.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "query_ctx.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

void LogicalOperator::set_query_ctx(Sql::QueryCtx* query_ctx)
{
    this->query_ctx = query_ctx;
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
    MY_ASSERT(can_be);
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
    if (ExprUtils::find_item(expr_produce, expr_consume)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_CONST)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_EXEC_PARAM)) {
        can_be = true;
    } else if (expr_consume->has_flag(IS_SET_EXPR)) {
        can_be = true;
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
        if (can_be && !ExprUtils::find_item(expr_output, expr_consume[i])) {
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
    if (ExprUtils::find_item(expr_produce, expr_consume)) {
        if (!ExprUtils::find_item(expr_output, expr_consume)) {
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

void LogicalOperator::print_basic_info(u32 depth, PlanInfo& info, const String &type)
{
    info.depth = depth;
    info.rows = std::to_string(int(output_rows));
    info.cost = std::to_string(int(cost));
    if (!output_exprs.empty()) {
        info.expr_info += "output expr:";
        if (type.empty()) {
            print_exprs(output_exprs, info.expr_info);
        } else {
            print_exprs(output_exprs, type, info.expr_info);
        }
    }
    if (!filters.empty()) {
        info.expr_info += "filters:";
        print_exprs(filters, info.expr_info);
    }
}

void LogicalOperator::print_exprs(Vector<ExprStmt_s> &exprs, String &out)
{
    out += "[";
    for (u32 i = 0; i < exprs.size(); ++i) {
        if (0 == i) {
            out += exprs[i]->to_string();
        } else {
            out += "," + exprs[i]->to_string();
        }
    }
    out += "]\n ";
}

void LogicalOperator::print_exprs(Vector<ExprStmt_s> &exprs, const String& type, String &out)
{
    out += "[";
    for (u32 i = 0; i < exprs.size(); ++i) {
        if (0 == i) {
            out += type + "(" + exprs[i]->to_string() + ")";
        } else {
            out += "," + type + "(" + exprs[i]->to_string() + ")";
        }
    }
    out += "]\n ";
}

void LogicalOperator::print_expr(ExprStmt_s &expr, String &out)
{
    out += expr->to_string();
}