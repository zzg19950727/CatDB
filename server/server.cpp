#include "request_handle.h"
#include "table_space.h"
#include "loginer.h"
#include "server.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;
using namespace CatDB::Storage;

ServerService::ServerService(const String& config)
	:m_config(config.c_str()),
	m_net_service(0),
	m_workers(m_config.thread_pool_size()),
	m_clients(0),
	m_thread_id(0)
{
	TableSpace::data_dir = m_config.data_dir();
	TableSpace::recycle_dir = m_config.recycle_dir();
	String log_path = m_config.log_file_path();
	if(!log_path.empty())
	{
		CatDB::Common::set_log_file(log_path.c_str());
	}
	CatDB::Common::set_debug_level(m_config.log_level());
}

ServerService::~ServerService()
{
	close_connection();
}

int ServerService::run()
{
	m_fd = start_listen(m_config.ip().c_str(), m_config.port(), m_config.max_client_count());
	if (m_fd > 0)
	{
		NetService::CallbackFunc func = std::bind(&ServerService::new_connection, this, std::placeholders::_1, std::placeholders::_2);
		if (m_net_service.register_io(m_fd, NetService::E_RW, func) < 0)
		{
			close_connection();
			return -1;
		}
		else 
		{
			m_net_service.poll();
			return 0;
		}
	}
	else {
		return - 1;
	}
}

void ServerService::new_connection(int fd, NetService::Event e)
{
	int client_fd = accept_connection(fd);
	if(client_fd > 0)
	{
		if(m_clients >= m_config.max_client_count())
		{
			LOG_WARN("ServerService::new_connection too much clients,rejuect", K(m_clients));
			net_close(client_fd);
			return;
		}
		do_login(client_fd);
	}
}

void ServerService::do_login(int fd)
{
	Loginer loginer(m_thread_id, fd);
	if (loginer.login() == SUCCESS) {
		auto ptr = std::make_shared<RequestHandle>(fd, *this);
		ptr->set_login_info(loginer.get_login_info());
		ptr->set_delete_handle(ptr);
		++m_clients;
		++m_thread_id;
		LOG_TRACE("new client login", K(fd));
	}
	else {
		LOG_WARN("login failed");
	}
}

void ServerService::close_connection(int fd)
{
	--m_clients;
	LOG_TRACE("client closed", K(fd));
}

void ServerService::close_connection()
{
	LOG_TRACE("stop ServerService success");
	net_close(m_fd);
	m_fd = -1;
}

NetService& ServerService::net_service()
{
	return m_net_service;
}

ThreadPool& ServerService::workers()
{
	return m_workers;
}

ServerServiceConfig& ServerService::config()
{
	return m_config;
}
