#ifndef LOG_SUBQUERY_EVALUATE_H
#define LOG_SUBQUERY_EVALUATE_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(SubQueryStmt);
    }
	namespace Optimizer {
		DECLARE(LogSubQueryEvaluate);
        using Parser::SubQueryStmt_s;

		//物理运算符接口
		class LogSubQueryEvaluate : public LogicalOperator
		{
		public:
			LogSubQueryEvaluate() {}
			virtual ~LogSubQueryEvaluate() {}
			static LogicalOperator_s make_subquery_evaluate(LogicalOperator_s &query_root);
			virtual u32 type() const override{return LOG_SUBQUERY_EVALUATE;}
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
            void add_subplan(SubQueryStmt_s& subquery_expr, LogicalOperator_s &subplan);
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
            Vector<SubQueryStmt_s> subqueries;
		};
	}
}

#endif	//LOG_SUBQUERY_EVALUATE_H