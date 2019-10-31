#ifndef SHOW_PLAN_H
#define SHOW_PLAN_H
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Sql {
		DECLARE(Plan);
		using Parser::Stmt_s;

		class ShowTablesPlan : public Plan
		{
		protected:
			ShowTablesPlan();
		public:
			~ShowTablesPlan();
			static Plan_s make_show_tables_plan(const Stmt_s& lex_show_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		};

		class ShowDatabasesPlan : public Plan
		{
		protected:
			ShowDatabasesPlan();
		public:
			~ShowDatabasesPlan();
			static Plan_s make_show_databases_plan(const Stmt_s& lex_show_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		};

		class DescTablePlan : public Plan
		{
		protected:
			DescTablePlan();
		public:
			~DescTablePlan();
			static Plan_s make_desc_table_plan(const Stmt_s& lex_show_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
			String table;
		};

		class UseDatabasePlan :public Plan
		{
		protected:
			UseDatabasePlan();
		public:
			~UseDatabasePlan();
			static Plan_s make_use_database_plan(const Stmt_s& lex_show_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
		};
	}
}

#endif	//SHOW_PLAN_H