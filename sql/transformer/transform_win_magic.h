#ifndef TRANSFORM_WIN_MAGIC_H
#define TRANSFORM_WIN_MAGIC_H
#include "transform_rule.h"
#include "print_helper.h"
#include "stmt_compare.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(SubQueryStmt);
        DECLARE(TableStmt);
        DECLARE(SelectStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::ExprStmt_s;
        using Parser::SubQueryStmt_s;
        using Parser::TableStmt_s;
        using Parser::SelectStmt_s;

        class TransformWinMagic : public TransformRule {
        public:
            struct TransfomHelper {

                KV_STRING(
                    K(partition_by_exprs),
                    K(subquery_expr),
                    K(cmp_map)
                );

                void reset()
                {
                    partition_by_exprs.clear();
                    subquery_expr.reset();
                    cmp_map.reset();
                }
                Vector<ExprStmt_s> partition_by_exprs;
                SubQueryStmt_s subquery_expr;
                Parser::StmtCompareMap cmp_map;
            };
            TransformWinMagic();
            ~TransformWinMagic();
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
        private:

            u32 get_transform_expr(DMLStmt_s &stmt,
                                   Vector<ExprStmt_s> &exprs, 
                                   TransfomHelper &helper,
                                   bool &is_valid);

            u32 get_transform_expr(DMLStmt_s &stmt,
                                   ExprStmt_s &expr, 
                                   TransfomHelper &helper, 
                                   bool &is_valid);

            u32 check_expr_need_transform(DMLStmt_s &stmt,
                                          ExprStmt_s &expr, 
                                          TransfomHelper &helper, 
                                          bool &is_valid);

            u32 is_simple_expr(TransfomHelper &helper,
                                DMLStmt_s &stmt,
                                SubQueryStmt_s &subquery_expr,
                                ExprStmt_s &expr,
                                bool &is_simple);

            u32 find_same_condition(TransfomHelper &helper,
                                    DMLStmt_s &stmt,
                                    ExprStmt_s &inner_expr,
                                    ExprStmt_s &outer_expr,
                                    bool &find);

            u32 do_transform(DMLStmt_s &stmt,
                             TransfomHelper &helper);

            u32 transform_aggr_expr_to_win_expr(TransfomHelper &helper, ExprStmt_s &expr);

            u32 transform_one_aggr_expr_to_win_expr(TransfomHelper &helper, 
                                                    ExprStmt_s &src_expr,
                                                    ExprStmt_s &dst_expr);

            u32 check_hint_disable(DMLStmt_s &stmt, SelectStmt_s& subquery, bool &is_disable);

            u32 generate_outline(DMLStmt_s &stmt, SelectStmt_s& subquery);
        };
    }
}

#endif  //TRANSFORM_WIN_MAGIC_H