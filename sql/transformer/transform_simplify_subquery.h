#ifndef TRANSFORM_SIMPLIFY_SUBQUERY_H
#define TRANSFORM_SIMPLIFY_SUBQUERY_H
#include "transform_rule.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(SelectStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::ExprStmt_s;
        using Parser::SelectStmt_s;

        class TransformSimplifySubquery : public TransformRule {
        public:
            TransformSimplifySubquery();
            ~TransformSimplifySubquery();
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
        private:
            u32 transform_one_expr(ExprStmt_s &expr);

            u32 inner_transform_one_expr(ExprStmt_s &expr);

            u32 check_subquery_status(SelectStmt_s &subquery,
                                      bool &is_empty, 
                                      bool &output_one_row);

            u32 simplify_subquery(SelectStmt_s &subquery, 
                                  bool &happened,
                                  OperationType type = OP_INVALID);

            u32 check_hint_disable(SelectStmt_s& subquery, bool &is_disable);

            u32 generate_outline(SelectStmt_s& subquery);
        };
    }
}

#endif  //TRANSFORM_SIMPLIFY_SUBQUERY_H