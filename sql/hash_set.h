#ifndef HASH_SET_H
#define HASH_SET_H

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
		//右表构建hash表，左表探测
		class HashUnion : public DoubleChildPhyOperator
		{
		private:
			HashUnion() = delete;
			HashUnion(PhyOperator_s& first_child, PhyOperator_s& second_child);
		public:
			~HashUnion();
			static PhyOperator_s make_hash_union(PhyOperator_s& first_child, PhyOperator_s& second_child);

			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;

		private:
			Common::HashTable hash_table;
		};

		class HashIntersect : public DoubleChildPhyOperator
		{
		private:
			HashIntersect() = delete;
			HashIntersect(PhyOperator_s& first_child, PhyOperator_s& second_child);
		public:
			~HashIntersect();
			static PhyOperator_s make_hash_intersect(PhyOperator_s& first_child, PhyOperator_s& second_child);

			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;

		private:
			u32 build_hash_table();
			Common::HashTable hash_table;
			//用于交集去重
			Common::HashTable hash_distinct;
		};

		class HashExcept : public DoubleChildPhyOperator
		{
		private:
			HashExcept() = delete;
			HashExcept(PhyOperator_s& first_child, PhyOperator_s& second_child);
		public:
			~HashExcept();
			static PhyOperator_s make_hash_except(PhyOperator_s& first_child, PhyOperator_s& second_child);

			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;

		private:
			u32 build_hash_table();
			Common::HashTable hash_table;
		};

		class UnionAll : public DoubleChildPhyOperator
		{
		private:
			UnionAll() = delete;
			UnionAll(PhyOperator_s& first_child, PhyOperator_s& second_child);
		public:
			~UnionAll();
			static PhyOperator_s make_union_all(PhyOperator_s& first_child, PhyOperator_s& second_child);

			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
		};
	}
}

#endif	//HASH_SET_H