#include "dbms_metadata.h"
#include "schema_guard.h"
#include "session_info.h"
#include "statis_manager.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Package;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;

u32 DBMS_METADATA::get_table_ddl(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
	String database = params[0]->to_string();
	String table = params[1]->to_string();
    
	SchemaGuard_s& guard = SchemaGuard::get_schema_guard();
    TableInfo_s table_info;
    CHECK(guard->find_table_info(database, table, table_info));
    MY_ASSERT(USER_VIEW_TABLE != table_info->type);
    Vector<ColumnInfo_s> columns;
    for (auto iter = table_info->id_column_infos.cbegin(); 
		 iter != table_info->id_column_infos.cend(); 
		 ++iter) {
		columns.push_back(iter->second);
	}
	std::sort(columns.begin(), 
			  columns.end(), 
			  [](const ColumnInfo_s& lhs, const ColumnInfo_s& rhs) 
              { return lhs->column_id < rhs->column_id; } );

    String ddl = "\nCREATE TABLE ";
    CHECK(print_ident(table_info->table_name, ddl));
    ddl += " (\n";
    for (u32 i = 0; i < columns.size(); ++i) {
        ColumnInfo_s &col = columns[i];
        ddl += "\t";
        CHECK(print_ident(col->column_name, ddl));
        ddl += " " + col->column_type.to_kv_string();
        if (i == columns.size() - 1) {
            ddl += "\n";
        } else {
            ddl += ",\n";
        }
    }
    ddl += ")\n";
    if (table_info->engine_args.size() == 4 && table_info->engine_args[0] == "CSV") {
        ddl += "ENGINE = " + table_info->engine_args[0] +
                ", INFILE = " + table_info->engine_args[1] + 
                ", FIELD SPLIT BY " + table_info->engine_args[2] +
                ", LINE SPLIT BY " + table_info->engine_args[3];
    }
    ddl += ";";
    CHECK(add_result(ddl, result));
	return ret;
}

u32 DBMS_METADATA::get_view_ddl(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
	String database = params[0]->to_string();
	String view = params[1]->to_string();
    
	SchemaGuard_s& guard = SchemaGuard::get_schema_guard();
    TableInfo_s table_info;
    CHECK(guard->find_table_info(database, view, table_info));
    MY_ASSERT(USER_VIEW_TABLE == table_info->type);
    String ddl = "\nCREATE VIEW ";
    CHECK(print_ident(table_info->table_name, ddl));
    ddl += " AS\n";
    MY_ASSERT(1 == table_info->engine_args.size());
    ddl += table_info->engine_args[0];
    ddl += ";";
    CHECK(add_result(ddl, result));
	return ret;
}

u32 DBMS_METADATA::get_package_ddl(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String package_name = params[0]->to_string();
	PackageManager_s &manager = PackageManager::get_package_manager();
    PackageManager::PackageInfo info;
    CHECK(manager->get_package_info(package_name, 
                                    info));
    String ddl = "\nCREATE OR REPLACE PACKAGE ";
    CHECK(print_ident(info.name, ddl));
    ddl += " IS\n\n";
    for (auto iter = info.funcs.cbegin(); iter != info.funcs.cend(); ++iter) {
        CHECK(print_function(iter->second, 
                             !iter->second.return_columns.empty(),
                             true,
                             ddl));
    }
    ddl += "END;";
    CHECK(add_result(ddl, result));
    return ret;
}

u32 DBMS_METADATA::get_function_ddl(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
    String package_name = params[0]->to_string();
    String function_name = params[1]->to_string();
	PackageManager_s &manager = PackageManager::get_package_manager();
    PackageManager::PackageFuncInfo info;
    CHECK(manager->get_func_info(package_name, 
                                 function_name, 
                                 info));
	String ddl;
    CHECK(print_function(info, true, false, ddl));
    CHECK(add_result(ddl, result));
    return ret;
}

u32 DBMS_METADATA::get_procedure_ddl(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
    String package_name = params[0]->to_string();
    String function_name = params[1]->to_string();
	PackageManager_s &manager = PackageManager::get_package_manager();
    PackageManager::PackageFuncInfo info;
    CHECK(manager->get_func_info(package_name, 
                                 function_name, 
                                 info));
	String ddl;
    CHECK(print_function(info, false, false, ddl));
    CHECK(add_result(ddl, result));
    return ret;
}

u32 DBMS_METADATA::print_ident(const String& ident, String &ddl)
{
    u32 ret= SUCCESS;
    ddl += "\"" + ident + "\"";
    return ret;
}

u32 DBMS_METADATA::print_function(const PackageManager::PackageFuncInfo &info, 
                                bool is_func, 
                                bool print_package, 
                                String &ddl)
{
    u32 ret = SUCCESS;
    if (print_package) {
        ddl += "\t";
    } else {
        ddl += '\n';
    }
    if (is_func) {
        ddl += "FUNCTION ";
    } else {
	    ddl += "PROCEDURE ";
    }
    CHECK(print_ident(info.name, ddl));
    ddl += " (\n";
    MY_ASSERT(info.param_names.size() == info.param_type_list.size());
    for (u32 i = 0; i < info.param_names.size(); ++i) {
        if (print_package) {
            ddl += "\t\t";
        } else {
            ddl += "\t";
        }
        CHECK(print_ident(info.param_names[i], ddl));
        ddl += " " + info.param_type_list[i].to_kv_string();
        if (i == info.param_names.size() - 1) {
            ddl += "\n";
        } else {
            ddl += ",\n";
        }
    }
    if (print_package) {
        ddl += "\t";
    }
    ddl += ")";
    if (info.return_columns.size() == 1 && 
        info.return_columns[0] == info.name) {
        MY_ASSERT(info.return_columns.size() == info.return_type_list.size());
        ddl += "RETURN " + info.return_type_list[0].to_kv_string();    
    } else if (info.return_columns.size() > 0) {
        MY_ASSERT(info.return_columns.size() == info.return_type_list.size());
        ddl += "RETURN TABLE(\n";
        for (u32 i = 0; i < info.return_columns.size(); ++i) {
            if (print_package) {
                ddl += "\t\t";
            } else {
                ddl += "\t";
            }
            CHECK(print_ident(info.return_columns[i], ddl));
            ddl += " " + info.return_columns[i];
            if (i == info.return_columns.size() - 1) {
                ddl += "\n";
            } else {
                ddl += ",\n";
            }
        }
        if (print_package) {
            ddl += "\t";
        }
        ddl += ")";
    } 
    ddl += ";";
    if (print_package) {
        ddl += "\n\n";
    }
    return ret;
}