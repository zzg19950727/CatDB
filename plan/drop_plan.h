#ifndef DROP_PLAN_H
#define DROP_PLAN_H
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Sql {
		DECLARE(Plan);
		using Parser::Stmt_s;

		class DropTablePlan : public Plan
		{
		protected:
			DropTablePlan();
		public:
			~DropTablePlan();
			static Plan_s make_drop_table_plan(const Stmt_s& lex_drop_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
			String table;
		};

		class DropDatabasePlan : public Plan
		{
		protected:
			DropDatabasePlan();
		public:
			~DropDatabasePlan();
			static Plan_s make_drop_database_plan(const Stmt_s& lex_drop_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
		};
	}
}

#endif	//DROP_PLAN_H