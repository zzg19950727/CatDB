#ifndef STMT_COMPARE_H
#define STMT_COMPARE_H
#include "print_helper.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(SetStmt);
        DECLARE(SelectStmt);
        DECLARE(ExprStmt);
        DECLARE(TableStmt);
		DECLARE(ColumnStmt);
		DECLARE(SubQueryStmt);

        struct StmtCompareMap {
            enum CompareRelation {UNCOMPARE=0, EQUAL, LEFT_SUBSET, RIGHT_SUBSET};

            StmtCompareMap()
            :relation(UNCOMPARE)
            {}
            void reset();
            typedef std::pair<u32, u32> TableIdPair;
            Vector<TableIdPair> table_id_map;
            Vector<int> table_map;
            Vector<int> where_map;
            Vector<int> group_map;
            Vector<int> having_map;
            Vector<int> select_map;
            CompareRelation relation;
            KV_STRING(
                K(table_map),
                K(where_map),
                K(group_map),
                K(having_map),
                K(select_map),
                KV(relation, (u32)relation)
            );
        };

        struct ExprCompareCtx {
            ExprCompareCtx();
            virtual ~ExprCompareCtx();

            virtual u32 compare_column(ColumnStmt *lhs, 
                                       ColumnStmt *rhs, 
                                       bool &is_equal);

            virtual u32 compare_query_ref(SubQueryStmt *lhs, 
                                          SubQueryStmt *rhs, 
                                          bool &is_equal);
        };

        struct StmtCompareCtx : ExprCompareCtx {
            StmtCompareCtx(StmtCompareMap &map)
                :compare_map(map) {}

            virtual u32 compare_column(ColumnStmt *lhs, 
                                       ColumnStmt *rhs, 
                                       bool &is_equal) override;

            virtual u32 compare_query_ref(SubQueryStmt *lhs, 
                                          SubQueryStmt *rhs, 
                                          bool &is_equal) override;

            StmtCompareMap &compare_map;
        };

        class StmtCompare {
        public:
            static u32 compare_stmt(DMLStmt_s &lhs, 
                                    DMLStmt_s &rhs, 
                                    StmtCompareMap &cmp_map);

            static u32 compare_select_stmt(SelectStmt_s &lhs, 
                                           SelectStmt_s &rhs, 
                                           StmtCompareMap &cmp_map);
        private:
            static u32 compare_normal_select_stmt(SelectStmt_s &lhs, 
                                                SelectStmt_s &rhs, 
                                                StmtCompareMap &cmp_map);

            static u32 compare_set_stmt(SetStmt_s &lhs, 
                                        SetStmt_s &rhs, 
                                        StmtCompareMap &cmp_map);

            static u32 compare_table_items(Vector<TableStmt_s> &lhs, 
                                           Vector<TableStmt_s> &rhs,
                                           u32 &match_count,
                                           StmtCompareCtx &ctx);

            static u32 compare_table_item(TableStmt_s &lhs, 
                                          TableStmt_s &rhs,
                                          bool &is_equal,
                                          StmtCompareCtx &ctx); 

            static u32 compare_exprs(Vector<ExprStmt_s> &lhs, 
                                     Vector<ExprStmt_s> &rhs, 
                                     Vector<int> &exprs_map,
                                     u32 &match_count,
                                     StmtCompareCtx &ctx);
        };
    }
}

#endif  //STMT_COMPARE_H