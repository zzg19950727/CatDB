#ifndef LOG_WINDOW_FUNC_H
#define LOG_WINDOW_FUNC_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogWindowFunc);

		//物理运算符接口
		class LogWindowFunc : public SingleChildLogicalOperator
		{
		public:
			LogWindowFunc(LogicalOperator_s child)
                :SingleChildLogicalOperator(child) {}
			virtual ~LogWindowFunc() {}
			virtual u32 type() const override{return LOG_WINDOW_FUNC;}
			static LogicalOperator_s make_window_func(const LogicalOperator_s& child,
													  Vector<ExprStmt_s> &win_items,
													  Vector<ExprStmt_s> &partition_by_exprs);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			Vector<ExprStmt_s> partition_by_exprs;
			Vector<ExprStmt_s> win_items;
		};
	}
}

#endif	//LOG_WINDOW_FUNC_H