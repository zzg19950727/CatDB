#include "session_info.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;
#define str_to_lower(str) 					\
{											\
	for(u32 i = 0; i<str.size(); ++i){		\
		if(str[i] >= 'A' && str[i] <= 'Z'){	\
			str[i] -= 'A';					\
			str[i] += 'a';					\
		}									\
	}										\
}

SessionInfo::SessionInfo()
{
    query_ctx = QueryCtx::make_query_ctx();
    reset();
}

SessionInfo_s SessionInfo::make_session_info()
{
    return SessionInfo_s(new SessionInfo);
}

SessionInfo_s SessionInfo::make_session_info(ConfigService &sys_params)
{
    SessionInfo_s info = SessionInfo_s(new SessionInfo);
    info->session_parameters = sys_params;
    info->refresh_parameters();
    return info;
}

void SessionInfo::reset()
{
    query_ctx->reset();
    session_status = IVALID;
    cur_database = "";
    query_start_time = 0;
    query_timeout = 1000000;
    session_id = 0;
    log_level = LOG_LEVEL_ERR;
    log_module = "ALL";
    killed = false;
    is_root_session = false;
    session_parameters.clear();
}

u32 SessionInfo::check_query_status()
{
    if (killed) {
        return QUERY_KILLED; 
    } else if (is_root_session) {
        return SUCCESS;
    } else if (query_timeout <= 
                DateTime::now_steady_miil() - 
                query_start_time) {
        return QUERY_TIMEOUT;
    } else {
        return SUCCESS; 
    }
}

void SessionInfo::begin_query(SessionStatus status, const String& sql)
{
    session_status = status;
    query_ctx->reset();
    query_sql = sql;
    killed = false;
    query_start_time = DateTime::now_steady_miil();
}

void SessionInfo::end_query()
{
    session_status = SLEEP;
}

void SessionInfo::set_cur_database(const String& db)
{
    cur_database = db;
}

const String& SessionInfo::get_cur_database() const
{
    return cur_database;
}

void SessionInfo::set_query_timeout(long long msec)
{
    query_timeout = msec;
}

long long SessionInfo::get_query_timeout() const
{
    return query_timeout;
}

void SessionInfo::set_session_id(u32 id)
{
    session_id = id;
}

u32 SessionInfo::get_session_id() const
{
    return session_id;
}

void SessionInfo::set_session_log_level(int level)
{
    log_level = level;
}

void SessionInfo::set_session_log_module(const String& module)
{
    log_module = module;
}

String SessionInfo::get_err_msg(int ret) const
{
    String err_msg = ErrCodeString[ret];
    err_msg += " " + query_ctx->get_error_msg();
    return err_msg;
}

long long SessionInfo::get_query_time() const
{
    return DateTime::now_steady_miil() - query_start_time;
}

String SessionInfo::get_session_status() const
{
    return String(SessionStatusString[session_status]);
}

const String& SessionInfo::get_query_sql() const
{
    return query_sql;
}

ConfigService& SessionInfo::get_session_parameters()
{
    return session_parameters;
}

u32 SessionInfo::refresh_parameters()
{
    u32 ret = SUCCESS;
    log_module = session_parameters.value("log_module");
    query_timeout = std::stoi(session_parameters.value("query_timeout"));
    String var_value = session_parameters.value("log_level");
    str_to_lower(var_value);
    if (var_value == "trace") {
        log_level = LOG_LEVEL_TRACE;
    } else if (var_value == "info") {
        log_level = LOG_LEVEL_INFO;
    } else if (var_value == "error") {
        log_level = LOG_LEVEL_ERR;
    }
    return ret;
}

SessionInfoHelper& get_cur_thread_session_info()
{
    thread_local SessionInfoHelper helper;
    return helper;
}