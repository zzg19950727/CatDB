#ifndef PHY_LIMIT_H
#define PHY_LIMIT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		class PhyLimit : public SingleChildPhyOperator
		{
		private:
			PhyLimit() = delete;
			PhyLimit(PhyOperator_s& child, u32 size, u32 offset);
		public:
			static PhyOperator_s make_limit(PhyOperator_s& child, u32 size, u32 offset = 0);
			//物理算子必须提供的接口
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 inner_get_next_row(Row_s &row);
			u32 type() const override;
		private:
			u32 size;
			u32 offset;
			//当前读取状态
			u32 cur_offset;
		};
	}
}

#endif	//PHY_LIMIT_H
