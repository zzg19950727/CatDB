#ifndef CMD_STMT_H
#define CMD_STMT_H
#include "object.h"
#include "error.h"
#include "select_stmt.h"
#include "type.h"
#include "log.h"

namespace CatDB {
	namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(SelectStmt);
        DECLARE(BasicTableStmt);
		DECLARE(Stmt);
		using Common::DataType;

        DECLARE(CMDParam);
        class CMDParam {
        public:
            VIRTUAL_KV_STRING("");
        };

        DECLARE(FunctionDefinition);
        class FunctionDefinition {
        public:
            static FunctionDefinition_s make_func_define();

            KV_STRING(
                K(name),
                K(param_list),
                K(return_type_list)
            );

            String name;
            Vector<ColumnDefineStmt_s> param_list;
            Vector<ColumnDefineStmt_s> return_type_list;
        };

        DECLARE(CreatePackageParam);
        class CreatePackageParam : public CMDParam {
        public:
            static CreatePackageParam_s make_create_package_param();

            VIRTUAL_KV_STRING_OVERRIDE(
                K(name),
                K(is_replace),
                K(functions)
            );

            String name;
            bool is_replace;
            Vector<FunctionDefinition_s> functions;
        };

        DECLARE(ExecFunctionParam);
        class ExecFunctionParam : public CMDParam {
        public:
            static ExecFunctionParam_s make_exec_func_param();

            VIRTUAL_KV_STRING_OVERRIDE(
                K(package_name),
                K(function_name),
                K(param_list)
            );

            String package_name;
            String function_name;
            Vector<ExprStmt_s> param_list;
        };

        DECLARE(CMDStmt);
		class CMDStmt : public Stmt
		{
		private:
			CMDStmt();
		public:
			~CMDStmt();
			StmtType stmt_type()const override;
			static Stmt_s make_cmd_stmt(CMDType cmd_type);
            u32 get_create_table_params(String &database, 
                                        String &table, 
                                        Vector<ColumnDefineStmt_s> &columns,
                                        Vector<String> &engine_args);
            u32 get_drop_table_params(String &database, String &table, bool &ignore_not_exists);
            u32 get_create_database_params(String &database);
            u32 get_drop_database_params(String &database, bool &ignore_not_exists);
            u32 get_show_tables_params(String &database);
            u32 get_show_databases_params(bool &is_select_current_database);
            u32 get_desc_table_params(String &database, 
                                      String &table, 
                                      bool &is_show_table_statis, 
                                      bool &is_show_column_statis);
            u32 get_use_database_params(String &database);
            u32 get_analyze_params(String &database, String &table, double &sample_size);
            u32 get_kill_params(int &session_id);
            u32 get_create_view_params(String &database,
                                    String &view_name,
                                    Vector<String> &column_define,
                                    String &view_define_sql,
                                    SelectStmt_s &ref_query);
            u32 get_drop_view_params(String &database,
                                     String &view_name, 
                                     bool &ignore_not_exists);
            u32 formalize() override;
		public:
            struct CMDParams{
                //for create table
                struct {
                    BasicTableStmt_s table;
			        Vector<ColumnDefineStmt_s> column_define_list;
                    Vector<String> engine_args;
                    KV_STRING(
                        K(table),
                        K(column_define_list),
                        K(engine_args)
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
                //kill session_id
                struct {
                    int session_id;
                    KV_STRING(
                        K(session_id)
                    );
                } kill_params;
                //create view
                struct {
                    String database;
                    String view_name;
                    Vector<String> column_define;
                    String view_define_sql;
                    SelectStmt_s ref_query;
                    KV_STRING(
                        K(database),
                        K(view_name),
                        K(column_define),
                        K(view_define_sql)
                    );
                } create_view_params;
                //drop view
                struct {
                    String database;
                    String view_name;
                    bool ignore_not_exists;
                    KV_STRING(
                        K(database),
                        K(view_name),
                        K(ignore_not_exists)
                    );
                } drop_view_params;

                CMDParams() { }
                ~CMDParams() { }
                KV_STRING(
                    V("NONE")
                );

            } params;
            CMDType cmd_type;
            CMDParam_s param;

            KV_STRING_OVERRIDE(
                KV(cmd_type, CMDTypeString[cmd_type]),
                K(param)
            );
		};
	}
}

#endif	//CMD_STMT_H
