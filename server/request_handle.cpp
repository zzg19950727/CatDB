#include "request_handle.h"
#include "server.h"
#include "error.h"
#include "log.h"
#define BLOCK_SIZE	2048
using namespace CatDB::Server;

RequestHandle::RequestHandle(int fd,ServerService& service)
	:m_fd(fd),
	m_read_cache(service.config().cache_size()),
	m_write_cache(service.config().cache_size()),
	m_server_service(service)
{
	NetService::CallbackFunc func = std::bind(&RequestHandle::notify_socket,this, std::placeholders::_1, std::placeholders::_2);
	if(m_server_service.m_net_service.register_io(m_fd, NetService::E_RW, func) < 0)
	{
		close_connection();
	}
}

RequestHandle::~RequestHandle()
{
	if(m_fd > 0)
		close_connection();
}

void RequestHandle::set_delete_handle(std::shared_ptr<RequestHandle>& self)
{
	m_self.swap(self);
}

void RequestHandle::notify_socket(int fd, NetService::Event e)
{
	if(e == NetService::E_READ)
		read_socket(fd);
	else if(e == NetService::E_WRITE)
		write_socket(fd);
}

void RequestHandle::read_socket(int fd)
{
	char data[BLOCK_SIZE];
	size_t len = BLOCK_SIZE;
	
	len = net_read(fd, data, len);
	Log(LOG_TRACE, "RequestHandle", "RequestHandle::read_socket fd %d recv %zu bytes data",m_fd,len);

	Buffer buffer;
	buffer.capacity = BLOCK_SIZE;
	buffer.length = len;
	buffer.data = data;
	m_read_cache.write(buffer);
	
	Header head;
	bool state = true;
	while(state)
	{
		buffer.length = BLOCK_SIZE;
		if(state=m_read_cache.read_package(head,buffer))
		{
			handle_request((unsigned char*)buffer.data, buffer.length);
		}
	}
	//disconnect
	if(len == 0)
	{
		close_connection();
	}
}

void RequestHandle::write_socket(int fd)
{
	char buf[BLOCK_SIZE];
	Buffer tmp;
	tmp.capacity = BLOCK_SIZE;
	tmp.data = buf;
	while(true)
	{
		tmp.length = BLOCK_SIZE;
		m_write_cache.have_a_look(tmp);
		size_t len = tmp.length;
		len = net_write(fd, buf, len);
	
		tmp.length = len;
		if(len > 0)
		{
			m_write_cache.read(tmp);
			Log(LOG_TRACE, "RequestHandle", "RequestHandle::write_socket fd %d send %zu bytes data",m_fd,len);
		}
		if(len != BLOCK_SIZE)
			break;
	}
}

void RequestHandle::close_connection()
{
	Log(LOG_TRACE, "RequestHandle", "RequestHandle close connection:%d",m_fd);
	m_server_service.m_net_service.unregister_io(m_fd, NetService::E_RW);
	m_server_service.close_connection(m_fd);
	std::shared_ptr<RequestHandle> copy;
	copy.swap(m_self);
	net_close(m_fd);
	m_fd = -1;
}

template <typename T>
std::shared_ptr<T> make_shared_array(size_t size)
{
    return std::shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

void RequestHandle::worker_caller(const std::string& func, std::shared_ptr<char> ptr, size_t len)
{
	Log(LOG_TRACE, "RequestHandle", "RequestHandle::worker_caller call func %s( 0x%p, %zu)", func.c_str(),ptr.get(), len);
	
	if(!m_write_cache.write_package(ptr.get(),len))
	{
		Log(LOG_ERR, "RequestHandle", "RequestHandle write_cache full,drop %s`s response", func.c_str());
	}
}

void RequestHandle::handle_request(unsigned char* buf, size_t len)
{
	
}
