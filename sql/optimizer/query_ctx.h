#ifndef QUERY_CTX_H
#define QUERY_CTX_H
#include "type.h"

namespace CatDB {
	namespace Sql {
        DECLARE(QueryCtx);
        class QueryCtx {
        public:
            QueryCtx()
            :cur_table_id(1),
            cur_stmt_id(1),
            sample_size(1),
            cur_view_id(1),
            killed(false)
            {}
            void reset();
            static QueryCtx_s make_query_ctx();
            u32 generate_table_id() {return cur_table_id++; }
            u32 generate_stmt_id() { return cur_stmt_id++; }
            String generate_view_name() { return "VIEW" + std::to_string(cur_view_id++); }
            void set_sample_size(double size) { sample_size = size; }
            double get_sample_size() const { return sample_size; }
            void set_error_msg(const String& msg) { err_msg = msg; }
            String get_error_msg() const { return err_msg; }
            u32 check_query_status();
            void increase_affected_rows() { ++affected_rows; }
            u32 get_affected_rows() const { return affected_rows; }

        public:
            String cur_database;
            String err_msg;
            u32 cur_table_id;
            u32 cur_stmt_id;
            u32 cur_view_id;
            double sample_size;
            u32 affected_rows;
            u32 pid;
            bool killed;
        };
    }
}
#endif //QUERY_CTX_H