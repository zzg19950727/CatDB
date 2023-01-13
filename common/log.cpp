#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cstdarg>
#include <execinfo.h>
#include "timer_manager.h"
#include "session_info.h"
#include "global_context.h"
#include "type.h"
#include "log.h"

using namespace CatDB::Common;

class LogStream
{
public:
	LogStream();
	~LogStream();
	void set_log_file(const char* path);
	void print_msg(int log_level, const char* file, int line, const char* function, const String& msg);

private:
	std::ofstream out_file;
	std::ostream* os;
	std::string path;
};

LogStream::LogStream()
	:os(&std::cout)
{

}

LogStream::~LogStream()
{
	if(out_file.is_open())
	{
		out_file.close();
	}	
}

void LogStream::set_log_file(const char* path)
{
	out_file.open(path, std::ios::app);
	this->path = String(path);
	os = &out_file;
}

LogStream ostream;

String put_time(time_t t)
{
	char tmp[255];
	tm* time = localtime(&t);
	if (time == NULL)
	{
		return "";
	}
	sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d",
		time->tm_year + 1900,
		time->tm_mon + 1,
		time->tm_mday + 1,
		time->tm_hour,
		time->tm_min,
		time->tm_sec
	);
	return String(tmp);
}

String get_module_name(const char* function)
{
	String func(function);
	String module;
	u32 i = 0;
	while (i < func.length() && func[i] != ':') ++i;
	++i;++i;
	while(i < func.length() && func[i] != ':') {
		module += func[i];
		++i;
	}
	return module;
}

void LogStream::print_msg(int log_level, const char* file, int line, const char* function, const String& msg)
{
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	*os << "[" << put_time(t) <<"] ";
	if (log_level == LOG_LEVEL_INFO)
		*os << "[INFO ] ";
	else if (log_level == LOG_LEVEL_ERR)
		*os << "[ERR  ] ";
	else if (log_level == LOG_LEVEL_TRACE)
		*os << "[TRACE] ";
	String module = get_module_name(function);
	*os << "[" << module << "] " << "[" << SESSION_CTX->get_trace_id() << "] " 
	<< "[" << file << ":" << line << "] " << "[ " << msg << " ]" 
	<< std::endl;
	os->flush();
}

void CatDB::Common::init_log_file()
{
	ostream.set_log_file(SYS_CONF.log_file_path().c_str());
}

bool CatDB::Common::can_trace_log(int log_level)
{
	return log_level <= SESSION_CTX->get_session_log_level();
}

void CatDB::Common::log_output(int log_level, const char* file, int line, const char* function, const String& msg)
{
	if (SESSION_CTX->get_session_log_module() == "ALL") {
		ostream.print_msg(log_level, file, line, function, msg);
	} else {
		String module = get_module_name(function);
		if (SESSION_CTX->get_session_log_module().find(module) != module.npos) {
			ostream.print_msg(log_level, file, line, function, msg);
		}
	}
}

std::string CatDB::Common::lbt() 
{
  void *array[128];
  size_t size = backtrace(array, 128);
  char tmp[32] = {0};
  std::string ret;
  for (int i = 0; i < size; ++i) {
      sprintf(tmp, "%p ", array[i]);
      ret += std::string(tmp);
  }
  return ret;
}