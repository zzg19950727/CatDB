#ifndef LOG_DISTINCT_H
#define LOG_DISTINCT_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogDistinct);

		//物理运算符接口
		class LogDistinct : public SingleChildLogicalOperator
		{
		public:
			LogDistinct(LogicalOperator_s &child)
                :SingleChildLogicalOperator(child) {}
			virtual ~LogDistinct() {}
			virtual u32 type() const override {return LogicalOperator::LOG_DISTINCT;}
			static LogicalOperator_s make_distinct(LogicalOperator_s &child);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			void set_distinct_exprs(const Vector<ExprStmt_s> &exprs) { distinct_exprs = exprs; }
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			Vector<ExprStmt_s> distinct_exprs;
		};
	}
}

#endif	//LOG_DISTINCT_H