#ifndef STATIS_MANAGER_H
#define STATIS_MANAGER_H
#include "type.h"

namespace CatDB {
    namespace Common {
		DECLARE(QueryResult);
	}
	namespace Optimizer {
        DECLARE(ColumnStatis);
        DECLARE(TableStatis);
        DECLARE(StatisManager);
        using Common::QueryResult_s;
        
        class ColumnStatis {
        private:
            DISALLOW_COPY_AND_ASSIGN(ColumnStatis);
            ColumnStatis()
            :tid(INVALID_ID),
            cid(INVALID_ID),
            ndv(0),
            null_count(0),
            max_value(0),
            min_value(0)
            {}
        public:
            u32 tid;
            u32 cid;
            u32 ndv;
            u32 null_count;
            u32 max_value;
            u32 min_value;

            static ColumnStatis_s make_column_statis()
            { return ColumnStatis_s(new ColumnStatis()); }
        };

        struct TableStatis {
        private:
            DISALLOW_COPY_AND_ASSIGN(TableStatis);
            TableStatis()
            :tid(INVALID_ID),
            row_count(0),
            space_size(0)
            {}
        public:
            u32 tid;
            u32 row_count;
            u32 space_size;
            UnorderedHashMap<u32, ColumnStatis_s> column_statis;

            static TableStatis_s make_table_statis()
            { return TableStatis_s(new TableStatis()); }
        };

        class StatisManager {
        private:
            DISALLOW_COPY_AND_ASSIGN(StatisManager);
            UnorderedHashMap<u32, TableStatis_s> all_table_statis;
            StatisManager()
            { }
        public:
            u32 init_statis_cache();
            static StatisManager_s make_statis_manager();
            u32 analyze_table(const String& database, const String& table, double sample_size=0.1);
			double get_table_row_count(u32 tid);
            double get_avg_row_size(u32 tid);
            double get_table_block_count(u32 tid);
			double get_column_ndv(u32 tid, u32 cid);
			double get_column_max_value(u32 tid, u32 cid);
			double get_column_min_value(u32 tid, u32 cid);
			double get_column_null_count(u32 tid, u32 cid);
            u32 delete_table_statis(u32 tid);
        protected:
            u32 execute_sys_sql(const String& sql, QueryResult_s &result, double sample_size=1);
            u32 init_table_statis(Vector<TableStatis_s> &table_statis);
            u32 init_column_statis(Vector<ColumnStatis_s> &column_statis);
            u32 inner_analyze_table(const String& database, const String& table, double sample_size);
            u32 generate_analyze_sql(const String& database,
                                     const String& table,
                                     String &query,
                                     u32 &table_id,
                                     Vector<u32> &column_ids);
            u32 generate_table_statis(u32 table_id,
                                      u32 space_size,
                                      double sample_size,
                                      Vector<u32> &column_ids,
                                      QueryResult_s result,
                                      TableStatis_s &table_statis);  
            u32 generate_load_sql(TableStatis_s &statis, String &table_statis_query, String &column_statis_query);                      
        };
    }
}

#endif  //STATIS_MANAGER_H