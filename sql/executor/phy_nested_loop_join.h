#ifndef PHY_NESTED_LOOP_JOIN_H
#define PHY_NESTED_LOOP_JOIN_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);
		DECLARE(PhyFilter);

		class PhyNestedLoopJoin : public JoinPhyOperator
		{
		private:
			PhyNestedLoopJoin() = delete;
			PhyNestedLoopJoin(const PhyOperator_s& left_child, const PhyOperator_s& right_child);
		public:
			~PhyNestedLoopJoin();
			static PhyOperator_s make_nested_loop_join(const PhyOperator_s& left_child,
				const PhyOperator_s& right_child,
				const Vector<Expression_s>& join_cond);
			
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
		private:
			
			u32 cache_right_table();
			u32 inner_join();
			u32 left_semi_join();
			u32 left_anti_join();
			u32 left_outer_join();
		private:
			Vector<Expression_s> join_condition;
			Vector<Row_s> right_cache;
			bool is_start;
			u32 right_pos;
			Row_s left_row;
		};
	}
}

#endif	//NESTED_LOOP_JOIN_H
