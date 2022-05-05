#ifndef SELECT_PLAN_H
#define SELECT_PLAN_H
#include "dml_plan.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(WinExprStmt);
	}
	namespace Optimizer {
		DECLARE(Plan);
		using Parser::Stmt_s;
		using Parser::WinExprStmt_s;

		class SelectPlan : public DMLPlan
		{
		protected:
			SelectPlan();
		public:
			~SelectPlan();
			static Plan_s make_select_plan(const Stmt_s& lex_select_stmt);
			PlanType type() const override;
			u32 generate_plan_tree() override;
			u32 generate_set_plan_tree();
			u32 generate_normal_plan_tree();
			u32 generate_group_by();
			u32 generate_scalar_group_by();
			u32 generate_window_function();
			u32 generate_one_window_function(WinExprStmt_s win_expr);
			u32 generate_distinct();
			u32 generate_order_by(bool &need_limit);
			u32 generate_limit();
		public:
			
		};
	}
}

#endif	//SELECT_PLAN_H