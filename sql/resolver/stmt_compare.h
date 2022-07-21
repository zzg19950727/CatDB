#ifndef STMT_COMPARE_H
#define STMT_COMPARE_H
#include "error.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
        DECLARE(DMLStmt);
        DECLARE(SelectStmt);
        DECLARE(ExprStmt);
		DECLARE(ColumnStmt);
		DECLARE(SubQueryStmt);

        struct StmtCompareMap {
            enum CompareRelation {UNCOMPARE=0, EQUAL, LEFT_SUBSET, RIGHT_SUBSET};

            StmtCompareMap()
            :is_table_equal(false),
            is_where_equal(false),
            is_group_equal(false),
            is_having_equal(false),
            is_select_equal(false),
            relation(UNCOMPARE)
            {}

            Vector<u32> table_map;
            Vector<u32> where_map;
            Vector<u32> group_map;
            Vector<u32> having_map;
            Vector<u32> select_map;
            bool is_table_equal;
            bool is_where_equal;
            bool is_group_equal;
            bool is_having_equal;
            bool is_select_equal;
            CompareRelation relation;
        };

        struct ExprCompareCxt {
            ExprCompareCxt();
            virtual ~ExprCompareCxt(){}
            u32 compare_expr(ExprStmt_s &lhs, ExprStmt_s &rhs, bool &is_equal);
            virtual u32 compare_column(ColumnStmt_s lhs, ColumnStmt_s rhs, bool &is_equal);
            virtual u32 compare_query_ref(SubQueryStmt_s lhs, SubQueryStmt_s rhs, bool &is_equal);
        };

        struct StmtCompareCtx : public ExprCompareCtx {
            StmtCompareCtx(const StmtCompareMap &map)
                :compare_map(map) {}

            virtual u32 compare_column(ColumnStmt_s lhs, ColumnStmt_s rhs, bool &is_equal);
            virtual u32 compare_query_ref(SubQueryStmt_s lhs, SubQueryStmt_s rhs, bool &is_equal);
            const StmtCompareMap &compare_map;
        };

        class StmtCompare {
        public:
            u32 compare_stmt(DMLStmt_s &lhs, DMLStmt_s &rhs, StmtCompareMap &cmp_map);
        private:
            u32 compare_select_stmt(SelectStmt_s &lhs, SelectStmt_s &rhs, StmtCompareMap &cmp_map);
            u32 compare_table_items(Vector<TableItem_s> &lhs, Vector<TableItem_s> &rhs, StmtCompareMap &cmp_map);
            u32 compare_exprs(Vector<ExprStmt_s> &lhs, Vector<ExprStmt_s> &rhs, StmtCompareMap &cmp_map);
        };
    }
}

#endif  //STMT_COMPARE_H