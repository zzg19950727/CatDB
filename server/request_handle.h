#ifndef REQUEST_HANDLE_H
#define REQUEST_HANDLE_H
#include "socket_buffer.h"
#include "net_service.h"
#include <memory>
#include <string>
namespace CatDB {
	namespace Server {
		class ServerService;
		class RequestHandle
		{
		public:
			RequestHandle(int fd, ServerService& service);
			~RequestHandle();
			void set_delete_handle(std::shared_ptr<RequestHandle>& self);

		private:
			void notify_socket(int fd, NetService::Event e);
			void read_socket(int fd);
			void write_socket(int fd);
			void close_connection();
			void handle_request(unsigned char* buf, size_t len);
			void worker_caller(const std::string& func, std::shared_ptr<char> ptr, size_t len);

			std::shared_ptr<RequestHandle> m_self;//delete self when no connection
			ServerService& m_server_service;
			BufferCache m_read_cache;
			BufferCache m_write_cache;
			int m_fd;
		};
	}
}

#endif	//REQUEST_HANDLE_H
