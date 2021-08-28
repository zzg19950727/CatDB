#ifndef SELECT_RESOLVER_H
#define SELECT_RESOLVER_H
#include "dml_resolver.h"

namespace CatDB {
	namespace Parser {
        DECLARE(SetStmt);
        DECLARE(SelectStmt);
        class SelectResolver : public DMLResolver
        {
        public:
            SelectResolver(SelectStmt_s &stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx);
            ~SelectResolver();
            u32 resolve_stmt();
        private:
            u32 resolve_select_list();
            u32 resolve_order_exprs(Vector<ExprStmt_s> &order_exprs);
            u32 resolve_all_column(ExprStmt_s &stmt, Vector<ExprStmt_s> &columns);
            u32 get_all_column_from_table(TableStmt_s &table, Vector<ExprStmt_s> &columns);
            u32 get_all_column_from_basic_table(BasicTableStmt_s table, Vector<ExprStmt_s> &columns);
            u32 get_all_column_from_view_table(ViewTableStmt_s table, Vector<ExprStmt_s> &columns);
            SelectStmt_s select_stmt;
        };
        class SetResolver : public DMLResolver
        {
        public:
            SetResolver(SetStmt_s &stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx);
            ~SetResolver();
            u32 resolve_stmt();
        private:
            SetStmt_s &set_stmt;
        };
    }
}

#endif  //SELECT_RESOLVER_H