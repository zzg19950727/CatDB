#ifndef LOG_H
#define LOG_H
#include "print_helper.h"
#define LOG_LEVEL_ERR	0x1
#define LOG_LEVEL_WARN	0x2
#define LOG_LEVEL_TRACE	0x3

#define LOG_ERR(...)	Log(LOG_LEVEL_ERR, ::to_kv_string(__VA_ARGS__))
#define LOG_WARN(...)	Log(LOG_LEVEL_WARN, ::to_kv_string(__VA_ARGS__))
#define LOG_TRACE(...)	Log(LOG_LEVEL_TRACE, ::to_kv_string(__VA_ARGS__))

#define DEBUG
#ifdef DEBUG
#define Log(log_level, msg) \
		do{\
			CatDB::Common::log_output(log_level, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg); \
		} while (0);
#else
#define Log(log_level, msg) 
#endif	//DEBUG

namespace CatDB {
	namespace Common {
		void set_log_file(const char* file);
		void set_debug_level(int level);
		void log_output(int log_level, const char* file, int line, const char* function, const String& msg);
	}
}
#endif	//LOG_H
