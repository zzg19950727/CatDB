#include "dbms_sys.h"
#include "global_context.h"
#include "session_info.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Package;

u32 DBMS_SYS::set_sys_var(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
    String name = params[0]->to_string();
    String value = params[1]->to_string();
    if (!GTX->config().has_key(name)) {
        ret = ERR_UNEXPECTED;
        String msg = "unknown var: " + name;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    GTX->config().set_value(name, value);
    return ret;
}

u32 DBMS_SYS::get_sys_var(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String name = params[0]->to_string();
    if (!GTX->config().has_key(name)) {
        ret = ERR_UNEXPECTED;
        String msg = "unknown var: " + name;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    CHECK(add_result(GTX->config().value(name), result));
    return ret;
}

u32 DBMS_SYS::list_sys_var(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    HashMap<String, String>& all_config = GTX->config().get_all_config();
    for (auto iter = all_config.cbegin(); iter != all_config.cend(); ++iter) {
        CHECK(add_result(iter->first, iter->second, result));
    }
    return ret;
}

u32 DBMS_SYS::set_session_var(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
    String name = params[0]->to_string();
    String value = params[1]->to_string();
    if (!SESSION_CTX->config().has_key(name)) {
        ret = ERR_UNEXPECTED;
        String msg = "unknown var: " + name;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    SESSION_CTX->config().set_value(name, value);
    return ret;
}

u32 DBMS_SYS::get_session_var(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String name = params[0]->to_string();
    if (!SESSION_CTX->config().has_key(name)) {
        ret = ERR_UNEXPECTED;
        String msg = "unknown var: " + name;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    CHECK(add_result(SESSION_CTX->config().value(name), result));
    return ret;
}

u32 DBMS_SYS::list_session_var(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    HashMap<String, String>& all_config = SESSION_CTX->config().get_all_config();
    for (auto iter = all_config.cbegin(); iter != all_config.cend(); ++iter) {
        CHECK(add_result(iter->first, iter->second, result));
    }
    return ret;
}