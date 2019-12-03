#include "schema_checker.h"
#include "query_result.h"
#include "table_space.h"
#include "sql_driver.h"
#include "object.h"
#include "error.h"
#include "plan.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;
using namespace CatDB::Storage;
using CatDB::SqlDriver;

SchemaChecker_s SchemaChecker::make_schema_checker()
{
	static SchemaChecker_s checker(new SchemaChecker());
	return checker;
}

u32 SchemaChecker::get_table_id(const String& database, const String& table_name)
{
	Hash<String> hash_func;
	return hash_func(database +"."+table_name);
}

u32 SchemaChecker::get_row_desc(const String& database, const String& table_name, RowDesc& row_desc)
{
	u32 ret = SUCCESS;
	u32 table_id = get_table_id(database, table_name);
	
	if (database == "system") {
		ret = get_row_desc_system(database, table_name, row_desc);
		if (ret == SUCCESS) {
			return ret;
		}
	}
	String query = R"(select col.id, col.type from system.sys_databases as db,system.sys_tables as tb,system.sys_columns as col 
				where db.id = tb.db_id and tb.id = col.table_id 
				and db.name=")" + database + R"(" and tb.name=")" + table_name + R"(" order by col.id;)";
	Object_s result;
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query row desc failed");
	}
	else {
		ret = get_row_desc_from_result(table_id, row_desc, result);
	}
	return ret;
}

u32 SchemaChecker::get_column_desc(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc)
{
	u32 ret = SUCCESS;
	if (database == "system") {
		ret = get_column_desc_system(database, table_name, column_name, col_desc);
		if (ret == SUCCESS) {
			return ret;
		}
	}
	String query = R"(select col.id,col.table_id,col.type from system.sys_databases as db,system.sys_tables as tb,system.sys_columns as col 
				where db.id = tb.db_id and tb.id = col.table_id 
				and db.name=")" + database + R"(" and tb.name=")" + table_name + R"(" and col.name=")" + column_name + R"(";)";
	Object_s result;
	ret = execute_sys_sql(query, result);
	u32 table_id = get_table_id(database, table_name);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query column failed");
	}
	else {
		ret = get_columns_desc_from_result(col_desc, result);
		if (ret == SUCCESS) {
			col_desc.set_tid(table_id);
		}
	}
	return ret;
}

bool SchemaChecker::have_column(const String& database, const String& table_name, const String& column_name)
{
	if (database == "system") {
		bool ret = true;
		ret = have_column_system(database, table_name, column_name);
		if (ret) {
			return ret;
		}
	}
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
		return get_id_from_result(id, result);
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
		return get_id_from_result(id, result);
	}
}

u32 SchemaChecker::analyze_table(const String & database, const String & table)
{
	if (database == "system") {
		Log(LOG_WARN, "AnalyzeStatis", "skip analyze system database");
		return CAN_NOT_ANALYZE_SYSTEM;
	}
	else if (database == "*") {
		Vector<String> databases;
		u32 ret = show_database(databases);
		if (ret != SUCCESS) {
			return ret;
		}
		for (u32 i = 0; i < databases.size(); ++i) {
			if (databases[i] == "system") {
				continue;
			}
			Vector<String> tables;
			u32 ret = show_tables(databases[i], tables);
			if (ret != SUCCESS) {
				continue;
			}
			for (u32 j = 0; j < tables.size(); ++j) {
				ret = analyze_table_system(databases[i], tables[j]);
			}
		}
		return ret;
	}
	else if (table == "*") {
		Vector<String> tables;
		u32 ret = show_tables(database, tables);
		if (ret != SUCCESS) {
			return ret;
		}
		for (u32 i = 0; i < tables.size(); ++i) {
			ret = analyze_table_system(database, tables[i]);
		}
		return ret;
	}
	else {
		return analyze_table_system(database, table);
	}
}

u32 SchemaChecker::statis_table_row_count(const String & database, const String & table)
{
	String query = R"(select statis.row_count
				from system.sys_databases as db,system.sys_tables as tb,system.table_statis as statis 
				where db.id = tb.db_id and tb.id = statis.tid 
				and db.name=")" + database + R"(" and tb.name=")" + table + 
				R"(" order by statis.analyze_time desc limit 1;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query table row count failed");
		return 0;
	}
	double value = 10000;
	get_value_from_result(value, result);
	return static_cast<u32>(value);
}

u32 SchemaChecker::statis_column_ndv(const String & database, const String & table_name, const String & column_name)
{
	String query = R"(select statis.ndv
				from system.sys_databases as db,system.sys_tables as tb,
				system.sys_columns as col,system.column_statis as statis
				where db.id = tb.db_id and tb.id = col.table_id and tb.id = statis.tid and col.id = statis.cid
				and db.name=")" + database + R"(" and tb.name=")" + table_name + 
				R"(" and col.name=")" + column_name + R"(" order by statis.analyze_time desc limit 1;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query column ndv failed");
		return 0;
	}
	double value = 10000;
	get_value_from_result(value, result);
	return static_cast<u32>(value);
}

u32 SchemaChecker::statis_column_max_value(const String & database, const String & table_name, const String & column_name)
{
	String query = R"(select statis.max_value
				from system.sys_databases as db,system.sys_tables as tb,
				system.sys_columns as col,system.column_statis as statis
				where db.id = tb.db_id and tb.id = col.table_id and tb.id = statis.tid and col.id = statis.cid
				and db.name=")" + database + R"(" and tb.name=")" + table_name +
		R"(" and col.name=")" + column_name + R"(" order by statis.analyze_time desc limit 1;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query column ndv failed");
		return 0;
	}
	double value = 10000;
	get_value_from_result(value, result);
	return static_cast<u32>(value);
}

u32 SchemaChecker::statis_column_min_value(const String & database, const String & table_name, const String & column_name)
{
	String query = R"(select statis.min_value
				from system.sys_databases as db,system.sys_tables as tb,
				system.sys_columns as col,system.column_statis as statis
				where db.id = tb.db_id and tb.id = col.table_id and tb.id = statis.tid and col.id = statis.cid
				and db.name=")" + database + R"(" and tb.name=")" + table_name +
		R"(" and col.name=")" + column_name + R"(" order by statis.analyze_time desc limit 1;)";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "SchemaChecker", "query column ndv failed");
		return 0;
	}
	double value = 10000;
	get_value_from_result(value, result);
	return static_cast<u32>(value);
}

u32 SchemaChecker::init_db()
{
	String query = "create database system;";
	Object_s result;
	u32 ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		return SYSTEM_SCHEMA_ERROR;
	}
	query = "create database system;";
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		return SYSTEM_SCHEMA_ERROR;
	}
	return SUCCESS;
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
		ret = get_id_from_result(id, result);
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
		ret = get_id_from_result(id, result);
		if (ret != SUCCESS) {
			id = 0;
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 SchemaChecker::get_id_from_result(u32 & id, const Object_s & result)
{
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() != 1) {
		return SYSTEM_SCHEMA_ERROR;
	}
	else {
		Row_s row;
		u32 ret = query_result->get_row(0, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query id failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 1) {
				Log(LOG_ERR, "SchemaChecker", "query id failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell;
				row->get_cell(0, cell);
				if (cell->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query id failed, query result type error");
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

u32 SchemaChecker::get_value_from_result(double & value, const Object_s & result)
{
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() != 1) {
		return SYSTEM_SCHEMA_ERROR;
	}
	else {
		Row_s row;
		u32 ret = query_result->get_row(0, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query value failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 1) {
				Log(LOG_ERR, "SchemaChecker", "query value failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell;
				row->get_cell(0, cell);
				if (cell->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query value failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				else {
					Number* col_value = dynamic_cast<Number*>(cell.get());
					value = col_value->value();
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

u32 SchemaChecker::get_columns_desc_from_result(ColumnDesc & col_desc, const Object_s & result)
{
	QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
	if (query_result->size() != 1) {
		return SYSTEM_SCHEMA_ERROR;
	}
	else {
		Row_s row;
		u32 ret = query_result->get_row(0, row);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "SchemaChecker", "query columns_desc failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 3) {
				Log(LOG_ERR, "SchemaChecker", "query columns_desc failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell1;
				row->get_cell(0, cell1);
				if (cell1->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Object_s cell2;
				row->get_cell(1, cell2);
				if (cell2->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Object_s cell3;
				row->get_cell(2, cell3);
				if (cell3->get_type() != T_VARCHAR) {
					Log(LOG_ERR, "SchemaChecker", "query failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Number* num = dynamic_cast<Number*>(cell1.get());
				u32 cid = static_cast<u32>(num->value());
				num = dynamic_cast<Number*>(cell2.get());
				u32 tid = static_cast<u32>(num->value());
				Varchar* str = dynamic_cast<Varchar*>(cell3.get());
				u32 type = string_to_type(str->to_string());
				col_desc.set_tid_cid(tid, cid);
				col_desc.set_data_type(type);
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
			Log(LOG_ERR, "SchemaChecker", "query row desc failed");
			return SYSTEM_SCHEMA_ERROR;
		}
		else {
			if (row->get_row_desc().get_column_num() != 2) {
				Log(LOG_ERR, "SchemaChecker", "query row desc failed, query result type error");
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Object_s cell1;
				row->get_cell(0, cell1);
				if (cell1->get_type() != T_NUMBER) {
					Log(LOG_ERR, "SchemaChecker", "query row desc failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Object_s cell2;
				row->get_cell(1, cell2);
				if (cell2->get_type() != T_VARCHAR) {
					Log(LOG_ERR, "SchemaChecker", "query row desc failed, query result type error");
					return SYSTEM_SCHEMA_ERROR;
				}
				Number* num = dynamic_cast<Number*>(cell1.get());
				u32 cid = static_cast<u32>(num->value());
				Varchar* str = dynamic_cast<Varchar*>(cell2.get());
				u32 type = string_to_type(str->to_string());
				col_desc.set_tid_cid(table_id, cid);
				col_desc.set_data_type(type);
				row_desc.add_column_desc(col_desc);
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
		u32 ret = plan->optimizer();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				Log(LOG_ERR, "SchemaChecker", "resolve sys sql plan error:%s", result->to_string().c_str());
				return SYSTEM_SCHEMA_ERROR;
			}
			else {
				Log(LOG_ERR, "SchemaChecker", "resolve sys sql plan error");
				return SYSTEM_SCHEMA_ERROR;
			}
		}
		ret = plan->build_plan();
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
		col_desc.set_data_type(T_NUMBER);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 1);
		col_desc.set_data_type(T_VARCHAR);
		row_desc.add_column_desc(col_desc);
	}
	else if (table_name == "sys_tables") {
		col_desc.set_tid_cid(table_id, 0);
		col_desc.set_data_type(T_NUMBER);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 1);
		col_desc.set_data_type(T_VARCHAR);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 2);
		col_desc.set_data_type(T_NUMBER);
		row_desc.add_column_desc(col_desc);
	}
	else if (table_name == "sys_columns") {
		col_desc.set_tid_cid(table_id, 0);
		col_desc.set_data_type(T_NUMBER);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 1);
		col_desc.set_data_type(T_VARCHAR);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 2);
		col_desc.set_data_type(T_NUMBER);
		row_desc.add_column_desc(col_desc);
		col_desc.set_tid_cid(table_id, 3);
		col_desc.set_data_type(T_VARCHAR);
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
			col_desc.set_data_type(T_NUMBER);
		}
		else if(column_name == "name") {
			col_desc.set_tid_cid(table_id, 1);
			col_desc.set_data_type(T_VARCHAR);
		}
		else {
			ret = SYSTEM_SCHEMA_ERROR;
		}
	}
	else if (table_name == "sys_tables") {
		if (column_name == "id") {
			col_desc.set_tid_cid(table_id, 0);
			col_desc.set_data_type(T_NUMBER);
		}
		else if (column_name == "name") {
			col_desc.set_tid_cid(table_id, 1);
			col_desc.set_data_type(T_VARCHAR);
		}
		else if (column_name == "db_id") {
			col_desc.set_tid_cid(table_id, 2);
			col_desc.set_data_type(T_NUMBER);
		}
		else {
			ret = SYSTEM_SCHEMA_ERROR;
		}
	}
	else if (table_name == "sys_columns") {
		if (column_name == "id") {
			col_desc.set_tid_cid(table_id, 0);
			col_desc.set_data_type(T_NUMBER);
		}
		else if (column_name == "name") {
			col_desc.set_tid_cid(table_id, 1);
			col_desc.set_data_type(T_VARCHAR);
		}
		else if (column_name == "table_id") {
			col_desc.set_tid_cid(table_id, 2);
			col_desc.set_data_type(T_NUMBER);
		}
		else if (column_name == "type") {
			col_desc.set_tid_cid(table_id, 3);
			col_desc.set_data_type(T_VARCHAR);
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

u32 SchemaChecker::analyze_table_system(const String & database, const String & table)
{
	Vector<Pair<String, String>> columns;
	u32 ret = desc_table(database, table, columns);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "%s.%s not exists", database.c_str(), table.c_str());
		return ret;
	}
	ret = analyze_table_statis(database, table);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "analyze %s.%s failed", database.c_str(), table.c_str());
		return ret;
	}
	for (u32 i = 0; i < columns.size(); ++i) {
		if (columns[i].second == "varchar") {
			continue;
		}
		ret = analyze_column_statis(database, table, columns[i].first);
		if (ret != SUCCESS) {
			Log(LOG_WARN, "AnalyzeStatic", "analyze %s.%s:%s failed", database.c_str(), table.c_str(), column.c_str());
		}
	}
	return SUCCESS;
}

u32 SchemaChecker::analyze_table_statis(const String & database, const String & table)
{
	u32 table_id;
	u32 ret = get_table_id(database, table, table_id);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "%s.%s not exists", database.c_str(), table.c_str());
		return ret;
	}
	u32 row_count = 0;
	u64 file_size = TableSpace::table_space_size(database, table);
	if (file_size > 0) {
		String query = "select count(*) from " + database + "." + table +";";
		Object_s result;
		ret = execute_sys_sql(query, result);
		if (ret != SUCCESS) {
			Log(LOG_WARN, "AnalyzeStatic", "query %s.%s count failed", database.c_str(), table.c_str());
			return ret;
		}
		else {
			ret = get_id_from_result(row_count, result);
			if (ret != SUCCESS) {
				Log(LOG_WARN, "AnalyzeStatic", "get %s.%s count failed", database.c_str(), table.c_str());
				return ret;
			}
		}
	}
	String cur_time = DateTime::CurrentDatetime();
	String query = "insert into system.table_statis values("
		+ std::to_string(table_id) + ","
		+ std::to_string(row_count) + ","
		+ std::to_string(file_size) + ","
		+ "'" + cur_time + "');";
	Object_s result;
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "analyze %s.%s statis failed", database.c_str(), table.c_str());
	}
	return ret;
}

u32 SchemaChecker::analyze_column_statis(const String & database, const String & table, const String & column, bool varchar_type)
{
	u32 tid, cid;
	ColumnDesc col_desc;
	u32 ret = get_column_desc(database, table, column, col_desc);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "%s.%s:%s not exists", database.c_str(), table.c_str(), column.c_str());
		return ret;
	}
	col_desc.get_tid_cid(tid, cid);
	ret = get_table_id(database, table, tid);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "%s.%s not exists", database.c_str(), table.c_str());
		return ret;
	}
	u32 ndv, null_count;
	String max_value, min_value;
	//NDV统计
	String query = "select count(*) from ( select distinct `" + column + "` from `" + database + "`.`" + table + "`) as tmp;";
	Object_s result;
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "query %s.%s:%s NDV failed", database.c_str(), table.c_str(), column.c_str());
		return ret;
	}
	else {
		ret = get_id_from_result(ndv, result);
		if (ret != SUCCESS) {
			Log(LOG_WARN, "AnalyzeStatic", "get %s.%s:%s NDV failed", database.c_str(), table.c_str(), column.c_str());
			return ret;
		}
	}
	//null count统计
	query = "select count(`" + column + "`) from `" + database + "`.`" + table + "` where `" + column + "`  is null;";
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "query %s.%s:%s null count failed", database.c_str(), table.c_str(), column.c_str());
		return ret;
	}
	else {
		ret = get_id_from_result(null_count, result);
		if (ret != SUCCESS) {
			Log(LOG_WARN, "AnalyzeStatic", "get %s.%s:%s null count failed", database.c_str(), table.c_str(), column.c_str());
			return ret;
		}
	}
	//获取最大值
	query = "select max(`" + column + "`) from `" + database + "`.`" + table + "`;";
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "query %s.%s:%s max value failed", database.c_str(), table.c_str(), column.c_str());
		return ret;
	}
	else {
		double value;
		ret = get_value_from_result(value, result);
		if (ret != SUCCESS) {
			Log(LOG_WARN, "AnalyzeStatic", "get %s.%s:%s max value failed", database.c_str(), table.c_str(), column.c_str());
			max_value = "NULL";
		}
		else {
			max_value = std::to_string(value);
		}
	}
	//获取最小值
	query = "select min(`" + column + "`) from `" + database + "`.`" + table + "`;";
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "query %s.%s:%s min value failed", database.c_str(), table.c_str(), column.c_str());
		return ret;
	}
	else {
		double value;
		ret = get_value_from_result(value, result);
		if (ret != SUCCESS) {
			Log(LOG_WARN, "AnalyzeStatic", "get %s.%s:%s min value failed", database.c_str(), table.c_str(), column.c_str());
			min_value = "NULL";
		}
		else {
			min_value = std::to_string(value);
		}
	}
	String cur_time = DateTime::CurrentDatetime();
	query = "insert into system.column_statis values("
		+ std::to_string(tid) + ","
		+ std::to_string(cid) + ","
		+ std::to_string(ndv) + ","
		+ std::to_string(null_count) + ","
		+ max_value + ","
		+ min_value + ","
		+ "'" + cur_time + "');";
	ret = execute_sys_sql(query, result);
	if (ret != SUCCESS) {
		Log(LOG_WARN, "AnalyzeStatic", "analyze %s.%s:%s statis failed", database.c_str(), table.c_str(), column.c_str());
	}
	return ret;
}