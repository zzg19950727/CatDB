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
		public:
			String database;
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
		private:
			bool is_select_current_database;
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
			u32 desc_table();
			u32 show_table_statis();
			u32 show_column_statis();
			String database;
			String table;
			bool is_show_table_statis;
			bool is_show_column_statis;
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

		class AnalyzePlan :public Plan
		{
		protected:
			AnalyzePlan();
		public:
			~AnalyzePlan();
			static Plan_s make_analyze_plan(const Stmt_s& lex_show_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			String database;
			String table;
		};
	}
}

#endif	//SHOW_PLAN_H