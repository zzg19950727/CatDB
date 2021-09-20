#ifndef EXPR_UTILS_H
#define EXPR_UTILS_H
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(ExprStmt);
        DECLARE(ColumnStmt);

        class ExprUtils {
        private:
            ExprUtils() = default;
            ~ExprUtils() = default;
        public:
            template<typename T>
            static bool find_item(const Vector<T>& src, const T& item, u32 *index = nullptr)
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
            static u32 remote_items(Vector<T>& src, const Vector<T>& remove)
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
            static u32 get_column_exprs(Vector<ExprStmt_s> &exprs, u32 table_id, Vector<ColumnStmt_s> &columns);
            static u32 get_column_exprs(ExprStmt_s& expr, u32 table_id, Vector<ColumnStmt_s> &columns);
			static bool find_equal_expr(const Vector<ExprStmt_s> &exprs, const ExprStmt_s& expr, u32 *index = nullptr);
        };
    }
}

#endif  //EXPR_UTILS_H
