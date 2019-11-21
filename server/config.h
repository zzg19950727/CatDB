#ifndef CONFIG_H
#define CONFIG_H
#include "config_read.h"
#include "type.h"
namespace CatDB {
	namespace Server {
		class ServerServiceConfig
		{
		public:
			ServerServiceConfig(const char* path);
			~ServerServiceConfig();
			String ip()const;
			int port()const;
			int max_client_count()const;
			int worker_count()const;
			int epoll_workers()const;
			int cache_size()const;
		private:
			Config m_config;
		};
	}
}

#endif	//CONFIG_H
