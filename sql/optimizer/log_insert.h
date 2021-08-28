#ifndef LOG_INSERT_H
#define LOG_INSERT_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogInsert);

		//物理运算符接口
		class LogInsert : public SingleChildLogicalOperator
		{
		public:
			LogInsert(LogicalOperator_s &child)
                :SingleChildLogicalOperator(child) {}
			virtual ~LogInsert() {}
			virtual u32 type() const {return LogicalOperator::LOG_INSERT;}
			static LogicalOperator_s make_insert(LogicalOperator_s &child, BasicTableStmt_s &insert_table);
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);
		public:
			BasicTableStmt_s table_item;
		};
	}
}

#endif	//LOG_INSERT_H