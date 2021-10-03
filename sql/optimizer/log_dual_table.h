#ifndef LOG_DUAL_TABLE_H
#define LOG_DUAL_TABLE_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogDualTable);
		using Parser::BasicTableStmt_s;

		//物理运算符接口
		class LogDualTable : public LogicalOperator
		{
		public:
			LogDualTable(BasicTableStmt_s &table)
                :LogicalOperator(),
				table_item(table) {}
			virtual ~LogDualTable() {}
			static LogicalOperator_s make_dual_table(BasicTableStmt_s &table);
			virtual u32 type() const {return LogicalOperator::LOG_DUAL_TABLE;}
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);
			
		public:
			BasicTableStmt_s table_item;
		};
	}
}

#endif	//LOG_DUAL_TABLE_H