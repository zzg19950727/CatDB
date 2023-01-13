#ifndef LOG_H
#define LOG_H
#include "print_helper.h"
#define LOG_LEVEL_INFO	0x1
#define LOG_LEVEL_ERR	0x2
#define LOG_LEVEL_TRACE	0x3

#define LOG_INFO(...)	Log(LOG_LEVEL_INFO, ::to_kv_string(__VA_ARGS__))
#define LOG_ERR(...)	Log(LOG_LEVEL_ERR, ::to_kv_string(__VA_ARGS__))
#define LOG_TRACE(...)	Log(LOG_LEVEL_TRACE, ::to_kv_string(__VA_ARGS__))

#define Log(log_level, msg) \
		do{\
			if (!CatDB::Common::can_trace_log(log_level)) {	\
			} else {	\
				CatDB::Common::log_output(log_level, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg); \
			}	\
		} while (0);

namespace CatDB {
	namespace Common {
		void init_log_file();
		bool can_trace_log(int log_level);
		void log_output(int log_level, const char* file, int line, const char* function, const String& msg);
		String lbt(); 
	}
}
#endif	//LOG_H
