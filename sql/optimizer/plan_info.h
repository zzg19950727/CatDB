#ifndef PLAN_INFO_H
#define PLAN_INFO_H
#include "type.h"

namespace CatDB {
    namespace Optimizer {
        struct PlanInfo {
            u32 depth;
            String id;
            String op;
            String name;
            String rows;
            String cost;
            String expr_info;
            
            static void formalize_plan_info(Vector<PlanInfo> &plan_infos);
            static void print_plan_info(Vector<PlanInfo> &plan_infos, String &explain_info);
        };
    }
}

#endif //PLAN_INFO_H