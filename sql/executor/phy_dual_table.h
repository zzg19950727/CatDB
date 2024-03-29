#ifndef PHY_DUAL_TABLE_H
#define PHY_DUAL_TABLE_H
#include "phy_operator.h"

namespace CatDB {
	namespace Sql {
        using Common::Row_s;

		class PhyDualTable : public PhyOperator
		{
		private:
			PhyDualTable();
		public:
			~PhyDualTable();
			static PhyOperator_s make_dual_table();
			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
		private:
			bool is_start;
		};
	}
}

#endif	//PHY_DUAL_TABLE_H
