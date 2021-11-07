#ifndef TRANSFORM_PRE_PROCESS_H
#define TRANSFORM_PRE_PROCESS_H
#include "transform_rule.h"

namespace CatDB {
    namespace Transform {
        using Parser::DMLStmt_s;

        class TransformPreProcess : public TransformRule {
        public:
            TransformPreProcess();
            ~TransformPreProcess();
            virtual bool need_rewrite(DMLStmt_s stmt) const override;
            virtual u32 transform_one_stmt(DMLStmt_s &stmt) override;
        private:
            
        };
    }
}

#endif  //TRANSFORM_PRE_PROCESS_H