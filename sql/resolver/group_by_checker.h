#ifndef GROUP_BY_CHECKER_H
#define GROUP_BY_CHECKER_H
#include "stmt.h"
#include "type.h"
#include "log.h"

namespace CatDB {
	namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(SelectStmt);
        class GroupByChecker
        {
        public:
            GroupByChecker(SelectStmt_s &stmt);
            ~GroupByChecker();
            u32 check_exprs(Vector<ExprStmt_s> &exprs, bool &in_group_by);

        private:
            u32 check_expr(ExprStmt_s &expr, bool &in_group_by);
            bool check_expr_in_group_by(ExprStmt_s expr);

        private:
            SelectStmt_s &stmt;
        };
    }
}

#endif //GROUP_BY_CHECKER_H