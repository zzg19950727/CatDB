#ifndef LOG_UPDATE_H
#define LOG_UPDATE_H
#include "logical_operator.h"
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogUpdate);

		//物理运算符接口
		class LogUpdate : public SingleChildLogicalOperator
		{
		public:
			LogUpdate(LogicalOperator_s &child)
                :SingleChildLogicalOperator(child) {}
			virtual ~LogUpdate() {}
			virtual u32 type() const override{return LOG_UPDATE;}
			static LogicalOperator_s make_update(LogicalOperator_s &child,
												BasicTableStmt_s &update_table,
												Vector<ExprStmt_s> &assign_exprs,
												Vector<ColumnStmt_s> &column_exprs,
												Vector<ExprStmt_s> &value_exprs,
												ColumnStmt_s &row_id);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			BasicTableStmt_s table_item;
			Vector<ExprStmt_s> assign_exprs;
			Vector<ColumnStmt_s> column_exprs;
			Vector<ExprStmt_s> value_exprs;
			ColumnStmt_s row_id;
			Common::RowDesc row_desc;
			Common::RowDesc update_desc;
		};
	}
}

#endif	//LOG_UPDATE_H