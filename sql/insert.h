#ifndef INSERT_H
#define INSERT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
	}
	namespace Sql {
		using Storage::TableSpace_s;
		
		class Insert : public PhyOperator
		{
		private:
			Insert() = delete;
			Insert(const TableSpace_s& table_space);
		public:
			~Insert();
			static PhyOperator_s make_insert(const String&database,const String& table, const String& alias_table_name);
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
		};
	}
}

#endif	//INSERT_H
