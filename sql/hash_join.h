#ifndef HASH_JOIN_H
#define HASH_JOIN_H
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

		class HashJoin : public DoubleChildPhyOperator
		{
		private:
			HashJoin() = delete;
			HashJoin(const PhyOperator_s& left_child, const PhyOperator_s& right_child);
		public:
			~HashJoin();
			static PhyOperator_s make_hash_join(const PhyOperator_s& left_child, 
				const PhyOperator_s& right_child,
				const Expression_s& equal_cond,
				const Expression_s& join_cond,
				u32 build_table_id);
			u32 set_equal_condition(const Expression_s& equal_cond);
			Expression_s get_equal_condition()const;
			u32 set_join_condition(const Expression_s& join_cond);
			Expression_s get_join_condition()const;
			u32 set_build_table_id(u32 table_id);
			u32 get_table_id()const;
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;

		private:
			u32 init_hash_table();
			u32 build_hash_table();

		private:
			Common::HashTable hash_table;
			//join condition包含equal condition,equal condition用于hash join
			Expression_s equal_condition;
			Expression_s join_condition;
			u32 build_table_id;
		private:
			//hash table state
			Queue<Row_s> probe_result;
			Row_s last_probe_row;
		};
	}
}

#endif	//HASH_JOIN_H
