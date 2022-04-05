#ifndef LOG_VIEW_H
#define LOG_VIEW_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogView);
		//物理运算符接口
		class LogView : public SingleChildLogicalOperator
		{
		public:
			LogView(LogicalOperator_s &child, ViewTableStmt_s &table)
                :SingleChildLogicalOperator(child),
				table_item(table) {}
			virtual ~LogView() {}
			static LogicalOperator_s make_view(ViewTableStmt_s &table, LogicalOperator_s &query_root);
			virtual u32 type() const override{return LOG_VIEW;}
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			ViewTableStmt_s table_item;
		};
	}
}

#endif	//LOG_VIEW_H