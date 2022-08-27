#ifndef SCHEMA_GUARD_H
#define SCHEMA_GUARD_H
#include "log.h"
#include "type.h"

namespace CatDB {
    namespace Sql {
		DECLARE(ResultSet);
	}
		
	namespace Parser {
        DECLARE(SchemaGuard);
        DECLARE(ColumnInfo);
        DECLARE(TableInfo);
        DECLARE(DatabaseInfo);
        DECLARE(ColumnDefineStmt);
        using Sql::ResultSet_s;
        using Common::DataType;

        class ColumnInfo {
            ColumnInfo()
            :table_id(INVALID_ID),
            column_id(INVALID_ID)
            {}

        public:
            u32 table_id;
            u32 column_id;
            String column_name;
            DataType column_type;

            KV_STRING(
                K(table_id),
                K(column_id),
                K(column_name),
                K(column_type)
            );
            static ColumnInfo_s make_column_info()
            { return ColumnInfo_s(new ColumnInfo()); }
        private:
			DISALLOW_COPY_AND_ASSIGN(ColumnInfo);
        };

        class TableInfo {
            TableInfo()
            :db_id(INVALID_ID),
            table_id(INVALID_ID),
            type(USER_PHY_TABLE)
            {}
        public:
            void set_table_type(int value);
            u32 db_id;
            u32 table_id;
            PhyTableType type;
            String table_name;
            Vector<String> engine_args;
            UnorderedHashMap<String, ColumnInfo_s> name_column_infos;
            UnorderedHashMap<u32, ColumnInfo_s> id_column_infos;

            KV_STRING(
                K(db_id),
                K(table_id),
                K(table_name),
                K(name_column_infos),
                K(id_column_infos)
            );
            static TableInfo_s make_table_info()
            { return TableInfo_s(new TableInfo()); }
        private:
			DISALLOW_COPY_AND_ASSIGN(TableInfo);
        };

        class DatabaseInfo {
            DatabaseInfo()
            :db_id(INVALID_ID)
            {}
        public:
            u32 db_id;
            String db_name;
            UnorderedHashMap<String, TableInfo_s> name_table_infos;
            UnorderedHashMap<u32, TableInfo_s> id_table_infos;

            KV_STRING(
                K(db_id),
                K(db_name),
                K(name_table_infos),
                K(id_table_infos)
            );
            static DatabaseInfo_s make_database_info()
            { return DatabaseInfo_s(new DatabaseInfo()); }
        private:
            DISALLOW_COPY_AND_ASSIGN(DatabaseInfo);
        };

		class SchemaGuard
		{
        private:
            SchemaGuard() 
            { }
		public:
            u32 init_guard();
			static SchemaGuard_s& get_schema_guard();
            u32 find_table_info(const String& db_name, const String& table_name, TableInfo_s &table_info);
            u32 find_table_info(u32 table_id, TableInfo_s &table_info);
            u32 find_column_info(u32 table_id, const String& column_name, ColumnInfo_s &column_info);
            u32 find_column_info(u32 table_id, u32 column_id, ColumnInfo_s &column_info);
            u32 find_database_info(const String& db_name, DatabaseInfo_s &db_info);
            u32 get_all_database(Vector<String> &databases);
            u32 get_all_table(const String& db_name, Vector<String> &tables);
            u32 add_database(const String& name);
            u32 del_database(const String& name);
            u32 add_table(const String& db_name,
                          const String& table_name,
                          PhyTableType type,
                          const Vector<ColumnDefineStmt_s> &columns,
                          const Vector<String> &engine_args);
            u32 del_table(const String &db_name, const String& table_name);

        private:
            u32 execute_sys_sql(const String& sql, ResultSet_s &result);
            u32 init_database_info();
            u32 init_table_info(Vector<TableInfo_s> &table_info);
            u32 init_column_info(Vector<ColumnInfo_s> &column_info);
            u32 init_system_schema(const Vector<ColumnDefineStmt_s> &SYS_DB_DEF,
                                   const Vector<ColumnDefineStmt_s> &SYS_TABLE_DEF,
                                   const Vector<ColumnDefineStmt_s> &SYS_COLUMN_DEF,
                                   const Vector<String> &SYS_ENGINE_ARGS);
            u32 add_database_to_cache(u32 id, const String& name);
            u32 add_database_to_inner_table(u32 id, const String& name);
            u32 add_table_to_cache(u32 db_id,
                                   u32 table_id,
                                   PhyTableType type,
                                   const String& name,
                                   const Vector<ColumnDefineStmt_s> &columns,
                                   const Vector<String> &engine_args);
            u32 add_table_to_inner_table(u32 db_id,
                                        u32 table_id,
                                        PhyTableType type,
                                        const String& name,
                                        const Vector<ColumnDefineStmt_s> &columns,
                                        const Vector<String> &engine_args);
            u32 del_database_from_cache(u32 id, const String& name);
            u32 del_database_from_inner_table(u32 id);
            u32 del_table_from_cache(u32 db_id,
                                   u32 table_id,
                                   const String& name);
            u32 del_table_from_inner_table(u32 db_id, u32 table_id);
            u32 generate_database_id(u32 &id);
            u32 generate_table_id(u32 db_id, u32 &table_id);


		private:
            UnorderedHashMap<String, DatabaseInfo_s> name_database_infos;
            UnorderedHashMap<u32, DatabaseInfo_s> id_database_infos;
            DISALLOW_COPY_AND_ASSIGN(SchemaGuard);
		};
	}
}

#endif	//SCHEMA_GUARD_H