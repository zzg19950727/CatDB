#ifndef OPT_EST_SEL_H
#define OPT_EST_SEL_H
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExprStmt);
    }
    namespace Optimizer {
        DECLARE(EstInfo);
        using Parser::ExprStmt_s;
        class EstSelUtil {
        public:
            static u32 calc_selectivity(EstInfo_s &est_info, Vector<ExprStmt_s> &exprs, double &selectivity);
            static u32 calc_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
            static u32 calc_distinct_count(EstInfo_s &est_info, Vector<ExprStmt_s> &exprs, double &ndv);
            static u32 calc_distinct_count(EstInfo_s &est_info, ExprStmt_s &expr, double &ndv);
        private:
            static u32 calc_eq_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
            static u32 calc_bwt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
            static u32 calc_lt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
            static u32 calc_gt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
            static u32 calc_is_null_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
        };
    }
}

#endif	//OPT_EST_SEL_H
