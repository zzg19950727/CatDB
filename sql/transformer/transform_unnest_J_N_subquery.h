#ifndef TRANSFORM_UNNEST_J_N_SUBQUERY_H
#define TRANSFORM_UNNEST_J_N_SUBQUERY_H
#include "transform_rule.h"
#include "print_helper.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(OpExprStmt);
        DECLARE(TableStmt);
        DECLARE(SelectStmt);
        DECLARE(SubQueryStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::ExprStmt_s;
        using Parser::OpExprStmt_s;
        using Parser::TableStmt_s;
        using Parser::SelectStmt_s;
        using Parser::SubQueryStmt_s;

        class TransformUnnestJNSubquery : public TransformRule {
        public:
            struct TransfomHelper {
                KV_STRING(
                    K(correlated_exprs),
                    K(pushdown_exprs),
                    K(table_item),
                    K(expr),
                    K(is_on_condition)
                );

                void reset()
                {
                    correlated_exprs.clear();
                    pushdown_exprs.clear();
                    expr.reset();
                    subquery_expr.reset();
                }

                Vector<ExprStmt_s> correlated_exprs;
                Vector<ExprStmt_s> pushdown_exprs;
                TableStmt_s table_item;
                OpExprStmt_s expr;
                SubQueryStmt_s subquery_expr;
                bool is_on_condition;
            };
            TransformUnnestJNSubquery();
            ~TransformUnnestJNSubquery();
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
        private:
            u32 transform_on_condition(DMLStmt_s &stmt, 
                                       TableStmt_s &table, 
                                       TransfomHelper &helper);

            u32 transform_conditions(DMLStmt_s &stmt,
                                     Vector<ExprStmt_s> &exprs, 
                                     TransfomHelper &helper);

            u32 transform_one_expr(DMLStmt_s &stmt,
                                   ExprStmt_s &expr, 
                                   TransfomHelper &helper, 
                                   bool &happened);

            u32 check_expr_need_transform(ExprStmt_s &expr, 
                                          TransfomHelper &helper, 
                                          bool &is_valid);

            u32 check_subquery_type(TransfomHelper &helper, 
                                    bool &is_valid);

            u32 check_correlated_exprs(TransfomHelper &helper, 
                                       bool &is_valid);

            u32 check_join_condition(TransfomHelper &helper, 
                                     TableStmt_s &table,
                                     bool &has_pullup_expr,
                                     bool &is_valid);

            u32 check_expr_can_pullup(TransfomHelper &helper,
                                      Vector<ExprStmt_s> &correlated_exprs,
                                      Vector<ExprStmt_s> &group_exprs,
                                      bool &is_valid);
            
            u32 check_expr_can_pullup(TransfomHelper &helper,
                                      ExprStmt_s &correlated_expr,
                                      Vector<ExprStmt_s> &group_exprs,
                                      bool &is_valid);

            u32 get_pushdown_exprs(TransfomHelper &helper,
                                    Vector<ExprStmt_s> &correlated_exprs);
            
            u32 get_pushdown_exprs(TransfomHelper &helper,
                                    ExprStmt_s &correlated_expr);

            u32 do_transform(DMLStmt_s &stmt, 
                             TransfomHelper &helper);

            u32 remove_correlated_exprs(SubQueryStmt_s &subquery_expr);

            u32 remove_correlated_exprs(TableStmt_s &table,
                                        SubQueryStmt_s &subquery_expr);

            u32 remove_correlated_exprs(Vector<ExprStmt_s> &exprs,
                                        SubQueryStmt_s &subquery_expr);

            u32 build_join_condition(TransfomHelper &helper, 
                                     ExprStmt_s &value_expr,
                                     JoinType &join_type);

            u32 check_hint_disable(SelectStmt_s& subquery, bool &is_disable);

            u32 generate_outline(SelectStmt_s& subquery);
        };
    }
}

#endif  //TRANSFORM_UNNEST_J_N_SUBQUERY_H