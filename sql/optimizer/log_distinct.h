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
			virtual u32 type() const {return LogicalOperator::LOG_DISTINCT;}
			static LogicalOperator_s make_distinct(LogicalOperator_s &child);
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);
		public:
			
		};
	}
}

#endif	//LOG_DISTINCT_H