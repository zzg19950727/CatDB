#include "expression_calc.h"
#include "phy_expression.h"
#include "phy_operator.h"
#include "obj_number.h"
#include "obj_varchar.h"
#include "obj_datetime.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

u32 do_nothing(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    return ERR_UNEXPECTED;
}

u32 do_minus(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_add(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_sub(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_mul(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_div(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_equal(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_equal(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_GE(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_greater(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_LE(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_less(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_equal_any(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_equal_any(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_GE_any(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_greater_any(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_LE_any(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_less_any(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_equal_all(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_equal_all(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_GE_all(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_greater_all(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_LE_all(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_less_all(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_between(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_between(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_is_null(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_is_not_null(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_in(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_in(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_in_list(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_in_list(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_exists(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_exists(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_and(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_or(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not(const Vector<Expression_s> &params, ExecCtx_s &ctx);

u32 do_like(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_not_like(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_ifnull(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_case_when(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_cast(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_to_char(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_substr(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_to_number(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_date_add(const Vector<Expression_s> &params, ExecCtx_s &ctx);
u32 do_date_sub(const Vector<Expression_s> &params, ExecCtx_s &ctx);

OpFuncType op_funcs[OP_MAX] = {
    do_nothing,
    do_minus,
    do_add,
    do_sub,
    do_mul,
    do_div,
    
    do_equal,
    do_not_equal,
    do_GE,
    do_greater,
    do_LE,
    do_less,
    
    do_equal_any,
    do_not_equal_any,
    do_GE_any,
    do_greater_any,
    do_LE_any,
    do_less_any,
    
    do_equal_all,
    do_not_equal_all,
    do_GE_all,
    do_greater_all,
    do_LE_all,
    do_less_all,
    
    do_between,
    do_not_between,
    do_is_null,
    do_is_not_null,
    
    do_in,
    do_not_in,
    do_in_list,
    do_not_in_list,
    do_exists,
    do_not_exists,
    
    do_and,
    do_or,
    do_not,
    
    do_like,
    do_not_like,
    do_ifnull,
    do_case_when,
    do_cast,
    do_to_char,
    do_substr,
    do_to_number,
    do_nothing,
    
    do_date_add,
    do_date_sub};


u32 do_minus(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s num = ctx->output_result;
    Number_s res;
    CHECK(Number::num_minus(num, res));
    ctx->output_result = res;
    return ret;
}

u32 do_add(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s rhs = ctx->output_result;
    Number_s res;
    CHECK(Number::num_add(lhs, rhs, res));
    ctx->output_result = res;
    return ret;
}

u32 do_sub(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s rhs = ctx->output_result;
    Number_s res;
    CHECK(Number::num_sub(lhs, rhs, res));
    ctx->output_result = res;
    return ret;
}

u32 do_mul(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s rhs = ctx->output_result;
    Number_s res;
    CHECK(Number::num_mul(lhs, rhs, res));
    ctx->output_result = res;
    return ret;
}

u32 do_div(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    if (ctx->output_result->is_null()) {
        ctx->output_result = Number::make_object(0.0);
		ctx->output_result->set_null();
        return ret;
    }
    Number_s rhs = ctx->output_result;
    Number_s res;
    CHECK(Number::num_div(lhs, rhs, res));
    ctx->output_result = res;
    return ret;
}

u32 do_equal(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    ctx->bool_result = 0 == res;
    return ret;
}

u32 do_not_equal(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    ctx->bool_result = 1 == res || -1 == res;
    return ret;
}

u32 do_greater(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    ctx->bool_result = 1 == res;
    return ret;
}

u32 do_GE(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    ctx->bool_result = 1 == res || 0 == res;
    return ret;
}

u32 do_less(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    ctx->bool_result = -1 == res;
    return ret;
}

u32 do_LE(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    ctx->bool_result = 0 == res || -1 == res;
    return ret;
}

u32 do_equal_any(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 == res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_not_equal_any(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 != res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_greater_any(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (1 == res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_GE_any(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 <= res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_less_any(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (-1 == res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_LE_any(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 == res || -1 == res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_equal_all(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 != res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_not_equal_all(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 == res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_greater_all(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (1 != res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_GE_all(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 > res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_less_all(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (-1 != res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_LE_all(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 != res && -1 != res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_between(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    if (res < 0) {
        ctx->bool_result = false;
        return ret;
    }
    CHECK(params[2]->get_result(ctx));
    rhs = ctx->output_result;
    CHECK(lhs->compare(rhs, res));
    if (0 != res && -1 != res) {
        ctx->bool_result = false;
        return ret;
    }
    ctx->bool_result = true;
    return ret;
}

u32 do_not_between(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Object_s rhs = ctx->output_result;
    int res;
    CHECK(lhs->compare(rhs, res));
    if (res >= 0 || -2 == res) {
        ctx->bool_result = false;
        return ret;
    }
    CHECK(params[2]->get_result(ctx));
    rhs = ctx->output_result;
    CHECK(lhs->compare(rhs, res));
    if (0 >= res) {
        ctx->bool_result = false;
        return ret;
    }
    ctx->bool_result = true;
    return ret;
}

u32 do_is_null(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    ctx->bool_result = ctx->output_result->is_null();
    return ret;
}

u32 do_is_not_null(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    ctx->bool_result = !ctx->output_result->is_null();
    return ret;
}

u32 do_in_list(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    ListExpression_s list = params[1];
    int res = 0;
    for (u32 i = 0; i < list->param_exprs.size(); ++i) {
        CHECK(list->param_exprs[i]->get_result(ctx));
        CHECK(lhs->compare(ctx->output_result, res));
        if (0 == res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    ctx->bool_result = false;
    return ret;
}

u32 do_not_in_list(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    if (lhs->is_null()) {
        ctx->bool_result = false;
        return ret;
    }
    ListExpression_s list = params[1];
    int res = 0;
    for (u32 i = 0; i < list->param_exprs.size(); ++i) {
        CHECK(list->param_exprs[i]->get_result(ctx));
        if (ctx->output_result->is_null()) {
            ctx->bool_result = false;
            return ret;
        }
        CHECK(lhs->compare(ctx->output_result, res));
        if (0 == res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    ctx->bool_result = true;
    return ret;
}

u32 do_in(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (0 == res) {
            ctx->bool_result = true;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_not_in(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    if (lhs->is_null()) {
        ctx->bool_result = false;
        return ret;
    }
    SubplanExpression_s subquery = params[1];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    int res;
    while (SUCC(subquery->get_next_result(rhs))) {
        CHECK(lhs->compare(rhs, res));
        if (rhs->is_null()) {
            ctx->bool_result = false;
            return ret;
        }
        if (0 == res) {
            ctx->bool_result = false;
            return ret;
        }
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_exists(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    SubplanExpression_s subquery = params[0];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    while (SUCC(subquery->get_next_result(rhs))) {
        ctx->bool_result = true;
        return ret;
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = false;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_not_exists(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    SubplanExpression_s subquery = params[0];
    CHECK(subquery->set_exec_param(ctx));
    Object_s rhs;
    while (SUCC(subquery->get_next_result(rhs))) {
        ctx->bool_result = false;
        return ret;
    }
    if (NO_MORE_ROWS == ret) {
        ctx->bool_result = true;
        ret = SUCCESS;
    }
    return ret;
}

u32 do_and(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (!ctx->bool_result) {
        return ret;
    }
    CHECK(params[1]->get_result(ctx));
    return ret;
}

u32 do_or(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    if (ctx->bool_result) {
        return ret;
    }
    CHECK(params[1]->get_result(ctx));
    return ret;
}

u32 do_not(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    ctx->bool_result = !ctx->bool_result;
    return ret;
}

u32 do_like(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Varchar_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Varchar_s rhs = ctx->output_result;
    CHECK(Varchar::like(lhs, rhs->to_string(), "", ctx->bool_result));
    return ret;
}

u32 do_not_like(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Varchar_s lhs = ctx->output_result;
    if (lhs->is_null()) {
        ctx->bool_result = false;
        return ret;
    }
    CHECK(params[1]->get_result(ctx));
    Varchar_s rhs = ctx->output_result;
    CHECK(Varchar::like(lhs, rhs->to_string(), "", ctx->bool_result));
    ctx->bool_result = !ctx->bool_result;
    return ret;
}

u32 do_cast(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Object_s lhs = ctx->output_result;
    CHECK(lhs->cast_to(params[1]->res_type, ctx->output_result));
    if (lhs->is_null()) {
        ctx->output_result->set_null();
    }
    return ret;
}

u32 do_ifnull(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
	u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Varchar_s lhs = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
    Varchar_s rhs = ctx->output_result;
    if (lhs->is_null()) {
        ctx->output_result = rhs;
    } else {
        ctx->output_result = lhs;
    }
    return ret;
}

u32 do_substr(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
    Varchar_s lhs = ctx->output_result;
    if (lhs->is_null()) {
        ctx->output_result = lhs;
        return ret;
    }
    CHECK(params[1]->get_result(ctx));
    double start = ctx->output_result->value();
    CHECK(params[2]->get_result(ctx));
    double len = ctx->output_result->value();
    Varchar_s res;
	CHECK(Varchar::substr(lhs, start, len, res));
    ctx->output_result = res;
	return ret;
}

u32 do_to_char(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
	if (params.size() == 1) {
        CHECK(params[0]->get_result(ctx));
		ctx->output_result = Varchar::make_object(ctx->output_result->to_string());
	} else {
        CHECK(params[0]->get_result(ctx));
        DateTime_s time = ctx->output_result;
        CHECK(params[1]->get_result(ctx));
        String str;
        CHECK(DateTime::format_datetime(time, ctx->output_result->to_string(), str));
        ctx->output_result = Varchar::make_object(str);
	}
    return ret;
}

u32 do_to_number(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
	ctx->output_result = Number::make_object(ctx->output_result->value());
    return ret;
}

u32 do_case_when(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
	if (params.size() & 1) {
		//calc case when expr1 then res1 else resN end
		for (u32 i = 0; i < params.size() / 2; i += 2) {
            CHECK(params[i]->get_result(ctx));
			if (ctx->bool_result) {
				CHECK(params[i+1]->get_result(ctx));
                return ret;
			}
		}
        CHECK(params[params.size()-1]->get_result(ctx));
	} else if (params.size() > 1) {
		//calc case cond when expr1 then res1 else resN end
		CHECK(params[0]->get_result(ctx));
        Object_s lhs = ctx->output_result;
        int res = 0;
		for (u32 i = 1; i < params.size() / 2; i += 2) {
            CHECK(params[i]->get_result(ctx));
            CHECK(ctx->output_result->compare(lhs, res));
			if (0 == res) {
				CHECK(params[i+1]->get_result(ctx));
                return ret;
			}
		}
		CHECK(params[params.size()-1]->get_result(ctx));
	}
    return ret;
}

u32 do_date_add(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
	DateTime_s time = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
	Number_s seconds = ctx->output_result;
    DateTime_s res;
    CHECK(DateTime::add_seconds(time, seconds, res));
    ctx->output_result = res;
    return ret;
}

u32 do_date_sub(const Vector<Expression_s> &params, ExecCtx_s &ctx)
{
    u32 ret = SUCCESS;
    CHECK(params[0]->get_result(ctx));
	DateTime_s time = ctx->output_result;
    CHECK(params[1]->get_result(ctx));
	Number_s seconds = ctx->output_result;
    DateTime_s res;
    CHECK(DateTime::sub_seconds(time, seconds, res));
    ctx->output_result = res;
    return ret;
}