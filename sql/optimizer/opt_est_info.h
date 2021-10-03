#ifndef OPT_EST_INFO_H
#define OPT_EST_INFO_H
#include "type.h"

namespace CatDB {
    namespace Optimizer {
        DECLARE(ColumnEstInfo);
        DECLARE(TableEstInfo);
        DECLARE(EstInfo);

        struct ColumnEstInfo {
            u32 column_id;
            double null_count;
            double ori_ndv;
            double ndv;
            double max_value;
            double min_value;

            static ColumnEstInfo_s make_column_est_info() { return ColumnEstInfo_s(new ColumnEstInfo); }
        };

        struct TableEstInfo {
            UnorderedHashMap<u32, ColumnEstInfo_s> columns_statis;
            u32 table_id;
            double table_rows;
            double output_rows;
            double block_count;
            double row_width;

            static TableEstInfo_s make_table_est_info() { return TableEstInfo_s(new TableEstInfo()); }
        };

        struct EstInfo {
            u32 add_view_statis(u32 table_id, TableEstInfo_s &table_statis);
            u32 init_table_statis(u32 table_ref_id, u32 table_id);
            u32 set_table_output_rows(u32 table_id, double output_rows);
            u32 get_table_statis(u32 table_id, TableEstInfo_s &table_statis);
            u32 get_column_statis(u32 table_id, u32 column_id, ColumnEstInfo_s &column_statis);
            UnorderedHashMap<u32, TableEstInfo_s> tables_statis;

            static EstInfo_s make_est_info() { return EstInfo_s(new EstInfo()); }
        };
    }
}

#endif	//OPT_EST_INFO_H
