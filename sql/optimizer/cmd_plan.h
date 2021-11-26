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
		DECLARE(ResultSet);
	}
	namespace Optimizer {
		DECLARE(Plan);
		DECLARE(CMDPlan);
		using Sql::ResultSet_s;
		using Parser::Stmt_s;

		class CMDPlan : public Plan
		{
		protected:
			CMDPlan();
		public:
			~CMDPlan();
			static Plan_s make_cmd_plan(const Stmt_s& lex_cmd_stmt);
			u32 build_plan();
			u32 execute(ResultSet_s &query_result);
			PlanType type() const;
            u32 do_cmd_create_table();
            u32 do_cmd_drop_table();
            u32 do_cmd_create_database();
            u32 do_cmd_drop_database();
            u32 do_cmd_show_tables(ResultSet_s &query_result);
            u32 do_cmd_show_databases(ResultSet_s &query_result);
            u32 do_cmd_desc_table(ResultSet_s &query_result);
			u32 desc_table(const String &database, const String &table, ResultSet_s &query_result);
			u32 show_table_statis(const String &database, const String &table, ResultSet_s &query_result);
			u32 show_column_statis(const String &database, const String &table, ResultSet_s &query_result);
            u32 do_cmd_use_database();
            u32 do_cmd_analyze();
			u32 do_set_var();
			u32 do_show_processlist(ResultSet_s &query_result);
			u32 do_kill_process();
			u32 do_show_memory(ResultSet_s &query_result);
		};
	}
}

#endif	//CMD_PLAN_H