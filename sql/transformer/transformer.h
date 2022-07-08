#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include "type.h"
#define TRANSFORM(class, stmt, ctx, happened) \
        do { \
            CHECK(ctx->query_ctx->check_query_status()); \
            class rule; \
            rule.set_transform_ctx(ctx); \
            CHECK(rule.transform(stmt)); \
            bool trans_happened = rule.transform_happened(); \
            happened |= trans_happened;\
            LOG_TRACE("succeed to transform a rule ", \
            K(stmt), K(ctx->query_ctx->query_hint), V(#class), K(trans_happened)); \
        } while(0);
namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
    }
    namespace Sql {
        DECLARE(QueryCtx);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Sql::QueryCtx_s;
        DECLARE(TransformCtx);
        struct TransformCtx {
            TransformCtx();
            ~TransformCtx();
            static TransformCtx_s make_transform_ctx();
            QueryCtx_s query_ctx;
        };

        class Transformer {
        public:
            Transformer()=default;
            ~Transformer()=default;
            u32 transform(DMLStmt_s &stmt, TransformCtx_s &ctx);
        };
    }
}

#endif  //TRANSFORMER_H