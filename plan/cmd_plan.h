#ifndef CMD_PLAN_H
#define CMD_PLAN_H
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(CMDStmt);
	}
	namespace Sql {
		DECLARE(Plan);
		using Parser::Stmt_s;

		class CMDPlan : public Plan
		{
		protected:
			CMDPlan();
		public:
			~CMDPlan();
			static Plan_s make_cmd_plan(const Stmt_s& lex_cmd_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
			bool send_plan_result()const;
			u32 get_stmt(Parser::CMDStmt *&stmt);
            u32 do_cmd_create_table();
            u32 do_cmd_drop_table();
            u32 do_cmd_create_database();
            u32 do_cmd_drop_database();
            u32 do_cmd_show_tables();
            u32 do_cmd_show_databases();
            u32 do_cmd_desc_table();
			u32 desc_table(const String &database, const String &table);
			u32 show_table_statis(const String &database, const String &table);
			u32 show_column_statis(const String &database, const String &table);
            u32 do_cmd_use_database();
            u32 do_cmd_analyze();
		};
	}
}

#endif	//CMD_PLAN_H