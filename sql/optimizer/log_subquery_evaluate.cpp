#include "log_subquery_evaluate.h"
#include "expr_stmt.h"

using namespace CatDB::Optimizer;

LogicalOperator_s LogSubQueryEvaluate::make_subquery_evaluate(LogicalOperator_s &query_root)
{
    LogSubQueryEvaluate *subquery_evaluate = new LogSubQueryEvaluate();
    subquery_evaluate->childs.push_back(query_root);
    return LogicalOperator_s(subquery_evaluate);
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
    info.expr_info += "subquery_infos:\n";
    for (u32 i = 0; i < subqueries.size(); ++i) {
        info.expr_info += subqueries[i]->to_string();
        info.expr_info += " exec_params:";
        print_exprs(subqueries[i]->get_params(), info.expr_info);
    }
    plan_info.push_back(info);
    for (u32 i = 0; i < childs.size(); ++i) {
        childs[i]->print_plan(depth + 1, plan_info);
    }
}