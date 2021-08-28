#ifndef REQUEST_HANDLE_H
#define REQUEST_HANDLE_H
#include "socket_buffer.h"
#include "net_service.h"
#include "query_ctx.h"
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
	}
	namespace Server {
		using Sql::ResultSet_s;
		using Sql::QueryCtx;
		class ServerService;
		class RequestHandle
		{
		public:
			RequestHandle(int fd, ServerService& service);
			~RequestHandle();
			void set_delete_handle(std::shared_ptr<RequestHandle>& self);
			void set_login_info(const Loginer::LoginInfo& info);

		private:
			void notify_socket(int fd, NetService::Event e);
			void read_socket(int fd);
			void write_socket(int fd);
			void close_connection();
			u32 post_packet(Packet& pack, uint8_t seq);
			u32 send_ok_packet();
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
			void worker_caller(const std::string& func, std::shared_ptr<char> ptr, size_t len);
			void load_tpch_data();

			std::shared_ptr<RequestHandle> m_self;//delete self when no connection
			ServerService& m_server_service;
			BufferCache m_read_cache;
			BufferCache m_write_cache;
			Loginer::LoginInfo login_info;
			QueryCtx query_ctx;
			bool is_com_field_list;
			int seq;
			int m_fd;
		};
	}
}

#endif	//REQUEST_HANDLE_H
