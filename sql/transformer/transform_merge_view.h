#ifndef TRANSFORM_MERGE_VIEW_H
#define TRANSFORM_MERGE_VIEW_H
#include "transform_rule.h"
#include "bit_set.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(TableStmt);
        DECLARE(SelectStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::ExprStmt_s;
        using Parser::TableStmt_s;
        using Parser::SelectStmt_s;

        struct TransfomHelper {
            TransfomHelper()
                :is_outer_right_view(false),
                upper_stmt_is_simple(false)
                {

                }

            KV_STRING(
                K(is_outer_right_view),
                K(upper_stmt_is_simple),
                K(row_id_expr)
            );

            void reset()
            {
                is_outer_right_view = false;
                upper_stmt_is_simple = false;
            }

            bool is_outer_right_view;
            bool upper_stmt_is_simple;
            ExprStmt_s row_id_expr;
        };

        class TransformMergeView : public TransformRule {
        public:
            TransformMergeView();
            ~TransformMergeView();
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
        private:
            u32 transform_one_table(DMLStmt_s &stmt, 
                                    TableStmt_s &table, 
                                    TransfomHelper &helper);

            u32 check_table_need_transform(DMLStmt_s &stmt, 
                                           TableStmt_s &table, 
                                           TransfomHelper &helper, 
                                           bool &is_valid);

            u32 find_not_null_row_id(DMLStmt_s &stmt,
                                     ExprStmt_s &row_id,
                                     bool &is_valid);

            u32 try_add_case_when(Vector<ExprStmt_s> &old_exprs,
                                  ExprStmt_s &not_null_expr,
                                  Vector<ExprStmt_s> &new_exprs);

            u32 do_transform(DMLStmt_s &stmt, 
                             TableStmt_s &table, 
                             TransfomHelper &helper);
        
            u32 check_hint_disable(SelectStmt_s& view, bool &is_disable);

            u32 generate_outline(SelectStmt_s& view);
        };
    }
}

#endif  //TRANSFORM_MERGE_VIEW_H