#ifndef CONFIG_READ_H
#define CONFIG_READ_H
#include "type.h"
namespace CatDB {
	namespace Server {
		class Config
		{
		public:
			Config(const char* path);
			~Config();
			String value(const String& key)const;

		private:
			void read_file(const char* path);

			HashMap<String, String> m_keys;
		};
	}
}
#endif	//CONFIG_READ_H
