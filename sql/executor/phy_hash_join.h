#ifndef PHY_HASH_JOIN_H
#define PHY_HASH_JOIN_H
#include "phy_operator.h"
#include "hash_table.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);

		class PhyHashJoin : public JoinPhyOperator
		{
		private:
			PhyHashJoin() = delete;
			PhyHashJoin(const PhyOperator_s& left_child, const PhyOperator_s& right_child);
		public:
			~PhyHashJoin();
			static PhyOperator_s make_hash_join(const PhyOperator_s& left_child, 
				const PhyOperator_s& right_child,
				const Vector<Expression_s>& other_join_cond,
				const Vector<Expression_s> &hash_exprs,
				const Vector<Expression_s> &prob_exprs);
			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
		private:
			u32 build_hash_table();
			u32 build_hash_table_for_left_na_anti();
			u32 build_hash_table_for_right_na_anti();
			u32 join();
			u32 left_semi_join();
			u32 right_semi_join();
			u32 left_anti_join();
			u32 right_anti_join();
			u32 left_outer_join();
			u32 right_outer_join();
			u32 full_outer_join();

		private:
			HashTable hash_table;
			Vector<Expression_s> hash_exprs;
			Vector<Expression_s> prob_exprs;
		private:
			//hash table state
			Queue<Row_s> probe_result;
			Row_s last_probe_row;
			bool is_build_hash_table;
			bool has_null_in_hash_table;
			bool prepare_output_hash_table;
		};
	}
}

#endif	//PHY_HASH_JOIN_H
