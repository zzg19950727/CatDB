#ifndef UPDATE_H
#define UPDATE_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
	}
    namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Storage::TableSpace_s;
        using Common::Row_s;
		DECLARE(Filter);
        
		class Update : public PhyOperator
		{
		private:
			Update() = delete;
			Update(const TableSpace_s& table_space);
		public:
			~Update();
			static PhyOperator_s make_update(const String&database,
											const String& table,
                                            const Row_s& new_row);
            static PhyOperator_s make_update(const String&database,
											const String& table,
											const String& alias_table_name,
											const Row_s& new_row,
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
			u32 explain_operator(u32 depth, QueryResult* result)override;
		private:
			TableSpace_s table_space;
			Filter_s filter;
            Row_s new_row;
		};
	}
}

#endif	//UPDATE_H