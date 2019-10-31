#ifndef CREATE_PLAN_H
#define CREATE_PLAN_H
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Sql {
		DECLARE(Plan);
		using Parser::Stmt_s;

		class CreateTablePlan : public Plan
		{
		protected:
			CreateTablePlan();
		public:
			~CreateTablePlan();
			static Plan_s make_create_table_plan(const Stmt_s& lex_create_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
			String table;
			Vector<Pair<String, String>> columns;
		};

		class CreateDatabasePlan : public Plan
		{
		protected:
			CreateDatabasePlan();
		public:
			~CreateDatabasePlan();
			static Plan_s make_create_database_plan(const Stmt_s& lex_create_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
		};
	}
}

#endif	//CREATE_PLAN_H