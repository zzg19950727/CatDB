#ifndef INSERT_RESOLVER_H
#define INSERT_RESOLVER_H
#include "dml_resolver.h"
#include "row.h"

namespace CatDB {
	namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(ListStmt);
        DECLARE(InsertStmt);
        DECLARE(SelectStmt);
        class InsertResolver : public DMLResolver
        {
        public:
            InsertResolver(InsertStmt_s &stmt, ResolveCtx &resolve_ctx);
            ~InsertResolver();
            u32 resolve_stmt();
            u32 check_insert_value(BasicTableStmt_s &insert_table, Vector<Vector<ExprStmt_s>> &value_list);
            u32 check_insert_value(BasicTableStmt_s &insert_table, SelectStmt_s &select_value);
            u32 resolve_row(Vector<ExprStmt_s>& list, const Common::RowDesc& row_desc);
        private:
            InsertStmt_s insert_stmt;
        };
    }
}

#endif  //INSERT_RESOLVER_H