#ifndef TRANSFORM_RULE_H
#define TRANSFORM_RULE_H
#include "transformer.h"
#include "type.h"
#define PRE_ORDER   true
#define POST_ORDER  false

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
    }
    namespace Transform {
        DECLARE(TransformCtx);
        using Parser::DMLStmt_s;

        class TransformRule {
        public:
            TransformRule(bool pre_order,
                          HintType control_hint = INVALID_HINT);
            virtual ~TransformRule();
            void set_transform_ctx(TransformCtx_s &ctx);
            u32 transform(DMLStmt_s stmt);
            bool transform_happened() const { return trans_happened; }
            void set_transform_happened() { trans_happened = true; }
            virtual bool need_rewrite(DMLStmt_s stmt) const;

        protected:
            virtual u32 transform_one_stmt(DMLStmt_s &stmt);

        private:
            u32 transform_pre_order(DMLStmt_s &stmt);
            u32 transform_post_order(DMLStmt_s &stmt);
            u32 transform_self(DMLStmt_s &stmt);
            u32 transform_chidren(DMLStmt_s &stmt);

        protected:
            bool pre_order;
            HintType control_hint;

        protected:
            TransformCtx_s ctx;
            bool trans_happened;
        };
    }
}

#endif  //TRANSFORM_RULE_H