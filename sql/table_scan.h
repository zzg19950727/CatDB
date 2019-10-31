#ifndef TABLE_SCAN_H
#define TABLE_SCAN_H
#include "phy_operator.h"
#include "filter.h"
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
	}
	namespace Sql {
		using Storage::TableSpace_s;
		DECLARE(Filter);
		
		class TableScan : public PhyOperator
		{
		private:
			TableScan() = delete;
			TableScan(const TableSpace_s& table_space);
		public:
			~TableScan();
			static PhyOperator_s make_table_scan(const String&database,
				const String& table,
				const Common::RowDesc& desc);
			static PhyOperator_s make_table_scan(const String&database,
				const String& table,
				const Common::RowDesc& desc,
				const Filter_s& filter);
			u32 set_filter(const Filter_s& filter);
			Filter_s get_filter()const;
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
		private:
			TableSpace_s table_space;
			Filter_s filter;
		};
	}
}

#endif	//TABLE_SCAN_H
