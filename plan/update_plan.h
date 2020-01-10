#ifndef UPDATE_PLAN_H
#define UPDATE_PLAN_H
#include "expr_stmt.h"
#include "type.h"
#include "plan.h"
#include "row.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(TableStmt);
	}
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		DECLARE(Plan);
		DECLARE(Filter);
		DECLARE(Expression);
		using Common::Row_s;
		using Parser::Stmt_s;
		using Parser::TableStmt;
		using Sql::Filter_s;
		using Sql::Expression_s;
		using Common::Object_s;
		using Common::ColumnDesc;

		class UpdatePlan : public Plan
		{
		protected:
			UpdatePlan();
		public:
			~UpdatePlan();
			static Plan_s make_update_plan(const Stmt_s& lex_insert_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			u32 resolve_filter(const Stmt_s& where_stmt, Filter_s& filter);
			u32 resolve_expr(const Stmt_s& expr_stmt, Expression_s& expr);
			u32 resolve_update_row(const Stmt_s& asgn_stmt, Row_s& row);
			u32 resolve_cell(const Stmt_s& asign_stmt, ColumnDesc&col_desc, Object_s& cell);
			void add_access_column(const ColumnDesc& col_desc);
		private:
			String database;
			String table_name;
			String alias_table_name;
			TableStmt* table;
			Vector<ColumnDesc> access_columns;
			Row_s new_row;
			Filter_s filter;
		};
	}
}

#endif	//UPDATE_PLAN_H