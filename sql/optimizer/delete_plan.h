#ifndef DELETE_PLAN_H
#define DELETE_PLAN_H
#include "dml_plan.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Optimizer {
		using Parser::Stmt_s;

		class DeletePlan : public DMLPlan
		{
		protected:
			DeletePlan();
		public:
			~DeletePlan();
			static Plan_s make_delete_plan(const Stmt_s& lex_delete_stmt);
			PlanType type() const;
			u32 generate_plan_tree();
		};
	}
}

#endif	//DELETE_PLAN_H