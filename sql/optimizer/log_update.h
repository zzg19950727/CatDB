#ifndef LOG_UPDATE_H
#define LOG_UPDATE_H
#include "logical_operator.h"
#include "type.h"

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
			virtual u32 type() const {return LogicalOperator::LOG_UPDATE;}
			static LogicalOperator_s make_update(LogicalOperator_s &child,
												BasicTableStmt_s &update_table,
												Vector<ExprStmt_s> &assign_exprs,
												Vector<ColumnStmt_s> &column_exprs,
												Vector<ExprStmt_s> &value_exprs,
												ColumnStmt_s &row_id);
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info);
		public:
			BasicTableStmt_s table_item;
			Vector<ExprStmt_s> assign_exprs;
			Vector<ColumnStmt_s> column_exprs;
			Vector<ExprStmt_s> value_exprs;
			ColumnStmt_s row_id;
		};
	}
}

#endif	//LOG_UPDATE_H