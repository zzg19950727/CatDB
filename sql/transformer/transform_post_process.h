#ifndef TRANSFORM_POST_PROCESS_H
#define TRANSFORM_POST_PROCESS_H
#include "transform_rule.h"

namespace CatDB {
    namespace Parser {
        DECLARE(TableStmt);
        DECLARE(LeadingTable);
        DECLARE(LeadingHintStmt);
    }
    namespace Transform {
        using Parser::DMLStmt_s;
        using Parser::TableStmt_s;
        using Parser::LeadingTable_s;
        using Parser::LeadingHintStmt_s;

        class TransformPostProcess : public TransformRule {
        public:
            TransformPostProcess();
            ~TransformPostProcess();
            virtual bool need_rewrite(DMLStmt_s stmt) const override;
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
        private:
            u32 update_table_hint(DMLStmt_s &stmt, bool &happened);
			u32 update_join_hint(DMLStmt_s &stmt, bool &happened);
            u32 update_leading_table_hint(DMLStmt_s &stmt, bool &happened);
			u32 change_ordered_to_leading(DMLStmt_s &stmt, LeadingHintStmt_s &leading_hint);
			u32 change_table_item_to_leading(TableStmt_s &table_item, LeadingTable_s &leading_table);
            bool inner_update_leading_table_hint(DMLStmt_s &stmt, LeadingTable_s &leading_hint);
        };
    }
}

#endif  //TRANSFORM_POST_PROCESS_H