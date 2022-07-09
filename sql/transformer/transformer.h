#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include "type.h"
#define TRANSFORM(class, stmt, ctx, happened) \
        do { \
            CHECK(GTX->check_query_status()); \
            class rule; \
            rule.set_transform_ctx(ctx); \
            CHECK(rule.transform(stmt)); \
            bool trans_happened = rule.transform_happened(); \
            happened |= trans_happened;\
            LOG_TRACE("succeed to transform a rule ", \
            K(stmt), K(QUERY_CTX->query_hint), V(#class), K(trans_happened)); \
        } while(0);
namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        DECLARE(TransformCtx);
        struct TransformCtx {
            TransformCtx();
            ~TransformCtx();
            static TransformCtx_s make_transform_ctx();
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