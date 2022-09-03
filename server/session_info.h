#ifndef SESSION_INFO_H
#define SESSION_INFO_H
#include "timer_manager.h"
#include "query_ctx.h"
#include "config.h"
#include "type.h"
#define SESSION_CTX (get_cur_thread_session_info().session_info)
#define SET_SESSION_CTX(session_info) get_cur_thread_session_info().session_info = session_info
#define SESSION_PARAMS (SESSION_CTX->get_session_parameters())
#define QUERY_CTX   (SESSION_CTX->get_query_ctx())

namespace CatDB {
	namespace Server {
        DECLARE(QueryCtx);
        DECLARE(SessionInfo);

        class SessionInfo {
        public:
            SessionInfo();
            virtual ~SessionInfo() {}
            static SessionInfo_s make_session_info();
            static SessionInfo_s make_session_info(ConfigService &sys_params);
            void reset();

			inline QueryCtx_s &get_query_ctx() { return query_ctx; }
            u32 check_query_status();

            void begin_query(SessionStatus status, const String& sql);
            void end_query();

            void set_cur_database(const String& db);
            const String& get_cur_database() const;

            void set_query_timeout(long long msec);
            long long get_query_timeout() const;

            void set_session_id(u32 session_id);
            u32 get_session_id() const;

            void set_session_log_level(int level);
            inline int get_session_log_level() const { return log_level; }
            
            void set_session_log_module(const String& module);
            inline const String& get_session_log_module() const { return log_module; }

            inline const String& get_trace_id() const { return query_ctx->get_trace_id(); }
            inline const String& get_last_trace_id() const { return query_ctx->last_trace_id; }
            String get_err_msg(int ret) const;

            long long get_query_time() const;
            String get_session_status() const;
            const String& get_query_sql() const;

            void kill_query() { killed = true; }
            void set_root_session() { is_root_session = true; }
            
            ConfigService& get_session_parameters();
            u32 refresh_parameters();

        private:
			QueryCtx_s query_ctx;
			ConfigService session_parameters;
			SessionStatus session_status;
            String cur_database;
            String query_sql;
            //query start time
			DateTime::RepMs query_start_time;
            DateTime::RepMs query_timeout;
            u32 session_id;
            int log_level;
            String log_module;
            //kill query
            bool killed;
            bool is_root_session;
        };
    }
}
struct SessionInfoHelper {
    SessionInfoHelper()
    {
        session_info = CatDB::Server::SessionInfo::make_session_info();
    }
    CatDB::Server::SessionInfo_s session_info;
};
SessionInfoHelper& get_cur_thread_session_info();
#endif //SESSION_INFO_H