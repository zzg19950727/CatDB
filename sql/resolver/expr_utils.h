#ifndef EXPR_UTILS_H
#define EXPR_UTILS_H
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(ColumnStmt);
        DECLARE(SubQueryStmt);

        class ExprUtils {
        private:
            ExprUtils() = default;
            ~ExprUtils() = default;
        public:
            template<typename T>
            static bool find_item(const Vector<T>& src, 
                                  const T& item, 
                                  u32 *index = nullptr)
            {
                for(u32 i = 0; i < src.size(); ++i) {
                    if (src[i] == item) {
                        if (index) {
                            *index = i;
                        }
                        return true;
                    }
                }
                return false;
            }
            
            template<typename T>
            static u32 remote_items(Vector<T>& src, 
                                    const Vector<T>& remove)
            {
                Vector<T> new_items;
                for(u32 i = 0; i < src.size(); ++i) {
                    if (!find_item(remove, src[i])) {
                        new_items.push_back(src[i]);
                    }
                }
                src = new_items;
                return 0;
            }

            static OperationType remove_op_any(OperationType type);
            static OperationType remove_op_all(OperationType type);

            static u32 get_column_exprs(Vector<ExprStmt_s> &exprs, 
                                        u32 table_id, 
                                        Vector<ColumnStmt_s> &columns);

            static u32 get_column_exprs(ExprStmt_s& expr, 
                                        u32 table_id, 
                                        Vector<ColumnStmt_s> &columns);

			static bool find_equal_expr(const Vector<ExprStmt_s> &exprs, 
                                        const ExprStmt_s& expr, 
                                        u32 *index = nullptr);

            static u32 replace_expr(const Vector<ExprStmt_s> &old_exprs, 
                                    const Vector<ExprStmt_s> &new_exprs, 
                                    ExprStmt_s& expr);
            
            static u32 replace_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                    const Vector<ExprStmt_s> &new_exprs, 
                                    Vector<ExprStmt_s> &exprs);
            
            static u32 deep_copy_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                       Vector<ExprStmt_s> &new_exprs,
                                       u32 flag = COPY_ON_WRITE);

            static u32 deep_copy_expr(const ExprStmt_s &old_expr, 
                                      ExprStmt_s &new_expr,
                                      u32 flag = COPY_ON_WRITE);
        
            static u32 make_row_id_expr(const String& table_name, 
                                        u32 table_id, 
                                        ColumnStmt_s &row_id);
        
            static u32 is_null_reject_expr(ExprStmt_s &expr, bool &reject_null);

            static u32 make_is_null_expr(ExprStmt_s &old_expr, ExprStmt_s &new_expr);

            static u32 make_or_expr(Vector<ExprStmt_s> &old_exprs, ExprStmt_s &new_expr);

            static u32 make_and_expr(Vector<ExprStmt_s> &old_exprs, ExprStmt_s &new_expr);

            static u32 make_null_expr(ExprStmt_s &null_expr);

            static u32 make_bool_expr(ExprStmt_s &bool_expr, bool value);

            static u32 make_int_expr(ExprStmt_s &int_expr, int value);

            static u32 extract_aggr_exprs(Vector<ExprStmt_s> &exprs, Vector<ExprStmt_s> &aggr_exprs);

            static u32 extract_aggr_exprs(ExprStmt_s &expr, Vector<ExprStmt_s> &aggr_exprs);

            static u32 extract_subquery_exprs(Vector<ExprStmt_s> &exprs, 
                                              Vector<SubQueryStmt_s> &subquery_exprs,
                                              bool ignore_aggr = false);

            static u32 extract_subquery_exprs(ExprStmt_s &expr, 
                                              Vector<SubQueryStmt_s> &subquery_exprs,
                                              bool ignore_aggr = false);
        
            static u32 extract_subquery_exprs(Vector<ExprStmt_s> &exprs, 
                                              Vector<ExprStmt_s> &subquery_exprs,
                                              Vector<ExprStmt_s> &none_subquery_exprs);
        
            static u32 is_calculable_expr(ExprStmt_s &expr, bool &is_calculable);

            static u32 expr_intersect(const Vector<ExprStmt_s> &lhs,
                                      const Vector<ExprStmt_s> &rhs,
                                      Vector<ExprStmt_s> &res);
        
            static u32 remove_equal_exprs(Vector<ExprStmt_s> &src,
                                          const Vector<ExprStmt_s> &dst);
        };
    }
}

#endif  //EXPR_UTILS_H
