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
            sample_size (1)
            {}
            u32 generate_table_id() {return cur_table_id++; }
            String cur_database;
            u32 cur_table_id;
            double sample_size;
        };
    }
}
#endif //QUERY_CTX_H