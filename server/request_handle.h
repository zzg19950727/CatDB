#ifndef REQUEST_HANDLE_H
#define REQUEST_HANDLE_H
#include "socket_buffer.h"
#include "net_service.h"
#include "loginer.h"
#include "packet.h"
#include <memory>
#include <string>
namespace CatDB {
	namespace Common {
		DECLARE(Buffer);
	}
	namespace Sql {
		DECLARE(ResultSet);
		DECLARE(QueryCtx);
	}
	namespace Server {
		using Sql::ResultSet_s;
		using Sql::QueryCtx_s;
		class ServerService;
		DECLARE(RequestHandle);
		class RequestHandle
		{
		public:
			RequestHandle(int fd, ServerService& service);
			~RequestHandle();
			void set_delete_handle(RequestHandle_s& self);
			void set_login_info(const Loginer::LoginInfo& info, int thread_id);
			QueryCtx_s &get_query_ctx() { return query_ctx; }
			const String &get_current_query() const { return cur_query; }

		private:
			void notify_socket(int fd, NetService::Event e);
			void read_socket(int fd);
			void write_socket(int fd);
			void close_connection();
			u32 post_packet(Packet& pack, uint8_t seq);
			u32 send_ok_packet(u32 affected_rows = 0);
			u32 send_error_packet(u32 err_code, const String& msg);
			u32 send_result_set(const ResultSet_s& result_set);
			u32 process_resheader_packet(Common::Buffer_s& buff, int64_t &buff_pos, const ResultSet_s& result_set);
			u32 process_field_packets(Common::Buffer_s& buff, int64_t &buff_pos, const ResultSet_s& result_set);
			u32 process_eof_packets(Common::Buffer_s& buff, int64_t &buff_pos, const ResultSet_s& result_set);
			u32 process_row_packets(Common::Buffer_s& buff, int64_t &buff_pos, const ResultSet_s& result_set);
			u32 process_single_packet(Common::Buffer_s& buff, int64_t &buff_pos, Packet& packet);

			u32 do_not_support();
			u32 do_cmd_query(const String& query);
			u32 send_explain_info(String& explain_info);
			void handle_request(char* buf, size_t len);
			void load_tpch_data();

			RequestHandle_s m_self;//delete self when no connection
			ServerService& m_server_service;
			BufferCache m_read_cache;
			BufferCache m_write_cache;
			Loginer::LoginInfo login_info;
			QueryCtx_s query_ctx;
			String cur_query;
			bool is_com_field_list;
			int seq;
			int m_fd;
		};
	}
}

#endif	//REQUEST_HANDLE_H
