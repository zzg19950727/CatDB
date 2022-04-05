#include "schema_checker.h"
#include "schema_guard.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;

SchemaChecker::SchemaChecker()
{
	schema_guard = SchemaGuard::make_schema_guard();
}

SchemaChecker::~SchemaChecker()
{
	
}

SchemaChecker_s SchemaChecker::make_schema_checker()
{
	static SchemaChecker_s checker(new SchemaChecker());
	return checker;
}

u32 SchemaChecker::get_row_desc(u32 ref_table_id, RowDesc& row_desc)
{
	u32 ret = SUCCESS;
	TableInfo_s table_info;
	CHECK(schema_guard->find_table_info(ref_table_id, table_info));
	Vector<ColumnInfo_s> columns;
	CHECK(get_all_columns(ref_table_id, columns));
	for (u32 i = 0; i < columns.size(); ++i) {
		ColumnInfo_s &column_info = columns[i];
		ColumnDesc col_desc;
		col_desc.set_cid(column_info->column_id);
		col_desc.set_data_type(column_info->column_type);
		row_desc.add_column_desc(col_desc);
	}
	return ret;
}

u32 SchemaChecker::get_all_columns(u32 ref_table_id, Vector<ColumnInfo_s> &columns)
{
	u32 ret = SUCCESS;
	TableInfo_s table_info;
	CHECK(schema_guard->find_table_info(ref_table_id, table_info));
	for (auto iter = table_info->id_column_infos.cbegin(); 
		 iter != table_info->id_column_infos.cend(); 
		 ++iter) {
		columns.push_back(iter->second);
	}
	std::sort(columns.begin(), 
			  columns.end(), 
			  [](const ColumnInfo_s& lhs, const ColumnInfo_s& rhs) { return lhs->column_id < rhs->column_id; } );
	return ret;
}

u32 SchemaChecker::get_column_desc(u32 ref_table_id, const String& column_name, ColumnDesc& col_desc)
{
	u32 ret = SUCCESS;
	ColumnInfo_s column_info;
	CHECK(schema_guard->find_column_info(ref_table_id, column_name, column_info));
	col_desc.set_cid(column_info->column_id);
	col_desc.set_data_type(column_info->column_type);
	return ret;
}

u32 SchemaChecker::get_table_id(const String & database, const String & table, u32 & id)
{
	u32 ret = SUCCESS;
	TableInfo_s table_info;
	CHECK(schema_guard->find_table_info(database, table, table_info));
	id = table_info->table_id;
	return ret;
}

u32 SchemaChecker::get_database_id(const String& database, u32 &id)
{
	u32 ret = SUCCESS;
	DatabaseInfo_s db_info;
	CHECK(schema_guard->find_database_info(database, db_info));
	id = db_info->db_id;
	return ret;
}