#ifndef SQL_ENGINE_H
#define SQL_ENGINE_H
#include "query_ctx.h"
#include "type.h"
#include "row.h"

namespace CatDB {
    namespace Common {
        DECLARE(Object);
        DECLARE(QueryResult);
    }
    namespace Parser {
        DECLARE(Stmt);
        DECLARE(DMLStmt);
    }

    namespace Optimizer {
        DECLARE(Plan);
        DECLARE(LogicalOperator);
    }

	namespace Sql {
        DECLARE(PhyOperator);
        DECLARE(SqlEngine);
        DECLARE(ResultSet);
        using Common::RowDesc;
        using Common::Row_s;
        using Common::Object_s;
        using Common::QueryResult_s;
        using Parser::Stmt_s;
        using Parser::DMLStmt_s;
        using Optimizer::Plan_s;
        using Optimizer::LogicalOperator_s;

        class ResultSet {
        private:
			ResultSet(u32 column_count);

		public:
			~ResultSet();
			static ResultSet_s make_result_set(u32 column_count=0);
            u32 get_column_num() const;
            void set_column_num(u32 count);
            u32 set_result_title(u32 idx, const String &title);
            String get_result_title(u32 idx) const;
            u32 set_result_type(u32 idx, u32 type);
            u32 get_result_type(u32 idx) const;
            u32 add_row(Row_s &row);
            u32 add_row(Vector<Object_s> &cells);
            u32 add_row(Object_s &cell);
            QueryResult_s get_query_result();
            u32 get_affect_rows() const;
            void set_explain_info(const String& info) { explain_info = info; }
            String get_explain_info() const { return explain_info; }
            bool need_send_result() const { return have_user_data; }
			
		private:
			QueryResult_s query_result;
            Vector<String> result_title;
            Vector<u32> result_type;
            RowDesc desc;
			u32 affect_rows;
            String explain_info;
            bool have_user_data;
        private:
            DISALLOW_COPY_AND_ASSIGN(ResultSet);
        };

		class SqlEngine
		{
		private:
			SqlEngine() = delete;
			SqlEngine(const String& query, QueryCtx& query_ctx);
		public:
			~SqlEngine();
			static SqlEngine_s make_sql_engine(const String& query, QueryCtx &query_ctx);
            static u32 handle_inner_sql(const String &query, QueryCtx &query_ctx, ResultSet_s &result_set);
            static u32 handle_subplan(PhyOperator_s root, Object_s &result);
            u32 handle_query();
			ResultSet_s get_query_result();
            const String& get_error_msg() const {return err_msg;}

        private:
            u32 execute_plan(PhyOperator_s root);
            u32 explain_plan(LogicalOperator_s root);
            u32 print_outline(String &outline);
            u32 print_stmt_outline(DMLStmt_s stmt, String &outline);

		private:
            QueryCtx &query_ctx;
            String query;
            Stmt_s lex_stmt;
            Plan_s plan;
            LogicalOperator_s log_root;
            PhyOperator_s phy_root;
			ResultSet_s query_result;
            String err_msg;
        private:
            DISALLOW_COPY_AND_ASSIGN(SqlEngine);
		};
	}
}

#endif	//SQL_ENGINE_H
