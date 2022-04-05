#ifndef TRANSFORM_FULL_OUTER_JOIN_H
#define TRANSFORM_FULL_OUTER_JOIN_H
#include "transform_rule.h"
#include "bit_set.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(TableStmt);
        DECLARE(SelectStmt);
        DECLARE(JoinedTableStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::ExprStmt_s;
        using Parser::TableStmt_s;
        using Parser::SelectStmt_s;
        using Parser::JoinedTableStmt_s;

        class TransformFullOuterJoin : public TransformRule {
        public:
            TransformFullOuterJoin();
            ~TransformFullOuterJoin();
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
            virtual bool need_rewrite(DMLStmt_s stmt) const override;
        private:
            u32 transform_one_table(DMLStmt_s &stmt, TableStmt_s &table);
            u32 check_table_need_transform(JoinedTableStmt_s &table, bool &is_valid);
            u32 check_join_condition(ExprStmt_s &expr, 
                                     BitSet &left_table_ids,
                                     BitSet &right_table_ids,
                                     bool &is_valid);
            u32 do_transform(DMLStmt_s &stmt, TableStmt_s &table);
            u32 output_null_for_anti(SelectStmt_s &view);
        };
    }
}

#endif  //TRANSFORM_FULL_OUTER_JOIN_H