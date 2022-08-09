#ifndef TRANSFORM_PRE_PROCESS_H
#define TRANSFORM_PRE_PROCESS_H
#include "transform_rule.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExprStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::ExprStmt_s;

        class TransformPreProcess : public TransformRule {
        public:
            TransformPreProcess();
            ~TransformPreProcess();
            virtual bool need_rewrite(DMLStmt_s stmt) const override;
            virtual u32 transform_one_stmt(DMLStmt_s &stmt) override;
        private:
            u32 simplify_calculable_expr(DMLStmt_s &stmt, bool &happened);
            u32 inner_simplify_calculable_expr(ExprStmt_s &expr, bool &happened);
        };
    }
}

#endif  //TRANSFORM_PRE_PROCESS_H