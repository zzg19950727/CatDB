#ifndef TRANSFORM_UNNEST_JA_SUBQUERY_H
#define TRANSFORM_UNNEST_JA_SUBQUERY_H
#include "transform_rule.h"
#include "print_helper.h"
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

        class TransformUnnestJASubquery : public TransformRule {
        public:
            struct TransfomHelper {

                KV_STRING(
                    K(outer_exprs),
                    K(inner_exprs),
                    K(table_item),
                    K(subquery),
                    K(is_on_condition)
                );

                void reset()
                {
                    outer_exprs.clear();
                    inner_exprs.clear();
                    subquery.reset();
                }
                Vector<ExprStmt_s> correlated_exprs;
                Vector<ExprStmt_s> outer_exprs;
                Vector<ExprStmt_s> inner_exprs;
                TableStmt_s table_item;
                SelectStmt_s subquery;
                bool is_on_condition;
            };
            TransformUnnestJASubquery();
            ~TransformUnnestJASubquery();
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
        private:
            u32 transform_on_condition(DMLStmt_s &stmt, 
                                       TableStmt_s &table, 
                                       TransfomHelper &helper);

            u32 transform_exprs(DMLStmt_s &stmt,
                                Vector<ExprStmt_s> &exprs, 
                                TransfomHelper &helper);

            u32 transform_one_expr(DMLStmt_s &stmt,
                                   ExprStmt_s &expr, 
                                   TransfomHelper &helper, 
                                   bool &happened);

            u32 check_expr_need_transform(ExprStmt_s &expr, 
                                          TransfomHelper &helper, 
                                          bool &is_valid);

            u32 check_correlated_exprs(TransfomHelper &helper, 
                                       SubQueryStmt_s &subquery_expr,
                                       bool &is_valid);

            u32 check_join_condition(TransfomHelper &helper, 
                                     TableStmt_s &table,
                                     SubQueryStmt_s &subquery_expr,
                                     Vector<ExprStmt_s> &correlated_exprs,
                                     bool &has_pullup_expr,
                                     bool &is_valid);

            u32 is_simple_correlated_expr(TransfomHelper &helper,
                                          ExprStmt_s &correlated_expr, 
                                          SubQueryStmt_s &subquery_expr,
                                          bool &is_simple);

            u32 do_transform(DMLStmt_s &stmt,
                             ExprStmt_s &expr,
                             TransfomHelper &helper);

            u32 remove_correlated_exprs(SubQueryStmt_s &subquery_expr);

            u32 remove_correlated_exprs(TableStmt_s &table,
                                        SubQueryStmt_s &subquery_expr);

            u32 remove_correlated_exprs(Vector<ExprStmt_s> &exprs,
                                        SubQueryStmt_s &subquery_expr);

            u32 try_add_case_when(Vector<ExprStmt_s> &aggr_exprs,
                                  Vector<ExprStmt_s> &column_exprs,
                                  ExprStmt_s &not_null_expr);

            u32 check_hint_disable(SelectStmt_s& subquery, bool &is_disable);

            u32 generate_outline(SelectStmt_s& subquery);
        };
    }
}

#endif  //TRANSFORM_UNNEST_JA_SUBQUERY_H