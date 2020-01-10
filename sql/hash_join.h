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
		using Common::HashTable;
		DECLARE(Expression);

		class HashJoin : public JoinPhyOperator
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
				u32 probe_table_id);
			u32 set_equal_condition(const Expression_s& equal_cond);
			Expression_s get_equal_condition()const;
			u32 set_join_condition(const Expression_s& join_cond);
			Expression_s get_join_condition()const;
			void set_probe_table_id(u32 id);
			u32 get_probe_table_id()const;
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
			u32 explain_operator(u32 depth, QueryResult* result)override;
		private:
			u32 init_hash_table();
			u32 build_hash_table();
			u32 join(Row_s &row);
			u32 semi_join(Row_s &row);
			//用于not exists的改写
			u32 anti_join(Row_s& row);
			u32 left_outer_join(Row_s &row);
			u32 right_outer_join(Row_s &row);
			u32 full_outer_join(Row_s &row);

		private:
			Common::HashTable hash_table;
			//join condition包含equal condition,equal condition用于hash join
			Expression_s equal_condition;
			Expression_s join_condition;
		private:
			//hash table state
			Queue<Row_s> probe_result;
			Row_s last_probe_row;
			u32 probe_table_id;
		};
		//支持join 列含有null值的anti join算法，目前只实现了单列join，用于支持not in改写
		class HashNullAwareAntiJoin : public JoinPhyOperator
		{
		private:
			HashNullAwareAntiJoin() = delete;
			HashNullAwareAntiJoin(const PhyOperator_s& left_child, const PhyOperator_s& right_child);
		public:
			~HashNullAwareAntiJoin();
			static PhyOperator_s make_hash_null_aware_anti_join(const PhyOperator_s& left_child,
				const PhyOperator_s& right_child,
				const Expression_s& anti_equal_cond,
				const Expression_s& equal_cond,
				const Expression_s& join_cond,
				u32 probe_table_id);
			u32 set_anti_equal_condition(const Expression_s& anti_equal_cond);
			Expression_s get_anti_equal_condition()const;
			u32 set_equal_condition(const Expression_s& equal_cond);
			Expression_s get_equal_condition()const;
			u32 set_join_condition(const Expression_s& join_cond);
			Expression_s get_join_condition()const;
			void set_probe_table_id(u32 id);
			u32 get_probe_table_id()const;
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
			u32 explain_operator(u32 depth, QueryResult* result)override;
		private:
			u32 init_hash_table();
			u32 build_hash_table();

		private:
			Common::HashTable hash_table;
			//join condition包含anti_equal condition,anti_equal condition用于hash join
			Expression_s anti_equal_condition;
			Expression_s equal_condition;
			Expression_s join_condition;
		private:
			HashTable::BucketIterator cur_bucket;
			u32 cur_bucket_pos;
			Expression_s anti_build_col, anti_probe_col;
			u32 probe_table_id;
		};
	}
}

#endif	//HASH_JOIN_H
