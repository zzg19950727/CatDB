#ifndef OPT_EST_COST_H
#define OPT_EST_COST_H
#include "type.h"

namespace CatDB {
    namespace Parser {
		DECLARE(ExprStmt);
	}
    namespace Optimizer {
        using Parser::ExprStmt_s;

        class EstCostUtil {
        public:
            static double sample_formula(double size, double red, double sample_size);

            static double cost_hash_expr(double rows, const Vector<ExprStmt_s> &exprs);

            static double cost_cmp_expr(double rows, const Vector<ExprStmt_s> &exprs);

            static double cost_table_scan(double rows,
                                          double block_count,
                                          const Vector<ExprStmt_s> &access_exprs);
            
            static double cost_filters(double rows, 
                                       const Vector<ExprStmt_s> &filters);
            
            static double cost_hash_join(double left_rows,
                                         double right_rows,
                                         double output_rows,
                                         const Vector<ExprStmt_s> &equal_join_condition,
                                         const Vector<ExprStmt_s> &other_join_condition);
            
            static double cost_nestloop_join(double left_rows,
                                            double right_rows,
                                            double right_cost,
                                            const Vector<ExprStmt_s> &equal_join_condition,
                                            const Vector<ExprStmt_s> &other_join_condition);
            
            static double cost_delete(double table_rows, 
                                      double block_count, 
                                      double delete_rows, 
                                      bool is_delete_all);
        
            static double cost_distinct(double rows,
                                        const Vector<ExprStmt_s> &distinct_exprs);
        
            static double cost_expr_value(double rows);
        
            static double cost_hash_group_by(double rows, 
                                             double ndv, 
                                             const Vector<ExprStmt_s> &group_exprs,
                                             const Vector<ExprStmt_s> &aggr_exprs);
        
            static double cost_sort(double rows, const Vector<ExprStmt_s> &sort_exprs);
        
            static double cost_insert(double rows);
        
            static double cost_limit(double rows);
        
            static double cost_scalar_group(double rows, const Vector<ExprStmt_s> &aggr_exprs);
        
            static double cost_union_all(double left_rows, double right_rows);
            static double cost_hash_union(double left_rows, double right_rows);
            static double cost_hash_intersec(double left_rows, double right_rows);
            static double cost_hash_except(double left_rows, double right_rows);

            static double cost_update(double table_rows, 
                                      double block_count, 
                                      double update_rows);
        };
    }
}
#endif	//OPT_EST_COST_H
