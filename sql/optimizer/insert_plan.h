#ifndef INSERT_PLAN_H
#define INSERT_PLAN_H
#include "type.h"
#include "dml_plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Optimizer {
		using Parser::Stmt_s;
		
		class InsertPlan : public DMLPlan
		{
		protected:
			InsertPlan();
		public:
			~InsertPlan();
			static Plan_s make_insert_plan(const Stmt_s& lex_insert_stmt);
			PlanType type() const override;
			u32 generate_value_plan_tree();
			u32 generate_plan_tree() override;
		};
	}
}

#endif	//INSERT_PLAN_H