#ifndef LOG_LIMIT_H
#define LOG_LIMIT_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogLimit);

		//物理运算符接口
		class LogLimit : public SingleChildLogicalOperator
		{
		public:
			LogLimit(LogicalOperator_s &child)
                :SingleChildLogicalOperator(child),
				 offset(0),
				 limit_value(0) {}
			virtual ~LogLimit() {}
			virtual u32 type() const override{return LOG_LIMIT;}
			static LogicalOperator_s make_limit(LogicalOperator_s &child, u32 offset, u32 limit_value);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			u32 offset;
			u32 limit_value;
		};
	}
}

#endif	//LOG_LIMIT_H