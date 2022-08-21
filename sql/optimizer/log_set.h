#ifndef LOG_SET_H
#define LOG_SET_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogSet);

		//物理运算符接口
		class LogSet : public DoubleChildLogicalOperator
		{
		public:
			LogSet(const LogicalOperator_s& left, const LogicalOperator_s& right)
                :DoubleChildLogicalOperator(left, right),
				distinct_rows(1) {}
			virtual ~LogSet() {}
			virtual u32 type() const override{return LOG_SET;}
			static LogicalOperator_s make_set_op(const LogicalOperator_s& left,
												 const LogicalOperator_s& right,
												 SetOpType set_type);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			SetOpType set_type;
			double distinct_rows;
		};
	}
}

#endif	//LOG_SET_H