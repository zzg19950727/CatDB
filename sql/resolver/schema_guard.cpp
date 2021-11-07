#include "schema_guard.h"
#include "statis_manager.h"
#include "system_table.def"
#include "sql_engine.h"
#include "query_result.h"
#include "query_ctx.h"
#include "error.h"
#include "row.h"

using namespace CatDB::Optimizer;
using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Sql;

SchemaGuard_s SchemaGuard::make_schema_guard()
{
    static SchemaGuard_s guard(new SchemaGuard());
	return guard;
}

u32 SchemaGuard::find_table_info(const String& db_name, const String& table_name, TableInfo_s &table_info)
{
    auto iter = name_database_infos.find(db_name);
    if (iter == name_database_infos.end()) {
        return DATABASE_NOT_EXISTS;
    }
    auto iter2 = iter->second->name_table_infos.find(table_name);
    if (iter2 == iter->second->name_table_infos.end()) {
        return TABLE_NOT_EXISTS;
    }
    table_info = iter2->second;
    return SUCCESS;
}

u32 SchemaGuard::find_table_info(u32 table_id, TableInfo_s &table_info)
{
    u32 db_id = table_id >> 20;
    auto iter = id_database_infos.find(db_id);
    if (iter == id_database_infos.end()) {
        return DATABASE_NOT_EXISTS;
    }
    auto iter2 = iter->second->id_table_infos.find(table_id);
    if (iter2 == iter->second->id_table_infos.end()) {
        return TABLE_NOT_EXISTS;
    }
    table_info = iter2->second;
    return SUCCESS;
}

u32 SchemaGuard::find_column_info(u32 table_id, const String& column_name, ColumnInfo_s &column_info)
{
    u32 ret = SUCCESS;
    TableInfo_s table_info;
    CHECK(find_table_info(table_id, table_info));
    auto iter = table_info->name_column_infos.find(column_name);
    if (iter == table_info->name_column_infos.end()) {
        return COLUMN_NOT_EXISTS;
    }
    column_info = iter->second;
    return ret;
}

u32 SchemaGuard::find_column_info(u32 table_id, u32 column_id, ColumnInfo_s &column_info)
{
    u32 ret = SUCCESS;
    TableInfo_s table_info;
    CHECK(find_table_info(table_id, table_info));
    auto iter = table_info->id_column_infos.find(column_id);
    if (iter == table_info->id_column_infos.end()) {
        return COLUMN_NOT_EXISTS;
    }
    column_info = iter->second;
    return ret;
}

u32 SchemaGuard::find_database_info(const String& db_name, DatabaseInfo_s &db_info)
{
    auto iter = name_database_infos.find(db_name);
    if (iter == name_database_infos.end()) {
        return DATABASE_NOT_EXISTS;
    }
    db_info = iter->second;
    return SUCCESS;
}

u32 SchemaGuard::get_all_database(Vector<String> &databases)
{
    for (auto iter = name_database_infos.cbegin(); iter != name_database_infos.cend(); ++iter) {
        databases.push_back(iter->first);
    }
    return SUCCESS;
}

u32 SchemaGuard::get_all_table(const String& db_name, Vector<String> &tables)
{
    u32 ret = SUCCESS;
    auto iter = name_database_infos.find(db_name);
    MY_ASSERT(iter != name_database_infos.end());
    DatabaseInfo_s info = iter->second;
    for (auto iter = info->name_table_infos.cbegin(); 
         iter != info->name_table_infos.cend(); 
         ++iter) {
        tables.push_back(iter->first);
    }
    return ret;
}

u32 SchemaGuard::add_database(const String& name)
{
    u32 ret = SUCCESS;
    auto iter = name_database_infos.find(name);
    MY_ASSERT(iter == name_database_infos.end());
    u32 id = INVALID_ID;
    CHECK(generate_database_id(id));
    CHECK(add_database_to_inner_table(id, name));
    CHECK(add_database_to_cache(id, name));
    return ret;
}

u32 SchemaGuard::del_database(const String& name)
{
    u32 ret = SUCCESS;
    auto iter = name_database_infos.find(name);
    MY_ASSERT(iter != name_database_infos.end());
    DatabaseInfo_s database_info = iter->second;
    CHECK(del_database_from_cache(database_info->db_id, name));
    CHECK(del_database_from_inner_table(database_info->db_id));
    StatisManager_s statis_manager = StatisManager::make_statis_manager();
    for (auto iter = database_info->id_table_infos.cbegin(); 
         iter != database_info->id_table_infos.cend(); 
         ++iter) {
        CHECK(statis_manager->delete_table_statis(iter->first));
    }
    return ret;
}

u32 SchemaGuard::add_table(const String& db_name,
                          const String& table_name,
                          const Vector<Pair<String,String>> &columns)
{
    u32 ret = SUCCESS;
    auto iter = name_database_infos.find(db_name);
    MY_ASSERT(iter != name_database_infos.end());
    u32 db_id = iter->second->db_id;
    u32 table_id = INVALID_ID;
    CHECK(generate_table_id(db_id, table_id));
    CHECK(add_table_to_inner_table(db_id, table_id, table_name, columns));
    CHECK(add_table_to_cache(db_id, table_id, table_name, columns));
    return ret;
}

u32 SchemaGuard::del_table(const String &db_name, const String& table_name)
{
    u32 ret = SUCCESS;
    auto iter = name_database_infos.find(db_name);
    MY_ASSERT(iter != name_database_infos.end());
    u32 db_id = iter->second->db_id;
    auto iter2 = iter->second->name_table_infos.find(table_name);
    MY_ASSERT(iter2 != iter->second->name_table_infos.end());
    u32 table_id = iter2->second->table_id;
    CHECK(del_table_from_cache(db_id, table_id, table_name));
    CHECK(del_table_from_inner_table(db_id, table_id));
    StatisManager_s statis_manager = StatisManager::make_statis_manager();
    CHECK(statis_manager->delete_table_statis(table_id));
    return ret;
}

u32 SchemaGuard::execute_sys_sql(const String& sql, QueryResult_s &result)
{
    u32 ret = SUCCESS;
    QueryCtx query_ctx;
	ResultSet_s result_set;
	CHECK(SqlEngine::handle_inner_sql(sql, query_ctx, result_set));
    MY_ASSERT(result_set);
	result = result_set->get_query_result();
	return ret;
}

u32 SchemaGuard::init_guard()
{
    u32 ret = SUCCESS;
    Vector<TableInfo_s> table_info;
    Vector<ColumnInfo_s> column_info;
    CHECK(add_database_to_cache(SYSTEM_DB_ID, SYSTEM_DB_NAME));
    CHECK(add_table_to_cache(SYSTEM_DB_ID, SYS_DB_ID, SYS_DB_NAME, SYS_DB_DEF));
    CHECK(add_table_to_cache(SYSTEM_DB_ID, SYS_TABLE_ID, SYS_TABLE_NAME, SYS_TABLE_DEF));
    CHECK(add_table_to_cache(SYSTEM_DB_ID, SYS_COLUMN_ID, SYS_COLUMN_NAME, SYS_COLUMN_DEF));
    init_database_info();
    init_table_info(table_info);
    init_column_info(column_info);
    if (table_info.empty() || column_info.empty()) {
        CHECK(init_system_schema());
    }
    u32 j = 0;
    for (u32 i = 0; i < table_info.size(); ++i) {
        auto iter = id_database_infos.find(table_info[i]->db_id);
        MY_ASSERT(iter != id_database_infos.end());
        DatabaseInfo_s database_info = iter->second;
        database_info->id_table_infos[table_info[i]->table_id] = table_info[i];
        database_info->name_table_infos[table_info[i]->table_name] = table_info[i];
        while (j < column_info.size()) {
            if (table_info[i]->table_id != column_info[j]->table_id) {
                break;
            } else {
                table_info[i]->id_column_infos[column_info[j]->column_id] = column_info[j];
                table_info[i]->name_column_infos[column_info[j]->column_name] = column_info[j];
                ++j;
            }
        }
    }
    return ret;
}

u32 SchemaGuard::init_database_info()
{
    u32 ret = SUCCESS;
    QueryResult_s database_info_result;
    String database_info_sql = "SELECT id, name FROM " + FULL_SYSTEM_DB_NAME + 
                               " ORDER BY id;";
    ret = execute_sys_sql(database_info_sql, database_info_result);
    if (FAIL(ret)) {
        LOG_ERR("database not init");
        ret = SUCCESS;
        return ret;
    }
    for (u32 i = 0; i < database_info_result->size(); ++i) {
		Row_s row;
        Object_s cell;
        u32 pos = 0;
		CHECK(database_info_result->get_row(i, row));
        MY_ASSERT(2 == row->get_row_desc().get_column_num());
		DatabaseInfo_s info = DatabaseInfo::make_database_info();
        row->get_cell(pos++, cell);
        info->db_id = cell->value();
        row->get_cell(pos++, cell);
        info->db_name = cell->to_string();
        if (name_database_infos.find(info->db_name) == name_database_infos.cend()) {
            id_database_infos[info->db_id] = info;
            name_database_infos[info->db_name] = info;
        }
	}
    return ret;
}

u32 SchemaGuard::init_table_info(Vector<TableInfo_s> &table_info)
{
    u32 ret = SUCCESS;
    QueryResult_s table_info_result;
    String table_info_sql = "SELECT db_id, id, name FROM " + FULL_SYSTEM_TABLE_NAME + 
                            " ORDER BY id;";
    ret = execute_sys_sql(table_info_sql, table_info_result);
    if (FAIL(ret)) {
        LOG_ERR("database not init");
        ret = SUCCESS;
        return ret;
    }
    for (u32 i = 0; i < table_info_result->size(); ++i) {
		Row_s row;
        Object_s cell;
        u32 pos = 0;
		CHECK(table_info_result->get_row(i, row));
        MY_ASSERT(3 == row->get_row_desc().get_column_num());
		TableInfo_s info = TableInfo::make_table_info();
        row->get_cell(pos++, cell);
        info->db_id = cell->value();
        row->get_cell(pos++, cell);
        info->table_id = cell->value();
        row->get_cell(pos++, cell);
        info->table_name = cell->to_string();
        table_info.push_back(info);
	}
    return ret;
}

u32 SchemaGuard::init_column_info(Vector<ColumnInfo_s> &column_info)
{
    u32 ret = SUCCESS;
    QueryResult_s column_info_result;
    String column_info_sql = "SELECT table_id, id, name, type FROM " + FULL_SYSTEM_COLUMN_NAME + 
                             " ORDER BY table_id;";
    ret = execute_sys_sql(column_info_sql, column_info_result);
    if (FAIL(ret)) {
        ret = SUCCESS;
        return ret;
    }
    for (u32 i = 0; i < column_info_result->size(); ++i) {
		Row_s row;
        Object_s cell;
        u32 pos = 0;
		CHECK(column_info_result->get_row(i, row));
        MY_ASSERT(4 == row->get_row_desc().get_column_num());
        ColumnInfo_s info = ColumnInfo::make_column_info();
        row->get_cell(pos++, cell);
        info->table_id = cell->value();
        row->get_cell(pos++, cell);
        info->column_id = cell->value();
        row->get_cell(pos++, cell);
        info->column_name = cell->to_string();
        row->get_cell(pos++, cell);
        info->column_type = cell->to_string();
        column_info.push_back(info);
	}
    return ret;
}

u32 SchemaGuard::init_system_schema()
{
    u32 ret = SUCCESS;
    CHECK(add_database_to_cache(SYSTEM_DB_ID, SYSTEM_DB_NAME));
    CHECK(add_table_to_cache(SYSTEM_DB_ID, SYS_DB_ID, SYS_DB_NAME, SYS_DB_DEF));
    CHECK(add_table_to_cache(SYSTEM_DB_ID, SYS_TABLE_ID, SYS_TABLE_NAME, SYS_TABLE_DEF));
    CHECK(add_table_to_cache(SYSTEM_DB_ID, SYS_COLUMN_ID, SYS_COLUMN_NAME, SYS_COLUMN_DEF));
    CHECK(add_database_to_inner_table(SYSTEM_DB_ID, SYSTEM_DB_NAME));
    CHECK(add_table_to_inner_table(SYSTEM_DB_ID, SYS_DB_ID, SYS_DB_NAME, SYS_DB_DEF));
    CHECK(add_table_to_inner_table(SYSTEM_DB_ID, SYS_TABLE_ID, SYS_TABLE_NAME, SYS_TABLE_DEF));
    CHECK(add_table_to_inner_table(SYSTEM_DB_ID, SYS_COLUMN_ID, SYS_COLUMN_NAME, SYS_COLUMN_DEF));
    LOG_TRACE("succeed to init database inner table");
    return ret;
}

u32 SchemaGuard::add_database_to_cache(u32 id, const String& name)
{
    u32 ret = SUCCESS;
    DatabaseInfo_s database_info = DatabaseInfo::make_database_info();
    database_info->db_id = id;
    database_info->db_name = name;
    id_database_infos[id] = database_info;
    name_database_infos[name] = database_info;
    return ret;
}

u32 SchemaGuard::add_database_to_inner_table(u32 id, const String& name)
{
    u32 ret = SUCCESS;
    QueryResult_s result;
    String query = "insert into " + FULL_SYSTEM_DB_NAME + " values(" +
                     std::to_string(id) + ",\"" + name + "\");";
	CHECK(execute_sys_sql(query, result));
    return ret;
}

u32 SchemaGuard::add_table_to_cache(u32 db_id,
                                   u32 table_id,
                                   const String& name,
                                   const Vector<Pair<String,String>> &columns)
{
    u32 ret = SUCCESS;
    auto iter = id_database_infos.find(db_id);
    MY_ASSERT(iter != id_database_infos.end());
    DatabaseInfo_s database_info = iter->second;
    TableInfo_s table_info = TableInfo::make_table_info();
    table_info->db_id = db_id;
    table_info->table_id = table_id;
    table_info->table_name = name;
    database_info->id_table_infos[table_id] = table_info;
    database_info->name_table_infos[name] = table_info;

    for (u32 i = 0; i<columns.size(); ++i) {
        ColumnInfo_s column_info = ColumnInfo::make_column_info();
        column_info->table_id = table_id;
        column_info->column_id = i;
        column_info->column_name = columns[i].first;
        column_info->column_type = columns[i].second;
        table_info->id_column_infos[column_info->column_id] = column_info;
        table_info->name_column_infos[column_info->column_name] = column_info;
    }
    return ret;
}

u32 SchemaGuard::add_table_to_inner_table(u32 db_id,
                                        u32 table_id,
                                        const String& name,
                                        const Vector<Pair<String,String>> &columns)
{
    u32 ret = SUCCESS;
    String query = "insert into " + FULL_SYSTEM_TABLE_NAME + " values(" +
            std::to_string(db_id) + "," + std::to_string(table_id) + ",\"" + name + "\");";
	QueryResult_s result;
	CHECK(execute_sys_sql(query, result));
	for (u32 i = 0; i < columns.size(); ++i) {
		query = "insert into " + FULL_SYSTEM_COLUMN_NAME + " values(" + 
        std::to_string(table_id) + "," + std::to_string(i) + ",\"" +
			columns[i].first + "\",\"" + columns[i].second + "\");";
		CHECK(execute_sys_sql(query, result));
	}
    return ret;
}

u32 SchemaGuard::del_database_from_cache(u32 id, const String& name)
{
    id_database_infos.erase(id);
    name_database_infos.erase(name);
    return SUCCESS;
}

u32 SchemaGuard::del_database_from_inner_table(u32 id)
{
    u32 ret = SUCCESS;
    QueryResult_s result;
    //delete column info
    String query = String("DELETE C FROM SYSTEM.SYS_DATABASES A, \
                    SYSTEM.SYS_TABLES B, SYSTEM.SYS_COLUMNS C  \
                    WHERE A.ID = B.DB_ID AND B.ID = C.TABLE_ID \
                    AND A.ID = ") + std::to_string(id) + ";";
	CHECK(execute_sys_sql(query, result));
    query = "DELETE B FROM SYSTEM.SYS_DATABASES A, \
                    SYSTEM.SYS_TABLES B  \
                    WHERE A.ID = B.DB_ID \
                    AND A.ID = " +
                     std::to_string(id) + ";";
	CHECK(execute_sys_sql(query, result));
    query = "DELETE FROM SYSTEM.SYS_DATABASES \
                    WHERE ID = " +
                     std::to_string(id) + ";";
	CHECK(execute_sys_sql(query, result));
    return ret;
}

u32 SchemaGuard::del_table_from_cache(u32 db_id,
                                   u32 table_id,
                                   const String& name)
{
    u32 ret = SUCCESS;
    auto iter = id_database_infos.find(db_id);
    MY_ASSERT(iter != id_database_infos.end());
    DatabaseInfo_s database_info = iter->second;
    database_info->id_table_infos.erase(table_id);
    database_info->name_table_infos.erase(name);
    return ret;
}

u32 SchemaGuard::del_table_from_inner_table(u32 db_id, u32 table_id)
{
    u32 ret = SUCCESS;
	QueryResult_s result;\
	//delete column info
    String query = String("DELETE C FROM SYSTEM.SYS_TABLES B, SYSTEM.SYS_COLUMNS C  \
                    WHERE B.ID = C.TABLE_ID \
                    AND B.DB_ID = ") + std::to_string(db_id) +
                    "AND B.ID = " + std::to_string(table_id) + ";";
	CHECK(execute_sys_sql(query, result));
    query = "DELETE FROM SYSTEM.SYS_TABLES  \
                    WHERE DB_ID = " + std::to_string(db_id) +
                    "AND ID = " + std::to_string(table_id) + ";";
	CHECK(execute_sys_sql(query, result));
    return ret;
}

u32 SchemaGuard::generate_database_id(u32 &id)
{
    u32 ret = SUCCESS;
    id = 0;
    for (auto iter = id_database_infos.cbegin(); 
         iter != id_database_infos.cend(); 
         ++iter) {
        if (id < iter->second->db_id) {
            id = iter->second->db_id;
        }
    }
    ++id;
    return ret;
}

u32 SchemaGuard::generate_table_id(u32 db_id, u32 &table_id)
{
    u32 ret = SUCCESS;
    auto iter = id_database_infos.find(db_id);
    MY_ASSERT(iter != id_database_infos.end());
    DatabaseInfo_s database_info = iter->second;
    table_id = 0;
    for (auto iter = database_info->id_table_infos.cbegin(); 
         iter != database_info->id_table_infos.cend(); 
         ++iter) {
        if (table_id < iter->second->table_id) {
            table_id = iter->second->table_id;
        }
    }
    ++table_id;
    table_id = db_id << 20 | table_id;
    return ret;
}