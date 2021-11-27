#ifndef SCHEMA_CHECKER_H
#define SCHEMA_CHECKER_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Parser {
		DECLARE(SchemaChecker);
		DECLARE(SchemaGuard);
		DECLARE(ColumnInfo);
		using Common::RowDesc;
		using Common::ColumnDesc;

		class SchemaChecker
		{
		private:
			SchemaChecker();
			~SchemaChecker();
		public:
			static SchemaChecker_s make_schema_checker();
			
			u32 get_row_desc(u32 ref_table_id, RowDesc& row_desc);

			u32 get_all_columns(u32 ref_table_id, Vector<ColumnInfo_s> &columns);
			
			u32 get_column_desc(u32 ref_table_id, const String& column_name, ColumnDesc& col_desc);
			
			u32 get_table_id(const String& database, const String& table, u32& id);

			u32 get_database_id(const String& database, u32 &id);

		private:
			SchemaGuard_s schema_guard;
		};
	}
}

#endif	//SCHEMA_CHECKER_H