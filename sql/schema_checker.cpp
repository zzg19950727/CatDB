#include "schema_checker.h"
#include "query_result.h"
#include "sql_driver.h"
#include "object.h"
#include "error.h"
#include "plan.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;
using CatDB::SqlDriver;

SchemaChecker_s SchemaChecker::make_schema_checker()
{
	return SchemaChecker_s(new SchemaChecker());
}

u32 SchemaChecker::get_table_id(const String& database, const String& table_name)
{
	Hash<String> hash_func;
	return hash_func(database +"."+table_name);
}

u32 SchemaChecker::get_row_desc(const String& database, const String& table_name, RowDesc& row_desc)
{
	u32 table_id = get_table_id(database, table_name);
	if (database == "system") {
		return get_row_desc_system(database, table_name, row_desc);
	}
	else {
		String query = R"(select col.id from system.sys_databases as db,system.sys_tables as tb,system.sys_columns as col 
				where db.id = tb.db_id and tb.id = col.table_id 
				and db.name=")" + database + R"(" and tb.name=")" + table_name + R"(" order by col.id;)";
		Object_s result;
		u32 ret = execute_sys_sql(query, result);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query row desc failed");
		}
		else {
			ret = get_row_desc_from_result(table_id, row_desc, result);
		}
		return ret;
	}
}

u32 SchemaChecker::get_column_desc(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc)
{
	if (database == "system") {
		return get_column_desc_system(database, table_name, column_name, col_desc);
	}
	else {
		String query = R"(select col.id from system.sys_databases as db,system.sys_tables as tb,system.sys_columns as col 
				where db.id = tb.db_id and tb.id = col.table_id 
				and db.name=")" + database + R"(" and tb.name=")" + table_name + R"(" and col.name=")" + column_name + R"(";)";
		Object_s result;
		u32 ret = execute_sys_sql(query, result);
		u32 table_id = get_table_id(database, table_name);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query column failed");
		}
		else {
			u32 column_id;
			ret = get_column_id_from_result(column_id, result);
			if (ret == SUCCESS) {
				col_desc.set_tid_cid(table_id, column_id);
			}
		}
		return ret;
	}
}

bool SchemaChecker::have_column(const String& database, const String& table_name, const String& column_name)
{
	if (database == "system") {
		return have_column_system(database, table_name, column_name);
	}
	else {
		String query = R"(select col.id from system.sys_databases as db,system.sys_tables as tb,system.sys_columns as col 
				where db.id = tb.db_id and tb.id = col.table_id 
				and db.name=")" + database + R"(" and tb.name=")" + table_name + R"(" and col.name=")" + column_name + R"(";)";
		Object_s result;
		u32 ret = execute_sys_sql(query, result);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query column failed");
			return false;
		}
		else {
			QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
			return query_result->size() > 0;
		}
	}
}

u32 SchemaChecker::add_database(const String & database)
{
	u32 id;
	if (get_database_id(database, id) == SUCCESS) {
		return DATABASE_EXISTS;
	}
	
	u32 ret = get_last_database_id(id);
	if (ret != SUCCESS) {
		return ret;
	}
	++id;
	String query = R"(insert into system.sys_databases values()" + std::to_string(id) + R"(,")" + database + R"(");)";
	Object_s result;
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "add database failed");
	}
	else {
		ret = SUCCESS;
	}
	return ret;
}

u32 SchemaChecker::delete_database(const String & database)
{
	u32 db_id;
	if (get_database_id(database, db_id) != SUCCESS) {
		return DATABASE_NOT_EXISTS;
	}
	Vector<String> tables;
	u32 ret = show_tables(database, tables);
	if (ret != SUCCESS) {
		return SUCCESS;
	}
	for (u32 i = 0; i < tables.size(); ++i) {
		u32 table_id;
		if (get_table_id(database, tables[i], table_id) != SUCCESS) {
			return TABLE_NOT_EXISTS;
		}
		String query = R"(delete from system.sys_columns
				where table_id=)" + std::to_string(table_id) + R"(;)";
		Object_s result;
		u32 ret = execute_sys_sql(query, result);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "delete column failed");
			return ret;
		}
		query = R"(delete from system.sys_tables
				where id=)" + std::to_string(table_id)
			+ R"( and db_id=)" + std::to_string(db_id) + R"(;)";

		ret = execute_sys_sql(query, result);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "delete table failed");
			return ret;
		}
	}
	String query = R"(delete from system.sys_databases where id=)" + std::to_string(db_id) + R"(;)";
	Object_s result;
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "delete database failed");
	}
	else {
		ret = SUCCESS;
	}
	return ret;
}

u32 SchemaChecker::add_table(const String & database, const String & table, const Vector<Pair<String, String>>& columns)
{
	u32 db_id, table_id;
	if (get_database_id(database, db_id) != SUCCESS){
		return DATABASE_NOT_EXISTS;
	}
	else if (get_table_id(database, table, table_id) == SUCCESS) {
		return TABLE_EXISTS;
	}

	u32 ret = get_last_table_id(database, table_id);
	if (ret != SUCCESS) {
		return ret;
	}
	++table_id;
	String query = R"(insert into system.sys_tables values()" + std::to_string(table_id)
		+ R"(,")" + table + R"(",)" + std::to_string(db_id) + R"();)";
	Object_s result;
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "add table failed");
	}
	for (u32 i = 0; i < columns.size(); ++i) {
		query = R"(insert into system.sys_columns values()" + std::to_string(i)
			+ R"(,")" + columns[i].first + R"(",)" + std::to_string(table_id) 
			+ R"(,")" + columns[i].second + R"(");)";
		ret = execute_sys_sql(query, result);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "add column failed");
			break;
		}
	}
	return ret;
}

u32 SchemaChecker::delete_table(const String & database, const String & table)
{
	u32 db_id, table_id;
	if (get_database_id(database, db_id) != SUCCESS) {
		return DATABASE_NOT_EXISTS;
	}
	else if (get_table_id(database, table, table_id) != SUCCESS) {
		return TABLE_NOT_EXISTS;
	}

	String query = R"(delete from system.sys_columns
				where table_id=)" + std::to_string(table_id) + R"(;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "delete column failed");
	}
	query = R"(delete from system.sys_tables
				where id=)" + std::to_string(table_id)
		+ R"( and db_id=)" + std::to_string(db_id) + R"(;)";

	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "delete table failed");
	}
	return ret;
}

u32 SchemaChecker::show_database(Vector<String>& databases)
{
	String query = R"(select name from system.sys_databases;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "show databases failed");
	}
	return get_name_from_result(databases, result);
}

u32 SchemaChecker::show_tables(const String & database, Vector<String>& tables)
{
	String query = R"(select tb.name from system.sys_databases as db,system.sys_tables as tb
				where db.id = tb.db_id
				and db.name=")" + database + R"(";)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "show tables failed");
	}
	return get_name_from_result(tables, result);
}

u32 SchemaChecker::desc_table(const String & database, const String & table, Vector<Pair<String, String>>& columns)
{
	String query = R"(select col.name,col.type from system.sys_databases as db,system.sys_tables as tb,system.sys_columns as col 
				where db.id = tb.db_id and tb.id = col.table_id 
				and db.name=")" + database + R"(" and tb.name=")" + table + R"(" order by col.id;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query column failed");
		return false;
	}
	return get_columns_from_result(columns, result);
}

u32 SchemaChecker::get_database_id(const String & database, u32 & id)
{
	String query = R"(select id from system.sys_databases as db where db.name=")" + database + R"(";)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query database failed");
		return false;
	}
	else {
		return get_column_id_from_result(id, result);
	}
}

u32 SchemaChecker::get_table_id(const String & database, const String & table, u32 & id)
{
	String query = R"(select tb.id from system.sys_databases as db,system.sys_tables as tb
				where db.id = tb.db_id
				and db.name=")" + database + R"(" and tb.name=")" + table + R"(";)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query table failed");
		return false;
	}
	else {
		return get_column_id_from_result(id, result);
	}
}

u32 SchemaChecker::get_last_database_id(u32 & id)
{
	String query = R"(select id from system.sys_databases as db order by id desc limit 1;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query last database id failed");
	}
	else {
		ret = get_column_id_from_result(id, result);
		if (ret != SUCCESS) {
			id = 0;
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 SchemaChecker::get_last_table_id(const String & database, u32 & id)
{
	String query = R"(select tb.id from system.sys_tables as tb order by tb.id desc limit 1;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query last table id failed");
	}
	else {
		ret = get_column_id_from_result(id, result);
		if (ret != SUCCESS) {
			id = 0;
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 SchemaChecker::get_column_id_from_result(u32 & id, const Object_s & result)
{
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() != 1) {
		return SYSTEM_SCHEMA_ERROR;
	}
	else {
		Row_s row;
		u32 ret = query_result->get_row(0, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query column failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 1) {
				Log(LOG_ERR, "SchemaChecker", "query column failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell;
				row->get_cell(0, cell);
				if (cell->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query column failed, query result type error");
					return SYSTEM_SCHEMA_ERROR; 
				}
				else {
					Number* col_id = dynamic_cast<Number*>(cell.get());
					id = static_cast<u32>(col_id->value());
					return SUCCESS;
				}
			}
		}
	}
}

u32 SchemaChecker::get_name_from_result(Vector<String>& list, const Object_s & result)
{
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() == 0)
		return SUCCESS;
	for (u32 i = 0; i<query_result->size(); ++i) {
		Row_s row;
		u32 ret = query_result->get_row(i, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 1) {
				Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell;
				row->get_cell(0, cell);
				if (cell->get_type() != T_VARCHAR) {
					Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				else {
					Varchar* name = dynamic_cast<Varchar*>(cell.get());
					list.push_back(name->to_string());
				}
			}
		}
	}
	return SUCCESS;
}

u32 SchemaChecker::get_columns_from_result(Vector<Pair<String, String>>& list, const Object_s & result)
{
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() == 0)
		return SYSTEM_SCHEMA_ERROR;
	for (u32 i = 0; i<query_result->size(); ++i) {
		Row_s row;
		u32 ret = query_result->get_row(i, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 2) {
				Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell1;
				row->get_cell(0, cell1);
				if (cell1->get_type() != T_VARCHAR) {
					Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Object_s cell2;
				row->get_cell(1, cell2);
				if (cell2->get_type() != T_VARCHAR) {
					Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Varchar* name = dynamic_cast<Varchar*>(cell1.get());
				Varchar* type = dynamic_cast<Varchar*>(cell2.get());
				list.push_back(Pair<String,String>(name->to_string(), type->to_string()));
			}
		}
	}
	return SUCCESS;
}

u32 SchemaChecker::get_row_desc_from_result(u32 table_id, RowDesc & row_desc, const Object_s & result)
{
	ColumnDesc col_desc;
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() == 0)
		return SYSTEM_SCHEMA_ERROR;
	for(u32 i=0; i<query_result->size(); ++i){
		Row_s row;
		u32 ret = query_result->get_row(i, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query column failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 1) {
				Log(LOG_ERR, "SchemaChecker", "query column failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell;
				row->get_cell(0, cell);
				if (cell->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query column failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				else {
					Number* col_id = dynamic_cast<Number*>(cell.get());
					u32 id = static_cast<u32>(col_id->value());
					col_desc.set_tid_cid(table_id, id);
					row_desc.add_column_desc(col_desc);
				}
			}
		}
	}
	return SUCCESS;
}

u32 SchemaChecker::execute_sys_sql(const String & sql, Object_s& result)
{
	SqlDriver parser;
	int ret = parser.parse_sql(sql);

	if (parser.is_sys_error()) {
		Log(LOG_ERR, "SchemaChecker", "parse sys sql error:%s", parser.sys_error().c_str());
		return SYSTEM_SCHEMA_ERROR;
	}
	else if (parser.is_syntax_error()) {
		Log(LOG_ERR, "SchemaChecker", "parse sys sql error:%s", parser.syntax_error().c_str());
		return SYSTEM_SCHEMA_ERROR;
	}
	else {
		Plan_s plan = Plan::make_plan(parser.parse_result());
		u32 ret = plan->build_plan();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				Log(LOG_ERR, "SchemaChecker", "build sys sql plan error:%s", result->to_string().c_str());
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Log(LOG_ERR, "SchemaChecker", "build sys sql plan error");
				return SYSTEM_SCHEMA_ERROR;
			}
		}
		ret = plan->execute();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				Log(LOG_ERR, "SchemaChecker", "execute sys sql plan error:%s", result->to_string().c_str());
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Log(LOG_ERR, "SchemaChecker", "execute sys sql plan error");
				return SYSTEM_SCHEMA_ERROR;
			}
		}
		else {
			result = plan->get_result();
			return SUCCESS;
		}
	}
}

u32 SchemaChecker::get_row_desc_system(const String & database, const String & table_name, RowDesc& row_desc)
{
	u32 table_id = get_table_id(database, table_name);
	ColumnDesc col_desc;
	u32 ret = SUCCESS;
	if (table_name == "sys_databases") {
		col_desc.set_tid_cid(table_id, 0);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 1);
		row_desc.add_column_desc(col_desc);
	}
	else if (table_name == "sys_tables") {
		col_desc.set_tid_cid(table_id, 0);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 1);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 2);
		row_desc.add_column_desc(col_desc);
	}
	else if (table_name == "sys_columns") {
		col_desc.set_tid_cid(table_id, 0);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 1);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 2);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 3);
		row_desc.add_column_desc(col_desc);
	}
	else {
		ret = SYSTEM_SCHEMA_ERROR;
	}
	return ret;
}

u32 SchemaChecker::get_column_desc_system(const String & database, const String & table_name, const String & column_name, ColumnDesc& col_desc)
{
	u32 table_id = get_table_id(database, table_name);
	u32 ret = SUCCESS;
	if (table_name == "sys_databases") {
		if (column_name == "id") {
			col_desc.set_tid_cid(table_id, 0);
		}
		else if(column_name == "name") {
			col_desc.set_tid_cid(table_id, 1);
		}
		else {
			ret = SYSTEM_SCHEMA_ERROR;
		}
	}
	else if (table_name == "sys_tables") {
		if (column_name == "id") {
			col_desc.set_tid_cid(table_id, 0);
		}
		else if (column_name == "name") {
			col_desc.set_tid_cid(table_id, 1);
		}
		else if (column_name == "db_id") {
			col_desc.set_tid_cid(table_id, 2);
		}
		else {
			ret = SYSTEM_SCHEMA_ERROR;
		}
	}
	else if (table_name == "sys_columns") {
		if (column_name == "id") {
			col_desc.set_tid_cid(table_id, 0);
		}
		else if (column_name == "name") {
			col_desc.set_tid_cid(table_id, 1);
		}
		else if (column_name == "table_id") {
			col_desc.set_tid_cid(table_id, 2);
		}
		else if (column_name == "type") {
			col_desc.set_tid_cid(table_id, 3);
		}
		else {
			ret = SYSTEM_SCHEMA_ERROR;
		}
	}
	else {
		ret = SYSTEM_SCHEMA_ERROR;
	}
	return ret;
}

bool SchemaChecker::have_column_system(const String &, const String & table_name, const String & column_name)
{
	if (table_name == "sys_databases") {
		if (column_name == "id" 
			|| column_name == "name") {
			return true;
		}
		else {
			return false;
		}
	}
	else if (table_name == "sys_tables") {
		if (column_name == "id" 
			|| column_name == "name"
			|| column_name == "db_id") {
			return true;
		}
		else {
			return false;
		}
	}
	else if (table_name == "sys_columns") {
		if (column_name == "id"
			|| column_name == "name"
			|| column_name == "table_id"
			|| column_name == "type") {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
