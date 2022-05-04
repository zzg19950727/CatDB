#ifndef TRANSFORM_EXPR_NORMALIZE_H
#define TRANSFORM_EXPR_NORMALIZE_H
#include "transform_rule.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(OpExprStmt);
    }
    namespace Transform {
        using Parser::ExprStmt_s;
        using Parser::OpExprStmt_s;

        class TransformExprNormalize : public TransformRule {
        public:
            TransformExprNormalize();

            ~TransformExprNormalize();

            virtual bool need_rewrite(DMLStmt_s stmt) const override;

            virtual u32 transform_one_stmt(DMLStmt_s &stmt) override;
        private:
            u32 transform_exprs(Vector<ExprStmt_s> &exprs);

            u32 transform_one_expr(ExprStmt_s &expr, 
                                   Vector<ExprStmt_s> &exprs, 
                                   bool &is_false, 
                                   bool &is_true);

            u32 transform_and_expr(OpExprStmt_s &expr, 
                                   Vector<ExprStmt_s> &exprs, 
                                   bool &is_false, 
                                   bool &is_true);

            u32 transform_or_expr(OpExprStmt_s &expr, 
                                  Vector<ExprStmt_s> &exprs, 
                                  bool &is_false, 
                                  bool &is_true);

            u32 check_hint_disable(DMLStmt_s& stmt, bool &is_disable) const;

            u32 generate_outline(DMLStmt_s& stmt);
        };
    }
}

#endif  //TRANSFORM_EXPR_NORMALIZE_H