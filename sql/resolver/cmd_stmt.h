#ifndef CMD_STMT_H
#define CMD_STMT_H
#include "object.h"
#include "error.h"
#include "stmt.h"
#include "type.h"
#include "log.h"

namespace CatDB {
	namespace Parser {
        DECLARE(ColumnDefineStmt);
        DECLARE(ExprStmt);
        DECLARE(BasicTableStmt);
		DECLARE(Stmt);

        class ColumnDefineStmt
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
			static ColumnDefineStmt_s make_column_define_stmt(const String& name, u32 data_type);
            KV_STRING(
                K(column_name),
                K(data_type)
            );

		public:
			String column_name;
			u32 data_type;
		};

        DECLARE(CMDStmt);
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
				Analyze,
                SetVar,
                ShowProcesslist,
                Kill
			};
			~CMDStmt();
			StmtType stmt_type()const;
			static Stmt_s make_cmd_stmt(CMDType cmd_type);
            u32 get_create_table_params(String &database, String &table, Vector<Pair<String, String>> &columns);
            u32 get_drop_table_params(String &database, String &table, bool &ignore_not_exists);
            u32 get_create_database_params(String &database);
            u32 get_drop_database_params(String &database, bool &ignore_not_exists);
            u32 get_show_tables_params(String &database);
            u32 get_show_databases_params(bool &is_select_current_database);
            u32 get_desc_table_params(String &database, String &table, bool &is_show_table_statis, bool &is_show_column_statis);
            u32 get_use_database_params(String &database);
            u32 get_analyze_params(String &database, String &table, double &sample_size);
            u32 get_set_var_params(String &var_name, String &var_value);
            u32 get_kill_params(int &thread_id);
            String get_cmd_type()const;
            u32 formalize();
		public:
            struct CMDParams{
                //for create table
                struct {
                    BasicTableStmt_s table;
			        Vector<ColumnDefineStmt_s> column_define_list;
                    KV_STRING(
                        K(table),
                        K(column_define_list)
                    );
                } create_table_params;
                //for drop table
                struct {
                    BasicTableStmt_s table;
                    bool ignore_not_exists;
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
                    bool ignore_not_exists;
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
                    bool is_select_current_database = false;
                    KV_STRING(
                        K(is_select_current_database)
                    );
                } show_databases_params;
                //for desc table
                struct {
                    BasicTableStmt_s table;
                    bool is_show_table_statis = false;
                    bool is_show_column_statis = false;
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
                    double sample_size;
                    KV_STRING(
                        K(database),
                        K(table),
                        K(sample_size)
                    );
                } analyze_params;
                //for set_var
                struct {
                    String var_name;
			        String var_value;
                    KV_STRING(
                        K(var_name),
                        K(var_value)
                    );
                } set_var_params;
                //kill thread_id
                struct {
                    int thread_id;
                    KV_STRING(
                        K(thread_id)
                    );
                } kill_params;

                CMDParams() { }
                ~CMDParams() { }
                KV_STRING(
                    V("NONE")
                );

            } params;
            CMDType cmd_type;

            KV_STRING(
                KV(cmd_type, get_cmd_type()),
                K(params)
            );
		};
	}
}

#endif	//CMD_STMT_H
