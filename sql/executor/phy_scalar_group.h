#ifndef PHY_SCALAR_GROUP_H
#define PHY_SCALAR_GROUP_H

#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Row_s;
		using Common::Object_s;
		DECLARE(Filter);
		DECLARE(Expression);

		class PhyScalarGroup : public SingleChildPhyOperator
		{
		public:
			PhyScalarGroup() = delete;
			PhyScalarGroup(PhyOperator_s& child,
							Vector<Expression_s> &aggr_items);
		public:
			~PhyScalarGroup();
			static PhyOperator_s make_scalar_group(PhyOperator_s& child,
												   Vector<Expression_s> &aggr_items);

			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row(Row_s &row) override;
			u32 type() const override;
		private:
			void reset_agg_func();
			u32 add_row_to_agg_func(const Row_s& row);

			Vector<Expression_s> agg_funcs;
			bool is_start;
		};

	}
}

#endif	//PHY_SCALAR_GROUP_H