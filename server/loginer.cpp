#include "loginer.h"
#include "packet/ok_packet.h"
#include "packet/error_packet.h"
#include "packet/handshake_packet.h"
#include "net_service.h"
#include "buffer.h"
#include "error.h"
#include "util.h"
#include "log.h"
#define MAX_LEN		1024
using namespace CatDB::Common;
using namespace CatDB::Server;

ClientCapability::ClientCapability(uint32_t ca) : capability_(ca)
{

}
Loginer::LoginInfo::LoginInfo()
	: capability_(0), max_packet_size_(0), character_set_(0)
	, user_name_(), db_name_(), auth_response_()
{
}
Loginer::Loginer(int client_id, int client_fd)
	: login_info_(),
	client_id(client_id),
	client_fd(client_fd)
{

}

Loginer::~Loginer()
{

}

int Loginer::login()
{
	int ret = SUCCESS;
	ret = handshake();
	if (SUCCESS != ret)
	{
		Log(LOG_WARN, "Loginer", "send hand shake packet failed");
	}
	else
	{
		ret = parse_packet();
		if (SUCCESS != ret)
		{
			Log(LOG_WARN, "Loginer", "parse client auth packet failed");
		}
		else
		{
			 ret = check_privilege();
			 if (SUCCESS != ret)
			 {
				Log(LOG_WARN, "Loginer", "login failed, err=%d", ret);
			 }
		}
	}
	return ret;
}

const Loginer::LoginInfo & Loginer::get_login_info() const
{
	return login_info_;
}

int Loginer::handshake()
{
	int ret = SUCCESS;
	HandshakePacket packet;

	packet.set_thread_id(client_id);
	Buffer_s buffer = Buffer::make_buffer(MAX_LEN);
	int64_t pos = 0;
	ret = packet.serialize((char*)buffer->buf, buffer->length, pos);
	if (SUCCESS != ret)
	{
		Log(LOG_ERR, "Loginer", "HandshakPacket serialize failed, buffer is %p, buffer len is %ld,"
			"pos is %ld", (char*)buffer->buf, buffer->length, pos);
	}
	else
	{
		ret = write_data(client_fd, (char*)buffer->buf, pos);
		if (SUCCESS != ret)
		{
			Log(LOG_ERR, "Loginer", "write packet data to client failed fd is %d, buffer is %p, length is %ld",
				c->fd, out_buffer.get_data(), out_buffer.get_position());
			ret = ERR_UNEXPECTED;
		}
		else
		{
			Log(LOG_INFO, "Loginer", "new client login", );
		}
	}
	return ret;
}

int Loginer::parse_packet()
{
	int ret = SUCCESS;
	int read_size = PACKET_HEADER_SIZE;
	char* len_pos = NULL;
	Buffer_s buffer = Buffer::make_buffer(MAX_LEN);
	int64_t pos = 0;
	//read packet header first
	ret = read_data(client_fd, (char*)buffer->buf, read_size);
	if (SUCCESS != ret)
	{
		Log(LOG_WARN, "Loginer", "read packet header failed");
		ret = ERR_UNEXPECTED;
	}
	else
	{
		uint32_t packet_len = 0;
		len_pos = (char*)buffer->buf;
		Util::get_uint3(len_pos, packet_len);
		Log(LOG_TRACE, "start read %u bytes", packet_len);
		ret = read_data(client_fd, (char*)buffer->buf + PACKET_HEADER_SIZE, packet_len);
		if (SUCCESS != ret)
		{
			Log(LOG_WARN, "Loginer", "read packet data failed, ret=%d", ret);
			ret = ERR_UNEXPECTED;
		}
		else
		{
			Log(LOG_TRACE, "readed %u bytes", packet_len);
			len_pos = (char*)buffer->buf + PACKET_HEADER_SIZE;
			uint32_t capability_flags = 0;
			uint32_t max_packet_size = 0;
			uint8_t character_set = 0;
			uint8_t auth_response_len = 0;
			int32_t username_len = -1;
			int32_t db_len = -1;
			Util::get_uint4(len_pos, capability_flags);
			login_info_.capability_.capability_ = capability_flags;
			Util::get_uint4(len_pos, max_packet_size);//16MB
			login_info_.max_packet_size_ = max_packet_size;
			Util::get_uint1(len_pos, character_set);
			login_info_.character_set_ = character_set;
			len_pos += 23;//23 bytes reserved
			username_len = static_cast<int32_t>(strlen(len_pos));
			login_info_.user_name_ = String(len_pos, username_len);
			len_pos += username_len + 1;
			Util::get_uint1(len_pos, auth_response_len);
			login_info_.auth_response_ = String(len_pos, static_cast<int32_t>(auth_response_len));
			len_pos += auth_response_len;
			db_len = static_cast<int32_t>(strlen(len_pos));
			login_info_.db_name_ = String(len_pos, db_len);
			len_pos += db_len;

		}
	}
	return ret;
}

int Loginer::check_privilege()
{
	int ret = SUCCESS;
	int send_err = SUCCESS;
	Packet *packet = NULL;
	ErrorPacket err_packet;
	OKPacket ok_packet;

	Buffer_s buffer = Buffer::make_buffer(MAX_LEN);
	int64_t pos = 0;
	packet = &ok_packet;
	send_err = packet->encode((char*)buffer->buf, buffer->length, pos);
	if (SUCCESS != send_err)
	{
		Log(LOG_ERR, "Loginer", "serialize packet failed, err=%d", send_err);
	}
	else
	{
		send_err = write_data(client_fd, (char*)buffer->buf, pos);
		if (SUCCESS != send_err)
		{
			Log(LOG_WARN, "Loginer", "write packet to mysql client failed, fd=%d, buffer=%p,"
				"length=%ld, err=%d", client_fd, (char*)buffer->buf, pos, send_err);
		}
		else
		{
			Log(LOG_TRACE, "Loginer", "send packet");
		}
	}
	return SUCCESS;
}

int Loginer::write_data(int fd, char* buffer, size_t length)
{
	int ret = SUCCESS;
	if (fd < 0 || NULL == buffer || length <= 0)
	{
		Log(LOG_ERR, "Loginer", "invalid argument fd=%d, buffer=%p, length=%zd", fd, buffer, length);
		ret = ERR_UNEXPECTED;
	}
	else
	{
		char* buff = buffer;
		size_t count = 0;
		while (SUCCESS == ret && length > 0 && (count = net_write(fd, buff, length)) != 0)
		{
			if (-1 == count)
			{
				if (errno == EINTR)
				{
					continue;
				}
				else
				{
					ret = ERROR;
					Log(LOG_WARN, "Loginer", "write data faild, errno is %d, errstr is %s", errno, strerror(errno));
				}

			}
			buff += count;
			length -= count;
		}
	}

	return ret;
}

int Loginer::read_data(int fd, char* buffer, size_t length)
{
	int ret = SUCCESS;
	static const int64_t timeout = 1000000;//1s
	if (fd < 0 || NULL == buffer || length <= 0)
	{
		Log(LOG_ERR, "invalid argument fd=%d, buffer=%p, length=%zd", fd, buffer, length);
		ret = ERR_UNEXPECTED;
	}
	else
	{
		char* buff = buffer;
		size_t count = 0;
		while (SUCCESS == ret && length > 0 && (count = net_read(fd, buff, length)) != 0)
		{
			if (-1 == count)
			{
				if (errno == EINTR || errno == EAGAIN)
				{
					continue;
				}
				else
				{
					ret = ERR_UNEXPECTED;
					Log(LOG_WARN, "Loginer", "read data faild, errno is %d, errstr is %s", errno, strerror(errno));
				}
			}
			buff += count;
			length -= count;
		}
		if (0 != length)
		{
			ret = ERR_UNEXPECTED;
			Log(LOG_WARN, "Loginer", "read not return enough data need %zu more bytes", length);
		}
	}
	return ret;
}
