#ifndef PHY_HASH_DISTINCT_H
#define PHY_HASH_DISTINCT_H
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

		class PhyHashDistinct : public SingleChildPhyOperator
		{
		public:
			PhyHashDistinct() = delete;
			PhyHashDistinct(PhyOperator_s& child);
		public:
			~PhyHashDistinct();
			static PhyOperator_s make_hash_distinct(PhyOperator_s& child);

			//物理算子必须提供的接口
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 inner_get_next_row(Row_s &row);
			u32 type() const;
		private:
			Common::HashTable hash_table;
		};

	}
}

#endif	//PHY_HASH_DISTINCT_H