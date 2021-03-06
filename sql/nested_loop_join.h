#ifndef NESTED_LOOP_JOIN_H
#define NESTED_LOOP_JOIN_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);
		DECLARE(Filter);

		class NestedLoopJoin : public JoinPhyOperator
		{
		private:
			NestedLoopJoin() = delete;
			NestedLoopJoin(const PhyOperator_s& left_child, const PhyOperator_s& right_child);
		public:
			~NestedLoopJoin();
			static PhyOperator_s make_nested_loop_join(const PhyOperator_s& left_child,
				const PhyOperator_s& right_child,
				const Expression_s& join_cond);
			u32 set_join_condition(const Expression_s& join_cond);
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
			u32 explain_operator(u32 depth, QueryResult* result)override;
		private:
			//缓存右表的执行结果
			u32 cache_right_table();
			u32 join(Row_s &row);
			u32 semi_join(Row_s &row);
			u32 anti_join(Row_s &row);
			u32 left_outer_join(Row_s &row);
			u32 right_outer_join(Row_s &row);
			u32 full_outer_join(Row_s &row);
		private:
			Vector<Row_s> right_tables;
			u32 right_pos;
			Filter_s join_condition;
			Row_s left_row;
		};
	}
}

#endif	//NESTED_LOOP_JOIN_H
