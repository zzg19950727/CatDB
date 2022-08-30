#ifndef CMD_STMT_H
#define CMD_STMT_H
#include "select_stmt.h"
#include "expr_stmt.h"
#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		using Common::DataType;

        DECLARE(CMDParam);
        class CMDParam {
        public:
            VIRTUAL_KV_STRING("");
        };

        //for create table
        DECLARE(CreateTableParam);
        class CreateTableParam : public CMDParam {
        public:
            static CreateTableParam_s make_create_table_param();
            String database_name;
            String table_name;
            Vector<ColumnDefineStmt_s> column_define_list;
            Vector<String> engine_args;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database_name),
                K(table_name),
                K(column_define_list),
                K(engine_args)
            );
        };

        //for drop table
        DECLARE(DropTableParam);
        class DropTableParam : public CMDParam {
        public:
            static DropTableParam_s make_drop_table_param();
            String database_name;
            String table_name;
            bool ignore_not_exists;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database_name),
                K(table_name),
                K(ignore_not_exists)
            );
        };

        //for create database
        DECLARE(CreateDBParam);
        class CreateDBParam : public CMDParam {
        public:
            static CreateDBParam_s make_create_DB_param();
            String database;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database)
            );
        };

        //for drop database
        DECLARE(DropDBParam);
        class DropDBParam : public CMDParam {
        public:
            static DropDBParam_s make_drop_DB_param();
            String database;
            bool ignore_not_exists;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database),
                K(ignore_not_exists)
            );
        };

        //for show tables
        DECLARE(ShowTablesParam);
        class ShowTablesParam : public CMDParam {
        public:
            static ShowTablesParam_s make_show_tables_param();
            String database;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database)
            );
        };

        //for desc table
        DECLARE(DescTableParam);
        class DescTableParam : public CMDParam {
        public:
            static DescTableParam_s make_desc_table_param();
            String database_name;
            String table_name;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database_name),
                K(table_name)
            );
        };

        //for use database
        DECLARE(UseDBParam);
        class UseDBParam : public CMDParam {
        public:
            static UseDBParam_s make_use_DB_param();
            String database;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database)
            );
        };
        
        //kill session_id
        DECLARE(KillSessionParam);
        class KillSessionParam : public CMDParam {
        public:
            static KillSessionParam_s make_kill_session_param();
            int session_id;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(session_id)
            );
        };

        //create view
        DECLARE(CreateViewParam);
        class CreateViewParam : public CMDParam {
        public:
            static CreateViewParam_s make_create_view_param();
            String database;
            String view_name;
            Vector<String> column_define;
            String view_define_sql;
            SelectStmt_s ref_query;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database),
                K(view_name),
                K(column_define),
                K(view_define_sql)
            );
        };

        //drop view
        DECLARE(DropViewParam);
        class DropViewParam : public CMDParam {
        public:
            static DropViewParam_s make_drop_view_param();
            String database;
            String view_name;
            bool ignore_not_exists;
            VIRTUAL_KV_STRING_OVERRIDE(
                K(database),
                K(view_name),
                K(ignore_not_exists)
            );
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
            u32 formalize() override;

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
