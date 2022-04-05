#ifndef PHY_HASH_SET_H
#define PHY_HASH_SET_H

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
		class PhyHashSetOp : public DoubleChildPhyOperator
		{
		private:
			PhyHashSetOp() = delete;
			PhyHashSetOp(PhyOperator_s& first_child,
					  PhyOperator_s& second_child,
					  SetOpType type);
		public:
			~PhyHashSetOp();
			static PhyOperator_s make_hash_setop(PhyOperator_s& first_child,
												 PhyOperator_s& second_child,
												 SetOpType type);

			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 get_next_row_union_all(Row_s &row);
			u32 get_next_row_union(Row_s &row);
			u32 get_next_row_intersect(Row_s &row);
			u32 get_next_row_except(Row_s &row);
			u32 build_hash_table();
			u32 type() const override;
		private:
			HashTable hash_table;
			bool init_hash_table;
			SetOpType set_type;
		};
	}
}

#endif	//PHY_HASH_SET_H