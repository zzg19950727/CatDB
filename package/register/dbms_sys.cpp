#include "dbms_sys.h"
#include "package_manager.h"
#include "global_context.h"
#include "session_info.h"
#include "schema_guard.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Package;

#define str_to_lower(str) 					\
{											\
	for(u32 i = 0; i<str.size(); ++i){		\
		if(str[i] >= 'A' && str[i] <= 'Z'){	\
			str[i] -= 'A';					\
			str[i] += 'a';					\
		}									\
	}										\
}

u32 DBMS_SYS::set_sys_parameter(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
    String name = params[0]->to_string();
    String value = params[1]->to_string();
    if (!SYS_PARAMS.has_key(name)) {
        ret = ERR_UNEXPECTED;
        String msg = "unknown var: " + name;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    SYS_PARAMS.set_value(name, value);
    return ret;
}

u32 DBMS_SYS::get_sys_parameter(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String name = params[0]->to_string();
    Vector<Pair<String, String>> sys_params;
    SYS_PARAMS.search_key(name, sys_params);
    for (u32 i = 0; i < sys_params.size(); ++i) {
        CHECK(add_result(sys_params[i].first, 
                         sys_params[i].second, 
                         result));
    }
    return ret;
}

u32 DBMS_SYS::list_sys_parameters(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    HashMap<String, String>& all_config = SYS_PARAMS.get_all_config();
    for (auto iter = all_config.cbegin(); iter != all_config.cend(); ++iter) {
        CHECK(add_result(iter->first, iter->second, result));
    }
    return ret;
}

u32 DBMS_SYS::list_sys_config(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    HashMap<String, String>& all_config = SYS_CONF.get_all_config();
    for (auto iter = all_config.cbegin(); iter != all_config.cend(); ++iter) {
        CHECK(add_result(iter->first, iter->second, result));
    }
    return ret;
}

u32 DBMS_SYS::set_session_parameter(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
    String name = params[0]->to_string();
    String value = params[1]->to_string();
    if (!SESSION_PARAMS.has_key(name)) {
        ret = ERR_UNEXPECTED;
        String msg = "unknown var: " + name;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    SESSION_PARAMS.set_value(name, value);
    CHECK(SESSION_CTX->refresh_parameters());
    return ret;
}

u32 DBMS_SYS::get_session_parameter(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String name = params[0]->to_string();
    Vector<Pair<String, String>> session_params;
    SESSION_PARAMS.search_key(name, session_params);
    for (u32 i = 0; i < session_params.size(); ++i) {
        CHECK(add_result(session_params[i].first, 
                         session_params[i].second, 
                         result));
    }
    return ret;
}

u32 DBMS_SYS::list_session_parameters(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    HashMap<String, String>& all_config = SESSION_PARAMS.get_all_config();
    for (auto iter = all_config.cbegin(); iter != all_config.cend(); ++iter) {
        CHECK(add_result(iter->first, iter->second, result));
    }
    return ret;
}

u32 DBMS_SYS::list_database(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
	Vector<String> databases;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->get_all_database(databases));
	for (u32 i = 0; i < databases.size(); ++i) {
		CHECK(add_result(databases[i], result));
	}
	return ret;
}

u32 DBMS_SYS::list_table(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String database = params[0]->to_string();
    Vector<String> tables;
	
	SchemaGuard_s &guard = SchemaGuard::get_schema_guard();
	MY_ASSERT(guard);
	CHECK(guard->get_all_table(database, tables));
    
	for (u32 i = 0; i < tables.size(); ++i) {
		CHECK(add_result(tables[i], result));
	}
	return ret;
}

u32 DBMS_SYS::list_package(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    Vector<String> packages;
    PackageManager_s &manager = PackageManager::get_package_manager();
    CHECK(manager->get_all_package_name(packages));
    for (u32 i = 0; i < packages.size(); ++i) {
		CHECK(add_result(packages[i], result));
	}
    return ret;
}