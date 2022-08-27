#ifndef SERVER_SERVICE_H
#define SERVER_SERVICE_H

#include "thread_pool.hpp"
#include "net_service.h"
#include "config.h"
#include "type.h"

namespace CatDB {
	namespace Server {
		DECLARE(SessionInfo);
		DECLARE(RequestHandle);
		class ServerService
		{
		public:
			ServerService();
			~ServerService();
			int init(const String& config);
			int run();
			void close_connection();

		private:
			void new_connection(int fd, NetService::Event e);
			void do_login(int fd);
			void close_connection(int fd);
			void kill_all_process();

			NetService& net_service();
			ThreadPool& workers();

			NetService m_net_service;
			ThreadPool m_workers;
			
			int m_fd;
			int m_clients;
			int m_session_id;

		friend class RequestHandle;
		};
	}
}
#endif	//SERVER_SERVICE_H

