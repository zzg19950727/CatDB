#ifndef SCHEMA_CHECKER_H
#define SCHEMA_CHECKER_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Sql {
		DECLARE(SchemaChecker);
		using Common::RowDesc;
		using Common::ColumnDesc;

		class SchemaChecker
		{
		public:
			static SchemaChecker_s make_schema_checker();
			u32 get_table_id(const String& database, const String& table_name);
			RowDesc get_row_desc(const String& database, const String& table_name);
			ColumnDesc get_column_desc(const String& database, const String& table_name, const String& column_name);
			bool have_column(const String& database, const String& table_name, const String& column_name);
		};
	}
}

#endif	//SCHEMA_CHECKER_H