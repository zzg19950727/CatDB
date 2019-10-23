#ifndef DELETE_PLAN_H
#define DELETE_PLAN_H
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		DECLARE(Plan);
		DECLARE(Filter);
		DECLARE(Expression);
		using Common::Row_s;
		using Parser::Stmt_s;
		using Sql::Filter_s;
		using Sql::Expression_s;

		class DeletePlan : public Plan
		{
		protected:
			DeletePlan();
		public:
			~DeletePlan();
			static Plan_s make_delete_plan(const Stmt_s& lex_insert_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			u32 resolve_filter(const Stmt_s& where_stmt, Filter_s& filter);
			u32 resolve_expr(const Stmt_s& expr_stmt, Expression_s& expr);
			String database;
			String table_name;
			Row_s row_access;
		};
	}
}

#endif	//DELETE_PLAN_H