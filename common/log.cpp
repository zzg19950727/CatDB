#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cstdarg>
#include <thread>
#include "timer_manager.h"
#include "type.h"
#include "log.h"

static int LOG_SET_LEVEL = LOG_LEVEL_TRACE;
static String LOG_MODULE = "";
HashMap<std::thread::id, String> trace_ids;

using namespace CatDB::Common;

void CatDB::Common::get_trace_id(String &trace_id)
{
	std::thread::id id = std::this_thread::get_id();
	if (trace_ids.find(id) != trace_ids.cend()) {
		trace_id = trace_ids[id];
	} else {
		trace_id = "000000000";
	}
}

void CatDB::Common::set_trace_id(String &trace_id)
{
	std::thread::id id = std::this_thread::get_id();
	CatDB::Server::DateTime::TimePoint tp = std::chrono::steady_clock::now();
	CatDB::Server::DateTime::NanoSeconds nano(tp.time_since_epoch());
	Hash<CatDB::Server::DateTime::Rep> func;
	u64 value = func(nano.count());
	static const char *hex_map = "0123456789ABCDEF";
	trace_id.clear();
	for (u32 i = 0; i < 16; ++i) {
		trace_id += hex_map[value & 0xf];
		value >>= 4;
	}
	trace_ids[id] = trace_id;
}

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
	String module = get_module_name(function);
	String trace_id;
	if (log_level > LOG_SET_LEVEL || 
		(LOG_MODULE.find(module) == LOG_MODULE.npos &&
		LOG_MODULE != "ALL"))
		return ;
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	get_trace_id(trace_id);
	*os << "[" << put_time(t) <<"] " << "[" << trace_id << "] ";
	if (log_level == LOG_LEVEL_INFO)
		*os << "[INFO ] ";
	else if (log_level == LOG_LEVEL_ERR)
		*os << "[ERR  ] ";
	else if (log_level == LOG_LEVEL_TRACE)
		*os << "[TRACE] ";
	*os << "[" << module << "] " << "[" << file << ":" << line << "] " << "[ " << msg << " ]" << std::endl;
	os->flush();
}

void CatDB::Common::set_log_file(const char* file)
{
	ostream.set_log_file(file);
}

void CatDB::Common::set_debug_level(int level)
{
	LOG_SET_LEVEL = level;
}

void CatDB::Common::set_debug_module(const String &module)
{
	LOG_MODULE = module;
}

void CatDB::Common::log_output(int log_level, const char* file, int line, const char* function, const String& msg)
{
	ostream.print_msg(log_level, file, line, function, msg);
}