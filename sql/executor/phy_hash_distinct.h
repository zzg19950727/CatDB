#ifndef PHY_HASH_DISTINCT_H
#define PHY_HASH_DISTINCT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);
		DECLARE(HashTable);

		class PhyHashDistinct : public SingleChildPhyOperator
		{
		public:
			PhyHashDistinct() = delete;
			PhyHashDistinct(PhyOperator_s& child);
		public:
			~PhyHashDistinct();
			static PhyOperator_s make_hash_distinct(PhyOperator_s& child);

			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
		private:
			HashTable_s hash_table;
		};

	}
}

#endif	//PHY_HASH_DISTINCT_H