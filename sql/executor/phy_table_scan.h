﻿#ifndef PHY_TABLE_SCAN_H
#define PHY_TABLE_SCAN_H
#include "phy_operator.h"
#include "phy_filter.h"
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
	}
	namespace Sql {
		using Storage::TableSpace_s;
		
		class PhyTableScan : public PhyOperator
		{
		private:
			PhyTableScan() = delete;
			PhyTableScan(const TableSpace_s& table_space);
		public:
			~PhyTableScan();
			static PhyOperator_s make_table_scan(const String&database,
				const String& table,
				double sample_size = 1);
			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row(Row_s &row) override;
			u32 type() const override;
			void set_access_desc(const RowDesc &desc);
		private:
			TableSpace_s table_space;
			Row_s access_row;
		};
	}
}

#endif	//PHY_TABLE_SCAN_H
