#include "schema_checker.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

SchemaChecker_s SchemaChecker::make_schema_checker()
{
	return SchemaChecker_s(new SchemaChecker());
}

u32 SchemaChecker::get_table_id(const String& database, const String& table_name)
{
	Hash<String> hash_func;
	return hash_func(database +"."+table_name);
}

RowDesc SchemaChecker::get_row_desc(const String& database, const String& table_name)
{
	u32 table_id = get_table_id(database, table_name);
	if (table_name == "t1"){
		RowDesc row_desc(2);
		ColumnDesc col_desc;
		col_desc.set_tid_cid(table_id, 0);
		row_desc.set_column_desc(0, col_desc);
		col_desc.set_tid_cid(table_id, 1);
		row_desc.set_column_desc(1, col_desc);
		return row_desc;
	}else if (table_name == "t2"){
		RowDesc row_desc(3);
		ColumnDesc col_desc;
		col_desc.set_tid_cid(table_id, 0);
		row_desc.set_column_desc(0, col_desc);
		col_desc.set_tid_cid(table_id, 1);
		row_desc.set_column_desc(1, col_desc);
		col_desc.set_tid_cid(table_id, 2);
		row_desc.set_column_desc(2, col_desc);
		return row_desc;
	}else{
		RowDesc row_desc(4);
		ColumnDesc col_desc;
		col_desc.set_tid_cid(table_id, 0);
		row_desc.set_column_desc(0, col_desc);
		col_desc.set_tid_cid(table_id, 1);
		row_desc.set_column_desc(1, col_desc);
		col_desc.set_tid_cid(table_id, 2);
		row_desc.set_column_desc(2, col_desc);
		col_desc.set_tid_cid(table_id, 3);
		row_desc.set_column_desc(3, col_desc);
		return row_desc;
	}
}

ColumnDesc SchemaChecker::get_column_desc(const String& database, const String& table_name, const String& column_name)
{
	u32 table_id = get_table_id(database, table_name);
	ColumnDesc col_desc;
	if (column_name == "t1c1" || column_name == "t2c1" || column_name == "t3c1") {
		col_desc.set_tid_cid(table_id, 0);
	}
	else if (column_name == "t1c2" || column_name == "t2c2" || column_name == "t3c2") {
		col_desc.set_tid_cid(table_id, 1);
	}
	else if (column_name == "t2c3" || column_name == "t3c3") {
		col_desc.set_tid_cid(table_id, 2);
	}
	else if(column_name == "t3c4"){
		col_desc.set_tid_cid(table_id, 3);
	}
	return col_desc;
}

bool SchemaChecker::have_column(const String& database, const String& table_name, const String& column_name)
{
	if (table_name == "t1") {
		if (column_name == "t1c1" || column_name == "t1c2")
			return true;
		else
			return false;
	}
	else if (table_name == "t2") {
		if (column_name == "t2c1" || column_name == "t2c2" || column_name == "t2c3")
			return true;
		else
			return false;
	}
	else if (table_name == "t3") {
		if (column_name == "t3c1" || column_name == "t3c2" || column_name == "t3c3" || column_name == "t3c4")
			return true;
		else
			return false;
	}
	else {
		return false;
	}
}