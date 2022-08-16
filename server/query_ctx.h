#ifndef QUERY_CTX_H
#define QUERY_CTX_H
#include "timer_manager.h"
#include "hint_stmt.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(QueryHint);
    }
	namespace Server {
        DECLARE(QueryCtx);
        using Parser::QueryHint_s;
        class QueryCtx {
        public:
            QueryCtx();
            virtual ~QueryCtx() {}
            void reset();
            static QueryCtx_s make_query_ctx();
            void generate_trace_id();
            const String& get_trace_id() const { return trace_id; }

            u32 generate_table_id() {return cur_table_id++; }
            u32 generate_stmt_id() { return cur_stmt_id++; }
            String generate_view_name() { return "VIEW" + std::to_string(cur_view_id++); }
            u32 generate_param_index() { return param_index++; }

            void set_sample_size(double size) { sample_size = size; }
            double get_sample_size() const { return sample_size; }

            void set_error_msg(const String& msg) { err_msg = msg; }
            String get_error_msg() const { return err_msg; }
            
            void increase_affected_rows() { ++affected_rows; }
            u32 get_affected_rows() const { return affected_rows; }

        public:
            //query`s hint
            QueryHint_s query_hint;
            //first error message
            String err_msg;
            //trace query
            String last_trace_id;
			String trace_id;
            //generate unique id for table,stmt,view,param
            u32 cur_table_id;
            u32 cur_stmt_id;
            u32 cur_view_id;
            u32 param_index;
            //query sample size
            double sample_size;
            //dml query affected rows
            u32 affected_rows;
        };
    }
}
#endif //QUERY_CTX_H