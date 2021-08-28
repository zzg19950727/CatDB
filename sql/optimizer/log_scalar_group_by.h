#ifndef LOG_SCALAR_GROUP_BY_H
#define LOG_SCALAR_GROUP_BY_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogScalarGroupBy);

		//物理运算符接口
		class LogScalarGroupBy : public SingleChildLogicalOperator
		{
		public:
			LogScalarGroupBy(LogicalOperator_s child)
                :SingleChildLogicalOperator(child) {}
			virtual ~LogScalarGroupBy() {}
			virtual u32 type() const {return LogicalOperator::LOG_SCALAR_GROUP;}
			static LogicalOperator_s make_scalar_group_by(const LogicalOperator_s& child,
														Vector<ExprStmt_s> &agg_items);
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);
		public:
			Vector<ExprStmt_s> agg_items;
		};
	}
}

#endif	//LOG_SCALAR_GROUP_BY_H