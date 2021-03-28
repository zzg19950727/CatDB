#ifndef LOG_H
#define LOG_H

#define LOG_ERR		0x1
#define LOG_WARN	0x2
#define LOG_INFO	0x4
#define LOG_TRACE	0x8
#define DEBUG 1
#ifdef DEBUG
#define Log(log_level, module, fmt, ...) \
		do{\
			CatDB::Common::log_set_level(log_level, module);\
			if(CatDB::Common::log_set_info(__FILE__, __LINE__, __FUNCTION__))\
				CatDB::Common::log_print(fmt, ##__VA_ARGS__);\
		} while (0);
#else
#define Log(log_level, module, fmt, ...) 
#endif	//DEBUG

namespace CatDB {
	namespace Common {
		bool log_set_info(const char* file, int line, const char* function);
		void log_set_level(int log_level, const char* module);
		void log_print(const char *fmt, ...);
		std::ostream* switch_log_ostream(std::ostream* os);
		void set_log_file(const char* file);
		void set_debug_level(int level);
	}
}
#endif	//LOG_H
