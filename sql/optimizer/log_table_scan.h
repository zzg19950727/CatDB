#ifndef LOG_TABLE_SCAN_H
#define LOG_TABLE_SCAN_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogTableScan);
		using Parser::BasicTableStmt_s;

		//物理运算符接口
		class LogTableScan : public LogicalOperator
		{
		public:
			LogTableScan(BasicTableStmt_s &table)
                :LogicalOperator(),
				table_item(table) {}
			virtual ~LogTableScan() {}
			static LogicalOperator_s make_table_scan(BasicTableStmt_s &table);
			virtual u32 type() const override{return LOG_TABLE_SCAN;}
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			BasicTableStmt_s table_item;
		};
	}
}

#endif	//LOG_TABLE_SCAN_H