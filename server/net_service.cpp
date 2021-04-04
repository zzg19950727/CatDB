#include "net_service.h"
#include "error.h"
#include "log.h"
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
typedef int socklen_t;
#define read(fd, buf, len)	recv(fd, buf, len, 0)
#define write(fd, buf, len)	send(fd, buf, len, 0)
#define close(fd)			closesocket(fd)
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#endif	//_WIN32
using namespace CatDB::Server;

int CatDB::Server::start_listen(const char* ip, int port, int listen_n)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		LOG_ERR("Server create socket failed", K(ip), K(port));
		return -1;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		LOG_ERR("Server bind address failed", K(ip), K(port));
		return -2;
	}

	if (listen(fd, listen_n) < 0)
	{
		LOG_ERR("Server listen failed", K(ip), K(port), K(listen_n));
		return -3;
	}
	return fd;
}

int CatDB::Server::accept_connection(int fd)
{
	socklen_t addrlen = sizeof(struct sockaddr);
	struct sockaddr_in client_addr;
	int client_fd = accept(fd, (struct sockaddr*)&client_addr, &addrlen);

	LOG_TRACE("accept connection", K(client_fd));
	return client_fd;
}

int CatDB::Server::net_read(int fd, char* buf, size_t len)\
{
	return read(fd, buf, len);
}

int CatDB::Server::net_write(int fd, char* buf, size_t len)
{
	return write(fd, buf, len);
}

void CatDB::Server::net_close(int fd)
{
	close(fd);
}

NetService::NetService(int size)
{
	FD_ZERO(&read_events);
	FD_ZERO(&write_events);
	create_poll(size);
}

NetService::~NetService()
{
#ifdef _WIN32
	WSACleanup();
#endif//_WIN32
}

void NetService::poll()
{
	static int id = 1;
	LOG_TRACE("NetService start poll", K(id));
	++id;
	while(1)
	{
		fd_set revent = read_events;
		fd_set wevent = write_events;
		int state = select(1024, &revent, &wevent, 0, 0);
		if(state == -1)
		{
			LOG_ERR("NetService poll error", K(errno));
			break;
		}
		else if(state == 0)
		{
			continue;
		}
		Vector<int> r_fds;
		for (auto iter = read_fds.cbegin(); iter != read_fds.cend(); ++iter) {
			r_fds.push_back(*iter);
		}
		for (u32 i = 0; i < r_fds.size(); ++i) {
			if (FD_ISSET(r_fds[i], &revent)) {
				response_event(r_fds[i], E_READ);
			}
		}

		Vector<int> w_fds;
		for (auto iter = write_fds.cbegin(); iter != write_fds.cend(); ++iter) {
			w_fds.push_back(*iter);
		}
		for (u32 i = 0; i < w_fds.size(); ++i) {
			if (FD_ISSET(w_fds[i], &wevent)) {
				response_event(w_fds[i], E_WRITE);
			}
		}
	}
	LOG_TRACE("NetService stop poll");
}

int NetService::register_io(int fd, Event e, CallbackFunc& f)
{
	LOG_TRACE("NetService register io", K(fd), K(u8(e)));
	if(add_event(fd,e) < 0)
	{
		LOG_ERR("NetService register io failed", K(errno));
		return -1;
	}
	
	if(e == E_RW)
	{
		m_callback_map[FdEvent(fd,E_READ)] = f;
		m_callback_map[FdEvent(fd,E_WRITE)] = f;
	}
	else
		m_callback_map[FdEvent(fd,e)] = f;
	return 0;
}

int NetService::unregister_io(int fd, Event e)
{
	LOG_TRACE("NetService unregister io", K(fd), K(u8(e)));
	if(remove_event(fd,e) < 0)
	{
		LOG_ERR("NetService unregister io failed", K(errno));
		return -1;
	}

	if(e == E_RW)
	{
		m_callback_map.erase(FdEvent(fd,E_READ));
		m_callback_map.erase(FdEvent(fd,E_WRITE));
	}
	else
	{
		m_callback_map.erase(FdEvent(fd, e));
	}
	return 0;
}

void NetService::create_poll(int size)
{
#ifdef _WIN32
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		LOG_TRACE("NetService create epoll");
#endif	//_WIN32
}

int NetService::add_event(int fd, Event e)
{
	if (e == E_READ) {
		FD_SET(fd, &read_events);
		read_fds.insert(fd);
	}
	else if (e == E_WRITE) {
		FD_SET(fd, &write_events);
		write_fds.insert(fd);
	}
	else {
		FD_SET(fd, &read_events);
		read_fds.insert(fd);
		FD_SET(fd, &write_events);
		write_fds.insert(fd);
	}
	return SUCCESS;
}

int NetService::remove_event(int fd, Event e)
{
	if (e == E_READ) {
		FD_CLR(fd, &read_events);
		read_fds.erase(fd);
	}
	else if (e == E_WRITE) {
		FD_CLR(fd, &write_events);
		write_fds.erase(fd);
	}
	else {
		FD_CLR(fd, &read_events);
		read_fds.erase(fd);
		FD_CLR(fd, &write_events);
		write_fds.erase(fd);
	}
	return SUCCESS;
}

int NetService::modify_event(int fd, Event e)
{
	remove_event(fd, E_RW);
	add_event(fd, e);
	return SUCCESS;
}

void NetService::response_event(int fd, Event e)
{
	FdEvent key(fd,e);
	auto iter = m_callback_map.find(key);
	if(iter != m_callback_map.cend())
	{
		if(m_callback_map[key])
			m_callback_map[key](fd, e);
	}
	else if(e != E_WRITE)
	{
		LOG_WARN("NetService:unknow fd", K(fd), K(u8(e)));
	}
}
