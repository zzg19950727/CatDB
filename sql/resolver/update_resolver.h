#ifndef UPDATE_RESOLVER_H
#define UPDATE_RESOLVER_H
#include "dml_resolver.h"

namespace CatDB {
	namespace Parser {
        DECLARE(UpdateStmt);
        class UpdateResolver : public DMLResolver
        {
        public:
            UpdateResolver(UpdateStmt_s &stmt, ResolveCtx &resolve_ctx);
            ~UpdateResolver();
            u32 resolve_stmt();
        private:
            u32 set_relation_for_assign_exprs();
            u32 check_assign_exprs();
            u32 check_assign_expr(ExprStmt_s &assign_expr,
                                  ColumnStmt_s &column_expr,
                                  ExprStmt_s &value_expr);
            UpdateStmt_s update_stmt;
        };
    }
}

#endif  //UPDATE_RESOLVER_H