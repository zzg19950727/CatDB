#ifndef LOG_SORT_H
#define LOG_SORT_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(OrderStmt);
	}
	namespace Optimizer {
		DECLARE(LogSort);
		using Parser::OrderStmt_s;

		//物理运算符接口
		class LogSort : public SingleChildLogicalOperator
		{
		public:
			LogSort(LogicalOperator_s &child)
                :SingleChildLogicalOperator(child),
				top_n(0) {}
			virtual ~LogSort() {}
			virtual u32 type() const {return LogicalOperator::LOG_SORT;}
			static LogicalOperator_s make_sort(LogicalOperator_s &child,
												Vector<OrderStmt_s> &sort_keys,
												u32 top_n = 0);
			void set_top_n(u32 top_n)	{ this->top_n = top_n; }	
			u32 get_top_n() const { return top_n; }
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);					
		public:
			Vector<OrderStmt_s> sort_keys;
			u32 top_n;
		};
	}
}

#endif	//LOG_SORT_H