#ifndef LOG_JOIN_H
#define LOG_JOIN_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogJoin);
		DECLARE(ConflictDetector);

		//物理运算符接口
		class LogJoin : public DoubleChildLogicalOperator
		{
		public:
			enum JoinAlgo {HASH_JOIN, NL_JOIN};
			LogJoin(const LogicalOperator_s& left, const LogicalOperator_s& right)
                :DoubleChildLogicalOperator(left, right),
				join_type(Inner),
				join_algo(NL_JOIN) {}
			virtual ~LogJoin() {}
			static LogicalOperator_s make_join(const LogicalOperator_s& left,
											const LogicalOperator_s& right,
											JoinType jon_type,
											JoinAlgo join_algo);
			virtual u32 type() const {return LogicalOperator::LOG_JOIN;}
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);
		public:
			Vector<ExprStmt_s> equal_join_condition;
			Vector<ExprStmt_s> other_join_condition;
			JoinType join_type;
			JoinAlgo join_algo;
		};
	}
}

#endif	//LOG_JOIN_H