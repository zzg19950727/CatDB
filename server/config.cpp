#include "config.h"

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

int ServerServiceConfig::worker_count()const
{
	String count = m_config.value("worker_count");
	if(count.empty() || std::stoi(count)<=0)
		return 600;
	else
		return std::stoi(count);
}

int ServerServiceConfig::epoll_workers()const
{
	String count = m_config.value("epoll_workers");
	if(count.empty() || std::stoi(count)<=0)
		return 40;
	else
		return std::stoi(count);
}
