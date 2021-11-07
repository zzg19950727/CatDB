#ifndef UPDATE_PLAN_H
#define UPDATE_PLAN_H
#include "type.h"
#include "dml_plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Optimizer {
		using Parser::Stmt_s;

		class UpdatePlan : public DMLPlan
		{
		protected:
			UpdatePlan();
		public:
			~UpdatePlan();
			static Plan_s make_update_plan(const Stmt_s& lex_update_stmt);
			PlanType type() const;
			u32 generate_plan_tree();
			u32 check_assign_exprs(Vector<ColumnStmt_s> &column_exprs,
								   Vector<ExprStmt_s> &value_exprs);
			u32 check_assign_expr(ExprStmt_s &assign_expr,
								  ColumnStmt_s &column_expr,
								  ExprStmt_s &value_expr);
		};
	}
}

#endif	//UPDATE_PLAN_H