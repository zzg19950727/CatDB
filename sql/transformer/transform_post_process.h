#ifndef TRANSFORM_POST_PROCESS_H
#define TRANSFORM_POST_PROCESS_H
#include "transform_rule.h"

namespace CatDB {
    namespace Transform {
        using Parser::DMLStmt_s;

        class TransformPostProcess : public TransformRule {
        public:
            TransformPostProcess();
            ~TransformPostProcess();
            virtual bool need_rewrite(DMLStmt_s stmt) const override;
            virtual u32 transform_one_stmt(DMLStmt_s &stmt)override;
            u32 finial_table_hint(DMLStmt_s &stmt, bool &happened);
        };
    }
}

#endif  //TRANSFORM_POST_PROCESS_H