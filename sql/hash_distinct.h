#ifndef HASH_DISTINCT_H
#define HASH_DISTINCT_H
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

		class HashDistinct : public SingleChildPhyOperator
		{
		public:
			HashDistinct() = delete;
			HashDistinct(PhyOperator_s& child);
		public:
			~HashDistinct();
			u32 set_distinct_columns(const Vector<Expression_s>& distinct_cols);
			static PhyOperator_s make_hash_distinct(PhyOperator_s& child);

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

	}
}

#endif	//HASH_DISTINCT_H