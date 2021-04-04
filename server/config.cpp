#include "config.h"
#include "log.h"

using namespace CatDB::Server;

ServerServiceConfig::ServerServiceConfig(const char* path)
	:m_config(path)
{
	
}

ServerServiceConfig::~ServerServiceConfig()
{
	
}

std::string ServerServiceConfig::ip()const
{
	String ip = m_config.value("ip");
	if(ip.empty())
		return "127.0.0.1";
	else
		return ip;
}

int ServerServiceConfig::port()const
{
	String port = m_config.value("port");
	if(port.empty() || std::stoi(port)<=0 )
		return 1234;
	else
		return std::stoi(port);
}

int ServerServiceConfig::max_client_count()const
{
	String count = m_config.value("max_client_count");
	if(count.empty() || std::stoi(count)<=0)
		return 2048;
	else
		return std::stoi(count);
}

int ServerServiceConfig::cache_size()const
{
	String size = m_config.value("cache_size");
	if(size.empty() || std::stoi(size)<=0)
		return 1024000;
	else
		return std::stoi(size);
}

int ServerServiceConfig::thread_pool_size() const
{
	String thread_pool_size = m_config.value("thread_pool_size");
	if (thread_pool_size.empty() || std::stoi(thread_pool_size) <= 0)
		return 1234;
	else
		return std::stoi(thread_pool_size);
}

String ServerServiceConfig::data_dir()const
{
	String dir = m_config.value("data_dir");
	return dir;
}

String ServerServiceConfig::recycle_dir()const
{
	String dir = m_config.value("recycle_dir");
	return dir;
}

String ServerServiceConfig::log_file_path()const
{
	String dir = m_config.value("log_file");
	return dir;
}

int ServerServiceConfig::log_level()const
{
	String level_str = m_config.value("log_level");
	int level = LOG_LEVEL_ERR;
	if (level_str == "trace") {
		level = LOG_LEVEL_TRACE;
	} else if (level_str == "warning") {
		level = LOG_LEVEL_WARN;
	} else if (level_str == "error") {
		level = LOG_LEVEL_ERR;
	}
	return level;
}

int ServerServiceConfig::sample_level() const
{
	String level = m_config.value("sample_level");
	if(level.empty())
		return 1;
	else
		return std::stoi(level);
}