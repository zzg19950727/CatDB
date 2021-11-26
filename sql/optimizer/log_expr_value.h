#ifndef LOG_EXPR_VALUE_H
#define LOG_EXPR_VALUE_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogExprValue);

		//物理运算符接口
		class LogExprValue : public LogicalOperator
		{
		public:
			LogExprValue()
                :LogicalOperator() {}
			virtual ~LogExprValue() {}
			virtual u32 type() const override{return LogicalOperator::LOG_EXPR_VALUE;}
			static LogicalOperator_s make_expr_value(Vector<Vector<ExprStmt_s>> &values);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			Vector<Vector<ExprStmt_s>> values;
		};
	}
}

#endif	//LOG_EXPR_VALUE_H