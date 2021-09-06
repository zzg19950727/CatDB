#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include "type.h"
#define TRANSFORM(class, stmt, happened) \
        do { \
            class rule; \
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

        class Transformer {
        public:
            Transformer()=default;
            ~Transformer()=default;
            u32 transform(DMLStmt_s &stmt);
        };
    }
}

#endif  //TRANSFORMER_H