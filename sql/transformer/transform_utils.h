#ifndef TRANSFORM_UTILS_H
#define TRANSFORM_UTILS_H
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(SelectStmt);
        DECLARE(TableStmt);
        DECLARE(ExprStmt);
        DECLARE(ViewTableStmt);
    }
    namespace Sql {
        DECLARE(QueryCtx);
    }
    namespace Transform {
        DECLARE(TransformCtx);
        using Sql::QueryCtx_s;
        using Parser::DMLStmt_s;
        using Parser::SelectStmt_s;
        using Parser::TableStmt_s;
        using Parser::ExprStmt_s;
        using Parser::ViewTableStmt_s;

        class TransformUtils {
        public:
            static u32 deep_copy_stmt(const SelectStmt_s &old_stmt, 
                                      SelectStmt_s &new_stmt, 
                                      QueryCtx_s &ctx,
                                      u32 flag = COPY_ON_WRITE | COPY_SHARE);
        
            static u32 create_view_with_table_item(const TableStmt_s &table,
                                                   SelectStmt_s &view,
                                                   TransformCtx_s &ctx);
        
            static u32 create_select_item_for_view_table(ViewTableStmt_s &view,
                                                         Vector<ExprStmt_s> &new_select_list,
                                                         Vector<ExprStmt_s> &new_column_list);
        
            static u32 create_column_for_view_table(ViewTableStmt_s &view,
                                                    Vector<ExprStmt_s> &new_column_list);

            static u32 create_set_stmt(const SelectStmt_s &left_query,
                                       const SelectStmt_s &right_query,
                                       SetOpType set_op,
                                       SelectStmt_s &set_stmt,
                                       TransformCtx_s &ctx);
        
            static u32 is_simple_stmt(SelectStmt_s &stmt, bool &is_simple);

            static u32 get_view_table_columns(DMLStmt_s &stmt, 
                                              ViewTableStmt_s &view_table, 
                                              Vector<ExprStmt_s> &columns, 
                                              Vector<ExprStmt_s> &select_exprs);
        
            static u32 build_joined_table(Vector<TableStmt_s> &table_items,
                                          TableStmt_s &joined_table);
        };
    }
}

#endif  //TRANSFORM_UTILS_H