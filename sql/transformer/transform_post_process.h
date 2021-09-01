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
            virtual u32 transform_one_stmt(DMLStmt_s &stmt);
        };
    }
}

#endif  //TRANSFORM_POST_PROCESS_H