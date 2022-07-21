#ifndef PHY_HASH_GROUP_H
#define PHY_HASH_GROUP_H
#include "phy_aggr_expression.h"
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

		class PhyHashGroup : public SingleChildPhyOperator, public AggregateExpCalculator
		{
		private:
			PhyHashGroup() = delete;
			PhyHashGroup(PhyOperator_s& child,
						 const Vector<Expression_s>& agg_funcs);
		public:
			~PhyHashGroup();
			static PhyOperator_s make_hash_group(PhyOperator_s& child, 
				const Vector<Expression_s>& group_exprs,
				const Vector<Expression_s>& agg_funcs,
				double distinct_rows);

			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
		private:
			u32 build_hash_table();

		private:
			HashTable_s hash_table;
			Row_s aggr_result_row;
			bool is_build_hash_table;
			double distinct_rows;
		};

	}
}

#endif	//PHY_HASH_GROUP_H