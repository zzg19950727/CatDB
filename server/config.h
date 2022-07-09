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
			String data_dir()const;
			String recycle_dir()const;
			String log_file_path()const;
			int log_level()const;
			int cache_size()const;
			int thread_pool_size()const;
			String log_module() const;
			long long query_timeout() const;
		private:
			Config m_config;
		};
	}
}

#endif	//CONFIG_H
