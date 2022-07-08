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
		DECLARE(HashTable);
		DECLARE(PhyHashJoin);

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
			//u32 build_hash_table_for_right_na_anti();
			u32 build_hash_table(bool &goto_next_state);
			u32 probe_hash_table(bool &goto_next_state);
			u32 probe_hash_table_right_outer(bool &goto_next_state);
			u32 probe_hash_table_right_semi(bool &goto_next_state);
			u32 probe_hash_table_right_anti(bool &goto_next_state);

			u32 visit_bucket_inner(bool &goto_next_state);
			u32 visit_bucket_left_outer(bool &goto_next_state);
			u32 visit_bucket_right_outer(bool &goto_next_state);
			u32 visit_bucket_left_semi(bool &goto_next_state);
			u32 visit_bucket_left_anti(bool &goto_next_state);

			u32 visti_hash_table(bool &goto_next_state);

			u32 output_rows_left_outer(bool &goto_next_state);
			u32 output_rows_left_semi(bool &goto_next_state);
			u32 output_rows_left_anti(bool &goto_next_state);
			u32 end_join(bool &goto_next_state);
		private:
			enum HashJoinState {
				BUILD_HASH_TABLE = 0,
				PROBE_HASH_TABLE,
				VISIT_BUCKET,
				VISIT_HASH_TABLE,
				OUTPUT_ROWS,
				END_JOIN,
				MAX_STATE
			};
			typedef u32 (PhyHashJoin::*HashJoinFuncType)(bool &goto_next_state);
			HashJoinFuncType hash_join_func[MaxJoinType][MAX_STATE];
		private:
			HashTable_s hash_table;
			Vector<Expression_s> hash_exprs;
			Vector<Expression_s> prob_exprs;
			Vector<Expression_s> other_conditions;
		private:
			//hash table state
			HashTable::RowIterator iter;
			Row_s last_probe_row;
			HashJoinState state;
		};
	}
}

#endif	//PHY_HASH_JOIN_H
