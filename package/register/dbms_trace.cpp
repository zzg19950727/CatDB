#include "dbms_trace.h"
#include "session_info.h"
#include "obj_varchar.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
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

u32 DBMS_TRACE::set_log_level(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String var_value = params[0]->to_string();
    str_to_lower(var_value);
    int level = LOG_LEVEL_ERR;
    if (var_value == "trace") {
        level = LOG_LEVEL_TRACE;
    } else if (var_value == "info") {
        level = LOG_LEVEL_INFO;
    } else if (var_value == "error") {
        level = LOG_LEVEL_ERR;
    } else {
        ret = ERR_UNEXPECTED;
        String msg = "unknown log level " + var_value;
        QUERY_CTX->set_error_msg(msg);
        return ret;
    }
    SESSION_CTX->set_session_log_level(level);
    return ret;
}

u32 DBMS_TRACE::get_log_level(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    int level = SESSION_CTX->get_session_log_level();
    String log_level;
    if (LOG_LEVEL_ERR == level) {
        log_level = "ERROR";
    } else if (LOG_LEVEL_INFO == level) {
        log_level = "INFO";
    } else if (LOG_LEVEL_TRACE == level) {
        log_level = "TRACE";
    }
    CHECK(add_result(log_level, result));
    return ret;
}

u32 DBMS_TRACE::set_log_module(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 1);
    String var_value = params[0]->to_string();
    SESSION_CTX->set_session_log_module(var_value);
    return ret;
}

u32 DBMS_TRACE::get_log_module(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    CHECK(add_result(SESSION_CTX->get_session_log_module(), result));
    return ret;
}

u32 DBMS_TRACE::get_last_trace_id(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    CHECK(add_result(SESSION_CTX->get_last_trace_id(), result));
    return ret;
}
