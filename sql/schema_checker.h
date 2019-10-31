#ifndef SCHEMA_CHECKER_H
#define SCHEMA_CHECKER_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		DECLARE(SchemaChecker);
		using Common::RowDesc;
		using Common::ColumnDesc;
		using Common::Object_s;

		class SchemaChecker
		{
		public:
			static SchemaChecker_s make_schema_checker();
			u32 get_table_id(const String& database, const String& table_name);
			u32 get_row_desc(const String& database, const String& table_name, RowDesc& row_desc);
			u32 get_column_desc(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc);
			bool have_column(const String& database, const String& table_name, const String& column_name);
			u32 add_database(const String& database);
			u32 delete_database(const String& database);
			u32 add_table(const String& database, const String& table, const Vector<Pair<String, String>>& columns);
			u32 delete_table(const String& database, const String& table);
			u32 show_database(Vector<String>& databases);
			u32 show_tables(const String& database, Vector<String>& tables);
			u32 desc_table(const String& database, const String& table, Vector<Pair<String, String>>& columns);
			u32 get_database_id(const String& database, u32& id);
			u32 get_table_id(const String& database, const String& table, u32& id);

		private:
			u32 get_last_database_id(u32& id);
			u32 get_last_table_id(const String& database, u32& id);
			u32 get_column_id_from_result(u32& id, const Object_s& result);
			u32 get_name_from_result(Vector<String>& list, const Object_s& result);
			u32 get_columns_from_result(Vector<Pair<String, String>>& list, const Object_s& result);
			u32 get_row_desc_from_result(u32 table_id, RowDesc& row_desc, const Object_s& result);
			u32 execute_sys_sql(const String& sql, Object_s& result);
			u32 get_row_desc_system(const String& database, const String& table_name, RowDesc& row_desc);
			u32 get_column_desc_system(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc);
			bool have_column_system(const String& database, const String& table_name, const String& column_name);
		};
	}
}

#endif	//SCHEMA_CHECKER_H