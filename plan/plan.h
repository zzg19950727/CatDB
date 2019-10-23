#ifndef PLAN_H
#define PLAN_H
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
	}
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		DECLARE(Plan);
		DECLARE(PhyOperator);
		using Parser::Stmt_s;
		using Common::Object_s;

		class Plan
		{
		public:
			enum PlanType {
				INSERT = 0,
				DELETE,
				UPDATE,
				SELECT
			};
		protected:
			Plan();
		public:
			virtual ~Plan();
			static Plan_s make_plan(const Stmt_s& lex_stmt);
			void set_lex_stmt(const Stmt_s& stmt);
			Stmt_s get_lex_stmt()const;
			/*finish later
			void set_thd(const Thd_s& thd);
			Thd_s get_thd()const;
			*/
			Object_s get_result();
			u32 affect_rows()const;
			virtual u32 execute() = 0;
			virtual u32 build_plan() = 0;
			virtual u32 optimizer() = 0;
			virtual PlanType type() const = 0;
			
		protected:
			Stmt_s lex_stmt;
			Object_s result;
			PhyOperator_s root_operator;
			u32 affect_rows_;
		};
	}
}

#endif	//PLAN_H