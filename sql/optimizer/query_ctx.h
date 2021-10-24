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
            sample_size (1)
            {}
            void reset() { reset_table_id(); reset_stmt_id(); reset_sample_size(); err_msg.clear(); }
            u32 generate_table_id() {return cur_table_id++; }
            void reset_table_id() { cur_table_id = 1; }
            u32 generate_stmt_id() { return cur_stmt_id++; }
            void reset_stmt_id() { cur_stmt_id = 1; }
            void set_sample_size(double size) { sample_size = size; }
            double get_sample_size() const { return sample_size; }
            void reset_sample_size() { sample_size = 1.0; }
            void set_error_msg(const String& msg) { err_msg = msg; }
            String get_error_msg() const { return err_msg; }
            String cur_database;
            String err_msg;
            u32 cur_table_id;
            u32 cur_stmt_id;
            double sample_size;
        };
    }
}
#endif //QUERY_CTX_H