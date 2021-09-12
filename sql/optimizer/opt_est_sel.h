#ifndef OPT_EST_SEL_H
#define OPT_EST_SEL_H

class EstSelUtil {
public:
u32 calc_selectivity(EstInfo_s &est_info, Vector<ExprStmt_s> &exprs, double &selectivity);
u32 calc_selectivity(EstInfo_s &est_info, ExprStmt_t &expr, double &selectivity);
u32 calc_distinct_count(EstInfo_s &est_info, Vector<ExprStmt_s> &exprs, double &ndv);
u32 calc_distinct_count(EstInfo_s &est_info, ExprStmt_s &expr, double &ndv);
private:
u32 calc_eq_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
u32 calc_nq_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
u32 calc_bwt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
u32 calc_lt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);
u32 calc_gt_selectivity(EstInfo_s &est_info, ExprStmt_s &expr, double &selectivity);

};

#endif	//OPT_EST_SEL_H
