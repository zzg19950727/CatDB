#ifndef PLAN_H
#define PLAN_H
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Sql {
		DECLARE(QueryCtx);
	}
	namespace Optimizer {
		DECLARE(Plan);
		DECLARE(LogicalOperator);
		using Sql::QueryCtx;
		using Parser::Stmt_s;
	
		class Plan
		{
		public:
			enum PlanType {
				INSERT_PLAN = 0,
				DELETE_PLAN,
				UPDATE_PLAN,
				SELECT_PLAN,
				CMD_PLAN
			};
		protected:
			Plan();
			
		public:
			virtual ~Plan();
			static Plan_s make_plan(const Stmt_s& lex_stmt, QueryCtx *ctx);
			void set_lex_stmt(const Stmt_s& stmt);
			Stmt_s get_lex_stmt()const;
			virtual u32 build_plan() = 0;
			virtual PlanType type() const = 0;
			virtual u32 generate_plan_tree(){return 0;}
			LogicalOperator_s get_root_operator();
			u32 explain_plan();

		protected:
			Stmt_s lex_stmt;
			LogicalOperator_s root_operator;
			QueryCtx *query_ctx;
		};
	}
}

#endif	//PLAN_H