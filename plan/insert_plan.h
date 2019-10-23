#ifndef INSERT_PLAN_H
#define INSERT_PLAN_H
#include "type.h"
#include "plan.h"
#include "row.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Sql {
		DECLARE(Plan);
		using Parser::Stmt_s;
		using Common::Row_s;
		using Common::RowDesc;
		
		class InsertPlan : public Plan
		{
		protected:
			InsertPlan();
		public:
			~InsertPlan();
			static Plan_s make_insert_plan(const Stmt_s& lex_insert_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			u32 check_column_value(const Stmt_s& list, const RowDesc& row_desc)const;
			u32 resolve_row(const Stmt_s& list, const RowDesc& row_desc, Row_s& row);

			Vector<Row_s> values;
		};
	}
}

#endif	//INSERT_PLAN_H