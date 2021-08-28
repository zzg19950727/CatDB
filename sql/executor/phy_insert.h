#ifndef PHY_INSERT_H
#define PHY_INSERT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
	}
	namespace Sql {
		using Storage::TableSpace_s;
		
		class PhyInsert : public SingleChildPhyOperator
		{
		private:
			PhyInsert() = delete;
			PhyInsert(PhyOperator_s &child, const TableSpace_s& table_space);
		public:
			~PhyInsert();
			static PhyOperator_s make_insert(PhyOperator_s &child,
											 const String&database,
											 const String& table);
			//物理算子必须提供的接口
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 inner_get_next_row(Row_s &row);
			u32 type() const;
		private:
			TableSpace_s table_space;
		};
	}
}

#endif	//PHY_INSERT_H
