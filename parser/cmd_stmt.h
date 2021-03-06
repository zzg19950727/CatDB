#ifndef CMD_STMT_H
#define CMD_STMT_H
#include "object.h"
#include "stmt.h"
#include "type.h"
#include "log.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

        class ColumnDefineStmt : public Stmt
		{
		public:
			enum DataType {
				NUMBER = 0,
				DATETIME,
				VARCHAR,
				INT
			};
		private:
			ColumnDefineStmt();
		public:
			~ColumnDefineStmt();
			StmtType stmt_type()const;
			static Stmt_s make_column_define_stmt(const String& name, u32 data_type);
            KV_STRING(
                K(column_name),
                K(data_type)
            );

		public:
			String column_name;
			u32 data_type;
		};

		class CMDStmt : public Stmt
		{
		private:
			CMDStmt();
		public:
        enum CMDType {
                NONE = 0,
				CreateTable,
				DropTable,
				CreateDatabase,
				DropDatabase,
				ShowTables,
				ShowDatabases,
				DescTable,
				UseDatabase,
				Analyze
			};
			~CMDStmt();
			StmtType stmt_type()const;
			static Stmt_s make_cmd_stmt(CMDType cmd_type);
            u32 get_create_table_params(String &database, String &table, Vector<Pair<String, String>> &columns);
            u32 get_drop_table_params(String &database, String &table);
            u32 get_create_database_params(String &database);
            u32 get_drop_database_params(String &database);
            u32 get_show_tables_params(String &database);
            u32 get_show_databases_params(bool &is_select_current_database);
            u32 get_desc_table_params(String &database, String &table, bool &is_show_table_statis, bool &is_show_column_statis);
            u32 get_use_database_params(String &database);
            u32 get_analyze_params(String &database, String &table);
            String get_cmd_type()const;
		public:
            struct CMDParams{
                //for create table
                struct {
                    Stmt_s table;
			        Stmt_s column_define_list;
                    KV_STRING(
                        K(table),
                        K(column_define_list)
                    );
                } create_table_params;
                //for drop table
                struct {
                    Stmt_s table;
                    KV_STRING(
                        K(table)
                    );
                } drop_table_params;
                //for create database
                struct {
                    String database;
                    KV_STRING(
                        K(database)
                    );
                } create_database_params;
                //for drop database
                struct {
                    String database;
                    KV_STRING(
                        K(database)
                    );
                } drop_database_params;
                //for show tables
                struct {
                    String database;
                    KV_STRING(
                        K(database)
                    );
                } show_tables_params;  
                //for show databases
                struct {
                    bool is_select_current_database;
                    KV_STRING(
                        K(is_select_current_database)
                    );
                } show_databases_params;
                //for desc table
                struct {
                    Stmt_s table;
                    bool is_show_table_statis;
                    bool is_show_column_statis;
                    KV_STRING(
                        K(table),
                        K(is_show_table_statis),
                        K(is_show_column_statis)
                    );
                } desc_table_params;
                //for use database
                struct {
                    String database;
                    KV_STRING(
                        K(database)
                    );
                } use_database_params;
                //for analyze
                struct {
                    String database;
			        String table;
                    KV_STRING(
                        K(database),
                        K(table)
                    );
                } analyze_params;
                CMDParams() { }
                ~CMDParams() { }
                KV_STRING(
                    K(create_table_params),
                    K(drop_table_params),
                    K(create_database_params),
                    K(drop_database_params),
                    K(show_tables_params),
                    K(show_databases_params),
                    K(desc_table_params),
                    K(use_database_params),
                    K(analyze_params)
                );

            } params;
            CMDType cmd_type;

            KV_STRING(
                K(get_cmd_type()),
                K(params)
            );
		};
	}
}

#endif	//CMD_STMT_H