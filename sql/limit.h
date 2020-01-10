#ifndef LIMIT_H
#define LIMIT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		class Limit : public SingleChildPhyOperator
		{
		private:
			Limit() = delete;
			Limit(PhyOperator_s& child, u32 size, u32 offset);
		public:
			static PhyOperator_s make_limit(PhyOperator_s& child, u32 size, u32 offset = 0);
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
			u32 explain_operator(u32 depth, QueryResult* result)override;
		private:
			u32 size;
			u32 offset;
			//当前读取状态
			u32 cur_offset;
		};
	}
}

#endif	//LIMIT_H
