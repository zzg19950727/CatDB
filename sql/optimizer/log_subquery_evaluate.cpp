#include "log_subquery_evaluate.h"
#include "expr_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogSubQueryEvaluate::make_subquery_evaluate(LogicalOperator_s &query_root)
{
    LogSubQueryEvaluate *subquery_evaluate = new LogSubQueryEvaluate();
    subquery_evaluate->childs.push_back(query_root);
    return LogicalOperator_s(subquery_evaluate);
}

u32 LogSubQueryEvaluate::est_row_count()
{
    u32 ret = SUCCESS;
    output_rows = childs[0]->get_output_rows();
    return ret;
}

u32 LogSubQueryEvaluate::est_cost()
{
    u32 ret = SUCCESS;
    cost = 0.0;
    for (u32 i = 0; i < childs.size(); ++i) {
        cost += childs[i]->get_cost();
    }
    return ret;
}

void LogSubQueryEvaluate::add_subplan(SubQueryStmt_s& subquery_expr, LogicalOperator_s &subplan)
{
    subqueries.push_back(subquery_expr);
    childs.push_back(subplan);
}

void LogSubQueryEvaluate::print_plan(u32 depth, Vector<PlanInfo> &plan_info)
{
    PlanInfo info;
    print_basic_info(depth, info);
    info.op += "SUBQUERY EVALUATE";
    for (u32 i = 0; i < subqueries.size(); ++i) {
        print_exprs(subqueries[i]->get_params(), subqueries[i]->to_string() + " exec_params", info);
    }
    plan_info.push_back(info);
    for (u32 i = 0; i < childs.size(); ++i) {
        childs[i]->print_plan(depth + 1, plan_info);
    }
}