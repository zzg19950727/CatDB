#ifndef SERVER_SERVICE_H
#define SERVER_SERVICE_H

#include "thread_pool.hpp"
#include "net_service.h"
#include "config.h"
#include "type.h"

namespace CatDB {
	namespace Server {
		class RequestHandle;
		class ServerService
		{
		public:
			ServerService(const String& config);
			~ServerService();
			int run();
			void close_connection();

		private:
			void new_connection(int fd, NetService::Event e);
			void do_login(int fd);
			void close_connection(int fd);

			NetService& net_service();
			ThreadPool& workers();
			ServerServiceConfig& config();

			ServerServiceConfig m_config;
			NetService m_net_service;
			ThreadPool m_workers;
			
			int m_fd;
			int m_clients;
			int m_thread_id;

			friend class RequestHandle;
		};
	}
}
#endif	//SERVER_SERVICE_H

