#include "opt_est_info.h"
#include "statis_manager.h"
#include "schema_guard.h"
#include "error.h"
#include "log.h"
#include <cmath>

using namespace CatDB::Optimizer;
using namespace CatDB::Parser;

u32 EstInfo::add_view_statis(u32 table_id, TableEstInfo_s &table_statis)
{
    u32 ret = SUCCESS;
    tables_statis[table_id] = table_statis;
    return ret;
}

u32 EstInfo::init_table_statis(u32 table_ref_id, u32 table_id)
{
    u32 ret = SUCCESS;
    if (tables_statis.find(table_id) != tables_statis.cend()) {
        //inited
    } else {
        StatisManager_s manager = StatisManager::make_statis_manager();
        TableEstInfo_s table_statis = TableEstInfo::make_table_est_info();
        table_statis->table_id = table_id;
        table_statis->table_rows = manager->get_table_row_count(table_ref_id);
        table_statis->output_rows = table_statis->table_rows;
        table_statis->block_count = manager->get_table_block_count(table_ref_id);
        table_statis->row_width = manager->get_avg_row_size(table_ref_id);
        tables_statis[table_id] = table_statis;

        SchemaGuard_s guard = SchemaGuard::make_schema_guard();
        TableInfo_s table_info;
        CHECK(guard->find_table_info(table_ref_id, table_info));
        for (auto iter = table_info->id_column_infos.cbegin(); iter != table_info->id_column_infos.cend(); ++iter) {
            ColumnEstInfo_s column_statis = ColumnEstInfo::make_column_est_info();
            column_statis->column_id = iter->first;
            column_statis->max_value = manager->get_column_max_value(table_ref_id, column_statis->column_id);
            column_statis->min_value = manager->get_column_min_value(table_ref_id, column_statis->column_id);
            column_statis->ndv = manager->get_column_ndv(table_ref_id, column_statis->column_id);
            column_statis->ori_ndv = column_statis->ndv;
            column_statis->null_count = manager->get_column_null_count(table_ref_id, column_statis->column_id);
            table_statis->columns_statis[column_statis->column_id] = column_statis;
        }
        //for row id
        {
            ColumnEstInfo_s column_statis = ColumnEstInfo::make_column_est_info();
            column_statis->column_id = ROWID_COLUMN_ID;
            column_statis->max_value = table_statis->table_rows;
            column_statis->min_value = 0;
            column_statis->ndv = table_statis->table_rows;
            column_statis->ori_ndv = column_statis->ndv;
            column_statis->null_count = 0;
            table_statis->columns_statis[column_statis->column_id] = column_statis;
        }
    }
    return ret;
}

u32 EstInfo::set_table_output_rows(u32 table_id, double output_rows)
{
    u32 ret = SUCCESS;
    auto iter = tables_statis.find(table_id);
    if (iter == tables_statis.end()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("table not found", K(table_id), K(ret));
        return ret;
    }
    TableEstInfo_s &table_statis = iter->second;
    table_statis->output_rows = output_rows;
    if (output_rows >= table_statis->table_rows) {
        return ret;
    }
    LOG_TRACE("start to update column ndv:", K(table_statis->table_rows), K(output_rows));
    for (auto iter = table_statis->columns_statis.begin(); iter != table_statis->columns_statis.end(); ++iter) {
        ColumnEstInfo_s &column_statis = iter->second;
        column_statis->ndv = column_statis->ori_ndv * 
        (1 - std::pow((1 - output_rows / table_statis->table_rows), table_statis->table_rows / column_statis->ori_ndv));
        LOG_TRACE("update column ndv:", K(column_statis->ori_ndv), K(column_statis->ndv));
    }
    return ret;
}

u32 EstInfo::get_table_statis(u32 table_id, TableEstInfo_s &table_statis)
{
    u32 ret = SUCCESS;
    auto iter = tables_statis.find(table_id);
    if (iter == tables_statis.end()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("table not found", K(table_id), K(ret));
        return ret;
    }
    table_statis = iter->second;
    return ret;
}

u32 EstInfo::get_column_statis(u32 table_id, u32 column_id, ColumnEstInfo_s &column_statis)
{
    u32 ret = SUCCESS;
    auto iter = tables_statis.find(table_id);
    if (iter == tables_statis.end()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("table not found", K(table_id), K(ret));
        return ret;
    }
    TableEstInfo_s &table_statis = iter->second;
    auto iter2 = table_statis->columns_statis.find(column_id);
    if (iter2 == table_statis->columns_statis.end()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("column not found", K(column_id), K(ret));
        return ret;
    }
    column_statis = iter2->second;
    return ret;
}