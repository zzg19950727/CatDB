#include "statis_manager.h"
#include "schema_guard.h"
#include "sql_engine.h"
#include "table_space.h"
#include "obj_datetime.h"
#include "query_ctx.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Storage;
using namespace CatDB::Common;
using namespace CatDB::Optimizer;
using namespace CatDB::Parser;
using namespace CatDB::Sql;

StatisManager_s StatisManager::make_statis_manager()
{
    static StatisManager_s manager(new StatisManager());
	return manager;
}

u32 StatisManager::analyze_table(const String& database, const String& table, double sample_size)
{
    u32 ret = SUCCESS;
    SchemaGuard_s guard = SchemaGuard::make_schema_guard();
	if (database == "*") {
		Vector<String> databases;
		CHECK(guard->get_all_database(databases));
		for (u32 i = 0; i < databases.size(); ++i) {
			Vector<String> tables;
			CHECK(guard->get_all_table(databases[i], tables));
			for (u32 j = 0; j < tables.size(); ++j) {
				CHECK(inner_analyze_table(databases[i], tables[j], sample_size));
			}
		}
	} else if (table == "*") {
		Vector<String> tables;
		CHECK(guard->get_all_table(database, tables));
		for (u32 i = 0; i < tables.size(); ++i) {
			CHECK(inner_analyze_table(database, tables[i], sample_size));
		}
	} else {
		CHECK(inner_analyze_table(database, table, sample_size));
	}
	return ret;
}

double StatisManager::get_table_row_count(u32 tid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end()) {
        return 100000;
    } else {
        return iter->second->row_count;
    }
}

double StatisManager::get_avg_row_size(u32 tid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end() || iter->second->row_count < 1) {
        return 100;
    } else {
        return iter->second->space_size / iter->second->row_count;
    }
}

double StatisManager::get_table_block_count(u32 tid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end()) {
        return 100;
    } else {
        return iter->second->space_size / PAGE_SIZE;
    }
}

double StatisManager::get_column_ndv(u32 tid, u32 cid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end()) {
        return 1000;
    } else {
        auto iter2 = iter->second->column_statis.find(cid);
        if (iter2 == iter->second->column_statis.end()) {
            return 1000;
        } else {
            return iter2->second->ndv;
        }
    }
}

double StatisManager::get_column_max_value(u32 tid, u32 cid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end()) {
        return 1000;
    } else {
        auto iter2 = iter->second->column_statis.find(cid);
        if (iter2 == iter->second->column_statis.end()) {
            return 1000;
        } else {
            return iter2->second->max_value;
        }
    }
}

double StatisManager::get_column_min_value(u32 tid, u32 cid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end()) {
        return 0;
    } else {
        auto iter2 = iter->second->column_statis.find(cid);
        if (iter2 == iter->second->column_statis.end()) {
            return 0;
        } else {
            return iter2->second->min_value;
        }
    }
}

double StatisManager::get_column_null_count(u32 tid, u32 cid)
{
    auto iter = all_table_statis.find(tid);
    if (iter == all_table_statis.end()) {
        return 1000;
    } else {
        auto iter2 = iter->second->column_statis.find(cid);
        if (iter2 == iter->second->column_statis.end()) {
            return 1000;
        } else {
            return iter2->second->null_count;
        }
    }
}

u32 StatisManager::delete_table_statis(u32 tid)
{
    u32 ret = SUCCESS;
    String sql = "DELETE FROM SYSTEM.TABLE_STATIS WHERE TID = ";
    sql += std::to_string(tid) + ";";
    ResultSet_s result;
    CHECK(execute_sys_sql(sql, result));
    sql = "DELETE FROM SYSTEM.COLUMN_STATIS WHERE TID = ";
    sql += std::to_string(tid) + ";";
    all_table_statis.erase(tid);
    CHECK(execute_sys_sql(sql, result));
    return ret;
}

u32 StatisManager::execute_sys_sql(const String& sql, ResultSet_s &result, double sample_size)
{
    u32 ret = SUCCESS;
    QueryCtx_s query_ctx = QueryCtx::make_query_ctx();
    query_ctx->sample_size = sample_size;
	CHECK(SqlEngine::handle_inner_sql(sql, query_ctx, result));
	return ret;
}

u32 StatisManager::init_statis_cache()
{
    u32 ret = SUCCESS;
    Vector<TableStatis_s> table_statis;
    Vector<ColumnStatis_s> column_statis;
    CHECK(init_table_statis(table_statis));
    CHECK(init_column_statis(column_statis));
    u32 j = 0;
    for (u32 i = 0; i < table_statis.size(); ++i) {
        all_table_statis[table_statis[i]->tid] = table_statis[i];
        while (j < column_statis.size()) {
            if (table_statis[i]->tid != column_statis[j]->tid) {
                break;
            } else {
                table_statis[i]->column_statis[column_statis[j]->cid] = column_statis[j];
                ++j;
            }
        }
    }
    return SUCCESS;
}

u32 StatisManager::init_table_statis(Vector<TableStatis_s> &table_statis)
{
    u32 ret = SUCCESS;
    ResultSet_s table_statis_result;
    String table_statis_sql = R"(SELECT tid, row_count, space_size FROM system.table_statis A
                            WHERE analyze_time = (SELECT MAX(analyze_time) from system.table_statis B WHERE A.tid=B.tid) 
                            ORDER BY tid;)";
    CHECK(execute_sys_sql(table_statis_sql, table_statis_result));
    Row_s row;
    CHECK(table_statis_result->open());
    MY_ASSERT(3 == table_statis_result->get_column_count());
    while (SUCC( ret = table_statis_result->get_next_row(row))) {
		TableStatis_s statis = TableStatis::make_table_statis();
        Object_s cell;
        u32 pos = 0;
        row->get_cell(pos++, cell);
        statis->tid = cell->value();
        row->get_cell(pos++, cell);
        statis->row_count = cell->value();
        row->get_cell(pos++, cell);
        statis->space_size = cell->value();
        table_statis.push_back(statis);
	}
    CHECK(table_statis_result->close());
    return ret;
}

u32 StatisManager::init_column_statis(Vector<ColumnStatis_s> &column_statis)
{
    u32 ret = SUCCESS;
    ResultSet_s column_statis_result;
    String column_statis_sql = R"(SELECT tid, cid, ndv, null_count, max_value, min_value FROM system.column_statis A
                                WHERE analyze_time = (SELECT MAX(analyze_time) from system.column_statis B WHERE A.tid=B.tid and A.cid = B.cid) 
                                ORDER BY tid;)";
    CHECK(execute_sys_sql(column_statis_sql, column_statis_result));
    Row_s row;
    CHECK(column_statis_result->open());
    MY_ASSERT(6 == column_statis_result->get_column_count());
    while (SUCC( ret = column_statis_result->get_next_row(row))) {
        ColumnStatis_s statis = ColumnStatis::make_column_statis();
        Object_s cell;
        u32 pos = 0;
        row->get_cell(pos++, cell);
        statis->tid = cell->value();
        row->get_cell(pos++, cell);
        statis->cid = cell->value();
        row->get_cell(pos++, cell);
        statis->ndv = cell->value();
        row->get_cell(pos++, cell);
        statis->null_count = cell->value();
        row->get_cell(pos++, cell);
        statis->max_value = cell->value();
        row->get_cell(pos++, cell);
        statis->min_value = cell->value();
        column_statis.push_back(statis);
	}
    CHECK(column_statis_result->close());
    return ret;
}

u32 StatisManager::inner_analyze_table(const String& database, const String& table, double sample_size)
{
    u32 ret = SUCCESS;
    u32 table_id = INVALID_ID;
    Vector<u32> column_ids;
    String query;
    CHECK(generate_analyze_sql(database, 
                               table, 
                               query, 
                               table_id, 
                               column_ids));
	u64 file_size = TableSpace::table_space_size(database, table);
	if (file_size / 16384 < 1.0 / sample_size) {
		sample_size = 1;
	}
    if (sample_size > 1) {
        sample_size = 1;
    }
    ResultSet_s result;
    CHECK(execute_sys_sql(query, result, sample_size));
    TableStatis_s table_statis;
    CHECK(generate_table_statis(table_id, 
                                file_size, 
                                sample_size, 
                                column_ids, 
                                result, 
                                table_statis));
                                
    String table_statis_query, column_statis_query;
    CHECK(generate_load_sql(table_statis, 
                            table_statis_query, 
                            column_statis_query));
    CHECK(execute_sys_sql(table_statis_query, result));
    CHECK(execute_sys_sql(column_statis_query, result));
	return ret;
}

u32 StatisManager::generate_analyze_sql(const String& database,
                                        const String& table,
                                        String &query,
                                        u32 &table_id,
                                        Vector<u32> &column_ids)
{
    u32 ret = SUCCESS;
    SchemaGuard_s guard = SchemaGuard::make_schema_guard();
    TableInfo_s table_info;
    CHECK(guard->find_table_info(database, table, table_info));
    table_id = table_info->table_id;
    query = "select count(1) ";
    for (auto iter = table_info->id_column_infos.cbegin(); iter != table_info->id_column_infos.cend(); ++iter) {
		if (T_VARCHAR == iter->second->column_type.res_type) {
			continue;
		}
        const String &column = iter->second->column_name;
        u32 column_id = iter->first;
        column_ids.push_back(column_id);
		query += ", count(distinct " + column + ")" + 
                ", count(case when " + column + " is null then 1 else null end)" +
                ", max(" + column + ")" +
                ", min(" + column + ")";
	}
    query += " from " + database + "." + table + ";";
    return ret;
}

u32 StatisManager::generate_table_statis(u32 table_id,
                                        u32 space_size,
                                        double sample_size,
                                        Vector<u32> &column_ids,
                                        ResultSet_s &result,
                                        TableStatis_s &table_statis)
{
    u32 ret = SUCCESS;
    Row_s row;
    CHECK(result->open());
    MY_ASSERT(4*column_ids.size()+1 == result->get_column_count());
    while (SUCC( ret = result->get_next_row(row))) {
    }
    CHECK(result->close());
    MY_ASSERT(row);
    table_statis = TableStatis::make_table_statis();
    table_statis->tid = table_id;
    table_statis->space_size = space_size;
    u32 pos = 0;
    Object_s cell;
    row->get_cell(pos++, cell);
    table_statis->row_count = cell->value() / sample_size;
    for (u32 i = 0; i < column_ids.size(); ++i) {
        ColumnStatis_s statis = ColumnStatis::make_column_statis();
        statis->tid = table_id;
        statis->cid = column_ids[i];
        row->get_cell(pos++, cell);
        statis->ndv = cell->value() / sample_size;
        row->get_cell(pos++, cell);
        statis->null_count = cell->value() / sample_size;
        row->get_cell(pos++, cell);
        statis->max_value = cell->value();
        row->get_cell(pos++, cell);
        statis->min_value = cell->value();
        table_statis->column_statis[column_ids[i]] = statis;
    }
    all_table_statis[table_id] = table_statis;
    return ret;
}

u32 StatisManager::generate_load_sql(TableStatis_s &table_statis, String &table_statis_query, String &column_statis_query)
{
    u32 ret = SUCCESS;
    DateTime_s cur;
    CHECK(DateTime::current_datetime(cur));
    String cur_time = cur->to_string();
	table_statis_query = "insert into system.table_statis values("
		+ std::to_string(table_statis->tid) + ","
		+ std::to_string(table_statis->row_count) + ","
		+ std::to_string(table_statis->space_size) + ","
		+ "'" + cur_time + "');";
    column_statis_query = "insert into system.column_statis values ";
    for (auto iter = table_statis->column_statis.cbegin(); iter != table_statis->column_statis.cend(); ++iter) {
        ColumnStatis_s column_statis = iter->second;
        column_statis_query += "("
		+ std::to_string(column_statis->tid) + ","
		+ std::to_string(column_statis->cid) + ","
		+ std::to_string(column_statis->ndv) + ","
		+ std::to_string(column_statis->null_count) + ","
        + std::to_string(column_statis->max_value) + ","
        + std::to_string(column_statis->min_value) + ","
		+ "'" + cur_time + "'),";
    }
    column_statis_query[column_statis_query.size()-1] = ';';
    return ret;
}