#ifndef OPT_EST_INFO_H
#define OPT_EST_INFO_H
#include "type.h"

class ColumnStatis {
u32 column_id;
double null_count;
double ori_ndv;
double ndv;
double max_value;
double min_value;
};

class TableStatis {
UnorderedHashMap<u32, ColumnStatis_s> columns_statis;
u32 table_id;
double table_rows;
double output_rows;
double block_count;
double row_width;
};

class EstInfo {
u32 init_table_statis(u32 table_ref_id, u32 table_id);
u32 set_table_output_rows(u32 table_id, double output_rows);
u32 get_table_statis(u32 table_id, TableStatis_s &table_statis);
u32 get_column_statis(u32 table_id, u32 column_id, ColumnStatis_s &column_statis);
UnorderedHashMap<u32, TableStatis_s> tables_statis;
};

#endif	//OPT_EST_INFO_H
