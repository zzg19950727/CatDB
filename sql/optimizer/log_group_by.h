#ifndef LOG_GROUP_BY_H
#define LOG_GROUP_BY_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogGroupBy);

		//物理运算符接口
		class LogGroupBy : public SingleChildLogicalOperator
		{
		public:
			LogGroupBy(const LogicalOperator_s& child)
                :SingleChildLogicalOperator(child),
				distinct_rows(1) {}
			virtual ~LogGroupBy() {}
			virtual u32 type() const override{return LOG_GROUP_BY;}
			static LogicalOperator_s make_group_by(const LogicalOperator_s& child,
													Vector<ExprStmt_s> &group_by_exprs, 
													Vector<ExprStmt_s> &agg_items);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			Vector<ExprStmt_s> group_by_exprs;
			Vector<ExprStmt_s> agg_items;
			double distinct_rows;
		};
	}
}

#endif	//LOG_GROUP_BY_H