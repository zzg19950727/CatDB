#ifndef CONFIG_READ_H
#define CONFIG_READ_H
#include "type.h"
namespace CatDB {
	namespace Server {
		class ConfigReader
		{
		public:
			void read_file(const char* path, HashMap<String, String> &keys);
		private:
			bool parse_key_value(const String& str, String& key, String& value);
		};
	}
}
#endif	//CONFIG_READ_H
