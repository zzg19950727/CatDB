#ifndef GLOBAL_CONTEXT_H
#define GLOBAL_CONTEXT_H
#include "request_handle.h"
#include "session_info.h"
#include "config.h"
#include "type.h"
#define GTX (CatDB::Server::GlobalCtx::get_global_ctx())

namespace CatDB {
	namespace Server {
        DECLARE(GlobalCtx);
        DECLARE(SessionInfo);
        DECLARE(RequestHandle);

        class GlobalCtx {
        public:
            GlobalCtx();
            virtual ~GlobalCtx();
            static GlobalCtx_s& get_global_ctx();

            int init_config(const char* path);
            ConfigService& config();
            HashMap<int, RequestHandle_s>& get_all_processlist();
            void add_process(int session_id, RequestHandle_s &handle);
            void remove_process(int session_id);
            void get_session_info(int session_id, SessionInfo_s &info);

        private:
			ConfigService config_service;
            HashMap<int, RequestHandle_s> m_processlist;
        };
    }
}
#endif //GLOBAL_CONTEXT_H