#ifndef CONFIG_H
#define CONFIG_H
#include "type.h"
namespace CatDB {
	namespace Server {
		class ConfigService
		{
		public:
			ConfigService();
			~ConfigService();
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
			String value(const String& key)const;
			int init(const char* path);
			HashMap<String, String>& get_all_config();
			void set_value(const String& key, const String& value);
			void add_value(const String& key, const String& value);
			bool has_key(const String& key) const;
		private:
			HashMap<String, String> m_keys;
		};
	}
}

#endif	//CONFIG_H
