#include "update_plan.h"
#include "schema_checker.h"
#include "update_stmt.h"
#include "expr_stmt.h"
#include "log_update.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

UpdatePlan::UpdatePlan()
{

}

UpdatePlan::~UpdatePlan()
{

}

Plan_s UpdatePlan::make_update_plan(const Stmt_s& lex_update_stmt)
{
	UpdatePlan* plan = new UpdatePlan;
	plan->set_lex_stmt(lex_update_stmt);
	return Plan_s(plan);
}

Plan::PlanType UpdatePlan::type() const
{
	return Plan::UPDATE_PLAN;
}

u32 UpdatePlan::generate_plan_tree()
{
	u32 ret = SUCCESS;
	UpdateStmt_s stmt = lex_stmt;
	CHECK(generate_join_order());
	Vector<ColumnStmt_s> column_exprs;
	Vector<ExprStmt_s> value_exprs;
	CHECK(check_assign_exprs(column_exprs, value_exprs));
	CHECK(generate_subquery_evaluate(root_operator,
									 value_exprs, 
									 false));
	root_operator = LogUpdate::make_update(root_operator,
										   stmt->table,
										   stmt->update_assign_stmt,
										   column_exprs,
										   value_exprs,
										   stmt->row_id_col);
	root_operator->init(query_ctx, est_info);
	LogUpdate_s log_update = root_operator;
	CHECK(root_operator->compute_property());
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	CHECK(checker->get_row_desc(stmt->table->ref_table_id, log_update->row_desc));
	
    ColumnDesc col_desc;
    for (u32 i = 0; i < column_exprs.size(); ++i) {
        ColumnStmt_s col_expr = column_exprs[i];
        col_desc.set_cid(col_expr->column_id);
        col_desc.set_data_type(col_expr->res_type);
        log_update->update_desc.add_column_desc(col_desc);
    }
	return ret;
}

u32 UpdatePlan::check_assign_exprs(Vector<ColumnStmt_s> &column_exprs,
								   Vector<ExprStmt_s> &value_exprs)
{
    u32 ret = SUCCESS;
	UpdateStmt_s stmt = lex_stmt;
    for (u32 i = 0; i < stmt->update_assign_stmt.size(); ++i) {
        ExprStmt_s value_expr;
        ColumnStmt_s column_expr;
        CHECK(check_assign_expr(stmt->update_assign_stmt[i],
                                column_expr,
                                value_expr));
        column_exprs.push_back(column_expr);
        value_exprs.push_back(value_expr);                        
    }
    return ret;
}

u32 UpdatePlan::check_assign_expr(ExprStmt_s &assign_expr,
								  ColumnStmt_s &column_expr,
								  ExprStmt_s &value_expr)
{
    u32 ret = SUCCESS;
	UpdateStmt_s stmt = lex_stmt;
    MY_ASSERT(assign_expr, OP_EXPR == assign_expr->expr_type());
    OpExprStmt_s expr = assign_expr;
    MY_ASSERT(OP_ASSIGN == expr->op_type, 2 == expr->params.size());
    MY_ASSERT(COLUMN == expr->params[0]->expr_type());
    column_expr = expr->params[0];
    MY_ASSERT(column_expr->table_id == stmt->table->table_id);
    value_expr = expr->params[1];
    return ret;
}