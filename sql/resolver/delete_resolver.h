#ifndef DELETE_RESOLVER_H
#define DELETE_RESOLVER_H
#include "dml_resolver.h"

namespace CatDB {
	namespace Parser {
        DECLARE(DeleteStmt);
        class DeleteResolver : public DMLResolver
        {
        public:
            DeleteResolver(DeleteStmt_s &stmt, ResolveCtx &resolve_ctx);
            ~DeleteResolver();
            u32 resolve_stmt();
        private:
            DeleteStmt_s delete_stmt;
        };
    }
}

#endif  //DELETE_RESOLVER_H