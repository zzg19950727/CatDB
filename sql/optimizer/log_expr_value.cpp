#include "log_expr_value.h"
#include "expr_stmt.h"
#include "object.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

LogicalOperator_s LogExprValue::make_expr_value(Vector<Vector<ExprStmt_s>> &values)
{
    LogExprValue *expr_value = new LogExprValue();
    expr_value->values = values;
    return LogicalOperator_s(expr_value);
}

u32  LogExprValue::allocate_expr_pre()
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < values.size(); ++i) {
        append(expr_ctx.expr_consume, values[i]);
    }
    CHECK(LogicalOperator::allocate_expr_pre());
    if (!values.empty()) {
        for (u32 i = 0; i < values[0].size(); ++i) {
            ExprStmt_s expr = ConstStmt::make_const_stmt(Common::Number::make_object(i, -1));
            output_exprs.push_back(expr);
        }
    }
    return ret;
}

void LogExprValue::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info, "VALUE");
    info.op = "EXPR VALUE";
    plan_info.push_back(info);
}