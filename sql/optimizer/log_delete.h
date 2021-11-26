#ifndef LOG_DELETE_H
#define LOG_DELETE_H
#include "logical_operator.h"
#include "type.h"

namespace CatDB {
	namespace Optimizer {
		DECLARE(LogDelete);

		//物理运算符接口
		class LogDelete : public SingleChildLogicalOperator
		{
		public:
			LogDelete(LogicalOperator_s &child)
                :SingleChildLogicalOperator(child),
				is_delete_all(false) {}
			virtual ~LogDelete() {}
			virtual u32 type() const override {return LogicalOperator::LOG_DELETE;}
			static LogicalOperator_s make_delete(LogicalOperator_s &child,
												BasicTableStmt_s &delete_table,
												ColumnStmt_s& row_id,
												bool is_delete_all = false);
			virtual u32 est_row_count()override;
			virtual u32 est_cost()override;
			u32 allocate_expr_pre() override;
			void print_plan(u32 depth, Vector<PlanInfo> &plan_info) override;
		public:
			BasicTableStmt_s table_item;
			ColumnStmt_s row_id;
			bool is_delete_all;
		};
	}
}

#endif	//LOG_DELETE_H