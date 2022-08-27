#ifndef SQL_ENGINE_H
#define SQL_ENGINE_H
#include "type.h"
#include "row.h"

namespace CatDB {
    namespace Common {
        DECLARE(Object);
    }
    namespace Parser {
        DECLARE(Stmt);
        DECLARE(ExprStmt);
        DECLARE(DMLStmt);
        DECLARE(SelectStmt);
        DECLARE(ResolveCtx);
    }

    namespace Optimizer {
        DECLARE(Plan);
        DECLARE(LogicalOperator);
    }

	namespace Sql {
        DECLARE(PhyOperator);
        DECLARE(SqlEngine);
        DECLARE(ResultSet);
        using Common::Row_s;
        using Common::Object_s;
        using Parser::Stmt_s;
        using Parser::ExprStmt_s;
        using Parser::DMLStmt_s;
        using Parser::SelectStmt_s;
        using Parser::ResolveCtx;
        using Optimizer::Plan_s;
        using Optimizer::LogicalOperator_s;

        class ResultSet {
        private:
			ResultSet();

		public:
			~ResultSet();
			static ResultSet_s make_result_set();
            u32 add_result_title(const String &title, OBJ_TYPE type);
            String get_result_title(u32 idx) const;
            OBJ_TYPE get_result_type(u32 idx) const;
            void add_row(const Row_s& row);
            void set_op_root(PhyOperator_s &root);
            u32 get_column_count() const { return result_type.size(); }

            void set_explain_info(const String& info) { is_explain = true; explain_info = info; }
            String get_explain_info() const { return explain_info; }
            bool need_send_result() const { return have_user_data; }
            bool is_explain_plan() const { return is_explain; }

            u32 open();
            u32 get_next_row(Row_s &row);
            u32 close();
			
		private:
            Vector<Row_s> cache_rows;
            Vector<String> result_title;
            Vector<OBJ_TYPE> result_type;
            String explain_info;
            PhyOperator_s phy_root;
            u32 row_idx;
            bool have_user_data;
            bool is_explain;
        private:
            DISALLOW_COPY_AND_ASSIGN(ResultSet);
        };

		class SqlEngine
		{
		private:
			SqlEngine() = delete;
			SqlEngine(const String& query);
		public:
			~SqlEngine();
			static SqlEngine_s make_sql_engine(const String& query);
            
            static u32 handle_inner_sql(const String &query);

            static u32 handle_inner_sql(const String &query,
                                        ResultSet_s &result_set);

            static u32 handle_user_view(const String &view, 
                                        ResolveCtx *ctx, 
                                        SelectStmt_s &ref_query);

            static u32 handle_const_expr(ExprStmt_s &expr, 
                                         Object_s &obj_result, 
                                         bool &bool_result);
            u32 handle_query();
			ResultSet_s get_query_result();

        private:
            u32 init_result_set();
            u32 explain_plan(LogicalOperator_s root, String &plan_info);
            u32 print_outline(String &outline);

		private:
            String query;
            Stmt_s lex_stmt;
            Plan_s plan;
            LogicalOperator_s log_root;
            PhyOperator_s phy_root;
			ResultSet_s query_result;
        private:
            DISALLOW_COPY_AND_ASSIGN(SqlEngine);
		};
	}
}

#endif	//SQL_ENGINE_H
