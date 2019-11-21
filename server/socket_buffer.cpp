#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "socket_buffer.h"
#define MAGIC	0xa2b3c4d5
using namespace CatDB::Server;

void CatDB::Server::buffer_calloc(Buffer& buf, unsigned int len)
{
	buf.capacity = len;
	buf.data = (char*)malloc(buf.capacity);
	if(buf.data)
		memset(buf.data, 0, buf.capacity);
	else
	{
		buf.capacity = 0;
		printf("no more memory");
	}
	buf.length = 0;
}

void CatDB::Server::buffer_free(Buffer& buf)
{
	if (buf.data)
		free(buf.data);
	buf.data = 0;
	buf.length = 0;
	buf.capacity = 0;
}

void CatDB::Server::buffer_realloc(Buffer& buf, unsigned int len)
{
	if(buf.capacity >= len)
		return;
	buffer_free(buf);
	buffer_calloc(buf,len);
}

void CatDB::Server::buffer_copy(Buffer& buf, const char* ptr, unsigned int len)
{
	if(buf.capacity < len)
	{
		buffer_free(buf);
		buffer_calloc(buf,len);
	}
	memcpy(buf.data, ptr, len);
	buf.length = len;
}

BufferCacheBase::BufferCacheBase(int size)
	:m_head(0),
	m_tail(0)
{
	buffer_calloc(m_buffer, size);
}

BufferCacheBase::~BufferCacheBase()
{
	buffer_free(m_buffer);
}

int BufferCacheBase::size()const
{
	if(m_head <= m_tail)
		return m_tail-m_head;
	else
		return m_tail+m_buffer.capacity-m_head;
}

int BufferCacheBase::capacity()const
{
	return m_buffer.capacity;
}

bool BufferCacheBase::is_empty()const
{
	return m_head == m_tail;
}

bool BufferCacheBase::is_full()const
{
	return ((m_tail+1))==m_head;
	return ((m_tail+1)%m_buffer.capacity)==m_head;
}

int BufferCacheBase::have_a_look(Buffer& buf)
{
	if(buf.length > size())
		buf.length = size();
	if(is_empty())
		return 0;

	if(m_buffer.capacity-m_head >= buf.length)
	{
		memcpy(buf.data, m_buffer.data+m_head, buf.length);
	}
	else
	{
		int len = m_buffer.capacity-m_head;
		memcpy(buf.data, m_buffer.data+m_head, len);
		memcpy(buf.data+len, m_buffer.data, buf.length-len);
	}
	return buf.length;
}

int BufferCacheBase::read(Buffer& buf)
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	if(buf.length > size())
		buf.length = size();
	if(is_empty())
		return 0;
	
	if(m_buffer.capacity-m_head >= buf.length)
	{
		memcpy(buf.data, m_buffer.data+m_head, buf.length);
		m_head += buf.length;
	}
	else
	{
		int len = m_buffer.capacity-m_head;
		memcpy(buf.data, m_buffer.data+m_head, len);
		memcpy(buf.data+len, m_buffer.data, buf.length-len);
		m_head = buf.length-len;
	}
	return buf.length;
}

int BufferCacheBase::write(const Buffer& buf)
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	if(is_full() || buf.length > capacity()-size() || buf.length <= 0)
		return 0;

	int length = buf.length;
	
	if(m_buffer.capacity-m_tail >= length)
	{
		memcpy(m_buffer.data+m_tail, buf.data, length);
		m_tail += length;
	}
	else
	{
		int len = m_buffer.capacity-m_tail;
		memcpy(m_buffer.data+m_tail, buf.data, len);
		memcpy(m_buffer.data, buf.data+len, length-len);
		m_tail = length - len;
	}
	
	return length;
}

BufferCache::BufferCache(int size)
	:BufferCacheBase(size)
{
}

BufferCache::~BufferCache()
{
}
#define SMALL_END
inline unsigned int change_endian(unsigned int n)
{
	return (n >> 24 & 0xFF) |
		(n >> 8 & 0xFF00) |
		(n << 8 & 0xFF0000) |
		(n << 24 & 0xFF000000);
}

inline unsigned int local_to_small_end(unsigned int n)
{
#ifdef SMALL_END
	return n;
#else
	return change_endian(n);
#endif	//BIG_END
}

inline unsigned int small_end_to_local(unsigned int n)
{
#ifdef SMALL_END
	return n;
#else
	return change_endian(n);
#endif	//BIG_END
}

bool BufferCache::write_package(const char* buffer, unsigned int len)
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	Header head;
	head.magic = MAGIC;
	head.length = len;
	Buffer buf;
	buf.capacity = sizeof(head);
	buf.length = sizeof(head);
	buf.data = reinterpret_cast<char*>(&head);
	if(write(buf) == 0)
		return false;
	
	buf.capacity = len;
	buf.length = len;
	buf.data = (char*)buffer;
	if(write(buf) == 0)
		return false;
	return true;
}

bool BufferCache::read_package(Header& head, Buffer& buffer)
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	
	if(size() < sizeof(Header))
		return false;
	
	Buffer buf;
	buf.capacity = sizeof(Header);
	buf.length = sizeof(Header);
	buf.data = (char*)&head;

	have_a_look(buf);
	if(head.magic != MAGIC)
	{
		return false;
	}
	if(size() < sizeof(Header)+head.length)
		return false;

	buffer_realloc(buffer, head.length);

	buf.length = sizeof(Header);
	read(buf);
	buffer.length = head.length;
	read(buffer);
	return true;
}
