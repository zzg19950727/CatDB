#include "global_context.h"

using namespace CatDB::Server;

GlobalCtx::GlobalCtx()
{


}
GlobalCtx::~GlobalCtx()
{

}

GlobalCtx_s &GlobalCtx::get_global_ctx()
{
    static GlobalCtx_s ctx(new GlobalCtx);
    return ctx;
}

int GlobalCtx::init_config(const char* path)
{
    return config_service.init(path);
}

ConfigService& GlobalCtx::config()
{
    return config_service;
}

HashMap<int, RequestHandle_s>& GlobalCtx::get_all_processlist()
{
    return m_processlist;
}

void GlobalCtx::add_process(int session_id, RequestHandle_s &handle)
{
    m_processlist[session_id] = handle;
}

void GlobalCtx::remove_process(int session_id)
{
    m_processlist.erase(session_id);
}

void GlobalCtx::get_session_info(int session_id, SessionInfo_s &info)
{
    if (m_processlist.find(session_id) != m_processlist.cend()) {
		info = m_processlist[session_id]->get_session_info();
	} else {
        info.reset();
    }
}