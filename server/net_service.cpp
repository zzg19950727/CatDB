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
		Log(LOG_ERR, "Socket", "Server create socket failed");
		return -1;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);// htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		Log(LOG_ERR, "Socket", "Server bind address failed");
		return -2;
	}

	if (listen(fd, listen_n) < 0)
	{
		Log(LOG_ERR, "Socket", "Server listen failed");
		return -3;
	}
	return fd;
}

int CatDB::Server::accept_connection(int fd)
{
	socklen_t addrlen = sizeof(struct sockaddr);
	struct sockaddr_in client_addr;
	int client_fd = accept(fd, (struct sockaddr*)&client_addr, &addrlen);

	Log(LOG_TRACE, "accept connection:%d", client_fd);
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
	Log(LOG_INFO, "NetService", "NetService start poll:%d", id++);
	while(1)
	{
		fd_set revent = read_events;
		fd_set wevent = write_events;
		int state = select(1024, &revent, &wevent, 0, 0);
		if(state == -1)
		{
			Log(LOG_ERR, "NetService", "NetService poll error:%d", errno);
			break;
		}
		else if(state == 0)
		{
			continue;
		}
		for (auto iter = read_fds.cbegin(); iter != read_fds.cend(); ++iter) {
			if (FD_ISSET(*iter, &revent)) {
				response_event(*iter, E_READ);
			}
		}
		for (auto iter = write_fds.cbegin(); iter != write_fds.cend(); ++iter) {
			if (FD_ISSET(*iter, &wevent)) {
				response_event(*iter, E_WRITE);
			}
		}
	}
	Log(LOG_INFO, "NetService", "NetService stop poll");
}

int NetService::register_io(int fd, Event e, CallbackFunc& f)
{
	Log(LOG_INFO, "NetService", "NetService register io fd:%d and Event:%d", fd, e);
	if(add_event(fd,e) < 0)
	{
		Log(LOG_ERR, "NetService", "NetService register io failed:%d",errno);
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
	Log(LOG_INFO, "NetService", "NetService unregister io fd:%d and Event:%d", fd, e);
	if(remove_event(fd,e) < 0)
	{
		Log(LOG_ERR, "NetService", "NetService unregister io failed:%d",errno);
		return -1;
	}

	if(e == E_RW)
	{
		m_callback_map.erase(FdEvent(fd,E_READ));
		m_callback_map.erase(FdEvent(fd,E_WRITE));
	}
	else
		m_callback_map.erase(FdEvent(fd,e));
	return 0;
}

void NetService::create_poll(int size)
{
#ifdef _WIN32
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		Log(LOG_DEBUG, "NetService", "NetService create epoll:%d",m_epoll_fd);
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
		Log(LOG_WARN, "NetService", "NetService:unknow fd:%d and Event:%d",fd,e);
	}
}
