#include "request_handle.h"
#include "schema_guard.h"
#include "package_manager.h"
#include "statis_manager.h"
#include "table_space.h"
#include "session_info.h"
#include "global_context.h"
#include "loginer.h"
#include "object.h"
#include "server.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;
using namespace CatDB::Storage;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;
using namespace CatDB::Package;
using namespace CatDB::Common;

ServerService::ServerService()
	:m_net_service(0),
	m_workers(),
	m_clients(0),
	m_session_id(0)
{
	
}

ServerService::~ServerService()
{
}

int ServerService::init(const String& config)
{
	GTX->init_config(config.c_str());
	m_workers.init(GTX->config().thread_pool_size());
	init_log_file();
	SESSION_CTX->set_session_log_level(GTX->config().log_level());
	SESSION_CTX->set_session_log_module(GTX->config().log_module());
	SESSION_CTX->set_root_session();
	PackageManager_s& package_manager = PackageManager::get_package_manager();
	if (package_manager->init() != SUCCESS) {
		LOG_ERR("failed to init package manager");
		return -1;
	}
	SchemaGuard_s &schema_guard = SchemaGuard::get_schema_guard();
	if ((schema_guard->init_guard()) != SUCCESS) {
		LOG_ERR("failed to init schema guard");
		return -1;
	}
	StatisManager_s &manager = StatisManager::get_statis_manager();
	if ((manager->init_statis_cache()) != SUCCESS) {
		LOG_ERR("failed to init statis manager");
		return -1;
	}
	return 0;
}

int ServerService::run()
{
	m_fd = start_listen(GTX->config().ip().c_str(), GTX->config().port(), GTX->config().max_client_count());
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
		if(m_clients >= GTX->config().max_client_count())
		{
			LOG_ERR("ServerService::new_connection too much clients,rejuect", K(m_clients));
			net_close(client_fd);
			return;
		}
		do_login(client_fd);
	}
}

void ServerService::do_login(int fd)
{
	Loginer loginer(m_session_id, fd);
	if (loginer.login() == SUCCESS) {
		auto ptr = RequestHandle_s(new RequestHandle(fd, *this));
		GTX->add_process(m_session_id, ptr);
		ptr->set_login_info(loginer.get_login_info(), m_session_id);
		ptr->set_delete_handle(ptr);
		++m_clients;
		++m_session_id;
		LOG_TRACE("new client login", K(fd));
	} else {
		LOG_ERR("login failed");
	}
}

void ServerService::close_connection(int fd)
{
	--m_clients;
	GTX->remove_process(fd);
	LOG_TRACE("client closed", K(fd));
}

void ServerService::close_connection()
{
	kill_all_process();
	net_close(m_fd);
	m_fd = -1;
	m_workers.quit();
	LOG_INFO("stop ServerService success");
}

void ServerService::kill_all_process()
{
	for (auto iter = GTX->get_all_processlist().begin(); iter != GTX->get_all_processlist().end(); ++iter) {
		iter->second->get_session_info()->kill_query();
	}
}

NetService& ServerService::net_service()
{
	return m_net_service;
}

ThreadPool& ServerService::workers()
{
	return m_workers;
}