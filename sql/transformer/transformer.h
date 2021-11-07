#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include "query_ctx.h"
#include "type.h"
#define TRANSFORM(class, stmt, ctx, happened) \
        do { \
            class rule; \
            rule.set_transform_ctx(ctx); \
            CHECK(rule.transform(stmt)); \
            happened |= rule.transform_happened(); \
            LOG_TRACE("succeed to transform a rule ", V(#class), K(stmt), K(happened)); \
        } while(0);
namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        DECLARE(TransformCtx);
        struct TransformCtx {
            static TransformCtx_s make_transform_ctx();
            Sql::QueryCtx *query_ctx;
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