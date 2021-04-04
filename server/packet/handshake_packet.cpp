#include "handshake_packet.h"
#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

HandshakePacket::HandshakePacket()
{
	protocol_version_ = 10;//Protocol::HandshakeV10
	server_version_ = "5.6.0";
	thread_id_ = 0;
	memset(scramble_buff_, 'a', 8);
	filler_ = 0;
	//0xF7FF, 多个flag的组合，其中支持4.1 协议的flag置了1,
	//CLIENT_PLUGIN_AUTH 这个flag没有置上,它的值为0x00080000
	//CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA 没有置为1，0x00200000
	//CLIENT_CONNECT_WITH_DB:IS SET,0x00000008
	server_capabilities_ = 63487;
	server_language_ = 8;//latin1_swedish_ci
	server_status_ = 0;// no this value in mysql protocol document
	memset(plugin_, 0, sizeof(plugin_));
	memset(plugin2_, 'b', 12);
}

HandshakePacket::~HandshakePacket()
{
}

//seq of handshake is 0
int HandshakePacket::serialize(char* buffer, int64_t len, int64_t &pos)
{
	int ret = SUCCESS;
	int64_t start_pos = pos;
	if (NULL == buffer || 0 >= len || pos < 0)
	{
		LOG_ERR("invalid argument buffer=%p, length=%ld, pos=%ld",
			buffer, len, pos);
		ret = INVALID_ARGUMENT;
	}
	else
	{
		//skip packet header serialize head at end of this function
		pos += PACKET_HEADER_SIZE;

		if (SUCCESS != (ret = Util::store_int1(buffer, len, protocol_version_, pos)))
		{
			LOG_ERR("serialize packet protocol_version failed, buffer=%p, length=%ld,"
				"protocol_version=%d, pos=%ld", buffer, len, protocol_version_, pos);
		}
		else if (SUCCESS != (ret = Util::store_obstr_zt(buffer, len, server_version_, pos)))
		{
			LOG_ERR("serialize packet server_version failed, buffer=%p, length=%ld,"
				"server_version length=%d, pos=%ld", buffer, len, server_version_.length(), pos);
		}
		else if (SUCCESS != (ret = Util::store_int4(buffer, len, thread_id_, pos)))
		{
			LOG_ERR("serialize packet thread_id failed, buffer=%p, length=%ld,"
				"thread_id=%d, pos=%ld", buffer, len, thread_id_, pos);
		}

		//make sure buffer + pos + SCRAMBLE_SIZE < buffer + length
		//serialize SCRAMBLE && filler_
		if (SUCCESS == ret)
		{
			if ((buffer + pos + SCRAMBLE_SIZE < buffer + len))
			{
				memcpy(buffer + pos, scramble_buff_, SCRAMBLE_SIZE + 1);
				pos += SCRAMBLE_SIZE + 1;
			}
			else
			{
				LOG_ERR("not enough buffer to serialize scramble_buff && filler, buffer=%p, len=%ld,"
					"scramble_buff&&filler length=%d,pos=%ld", buffer, len, SCRAMBLE_SIZE + 1, pos);
				ret = ERR_UNEXPECTED;
			}
		}

		if (SUCCESS == ret)
		{
			/*if (SUCCESS != (ret = Util::store_int1(buffer, len, filler_, pos)))
			{
			LOG_ERR("serialize packet filler_ failed, buffer=%p, length=%ld,"
			"server_capabilities=%c, pos=%ld", buffer, len, filler_, pos);
			}
			else*/ if (SUCCESS != (ret = Util::store_int2(buffer, len, server_capabilities_, pos)))
			{
				LOG_ERR("serialize packet server_capabilities failed, buffer=%p, length=%ld,"
					"server_capabilities=%d, pos=%ld", buffer, len, server_capabilities_, pos);
			}
			else if (SUCCESS != (ret = Util::store_int1(buffer, len, server_language_, pos)))
			{
				LOG_ERR("serialize packet server_language failed, buffer=%p, length=%ld,"
					"server_language=%d, pos=%ld", buffer, len, server_language_, pos);
			}
			else if (SUCCESS != (ret = Util::store_int2(buffer, len, server_status_, pos)))
			{
				LOG_ERR("serialize packet server_status failed, buffer=%p, length=%ld,"
					"server_status=%d, pos=%ld", buffer, len, server_status_, pos);
			}
		}

		if (SUCCESS == ret)
		{
			if ((buffer + pos + PLUGIN_SIZE < buffer + len))
			{
				memcpy(buffer + pos, plugin_, PLUGIN_SIZE);
				pos += PLUGIN_SIZE;
			}
			else
			{
				LOG_ERR("not enough buffer to serialize plugin, buffer=%p, len=%ld,"
					"scramble_buff length=%d,pos=%ld", buffer, len, PLUGIN_SIZE, pos);
				ret = ERR_UNEXPECTED;
			}
		}

		if (SUCCESS == ret)
		{
			if ((buffer + pos + PLUGIN2_SIZE < buffer + len))
			{
				memcpy(buffer + pos, plugin2_, PLUGIN2_SIZE);
				pos += PLUGIN2_SIZE;
			}
			else
			{
				LOG_ERR("not enough buffer to serialize plugin, buffer=%p, len=%ld,"
					"plugin2 length=%d,pos=%ld", buffer, len, PLUGIN2_SIZE, pos);
				ret = ERR_UNEXPECTED;
			}
		}

		if (SUCCESS == ret)
		{
			if ((buffer + pos + 1 < buffer + len))
			{
				memset(buffer + pos, 0, 1);
				pos += 1;
			}
			else
			{
				LOG_ERR("not enough buffer to serialize filler2, buffer=%p, len=%ld,"
					"terminated_ length=%c,pos=%ld", buffer, len, terminated_, pos);
				ret = ERR_UNEXPECTED;
			}
		}

		uint32_t pkt_len = static_cast<uint32_t>(pos - start_pos - PACKET_HEADER_SIZE);
		if (SUCCESS == ret)
		{
			if (SUCCESS != (ret = Util::store_int3(buffer, len, pkt_len, start_pos)))
			{
				LOG_ERR("serialize packet haader size failed, buffer=%p, length=%ld, value=%d, pos=%ld",
					buffer, len, pkt_len, start_pos);
			}
			else if (SUCCESS != (ret = Util::store_int1(buffer, len, 0, start_pos)))
			{
				LOG_ERR("serialize packet haader seq failed, buffer=%p, length=%ld, value=%d, pos=%ld",
					buffer, len, 1, start_pos);
			}
		}
	}
	return ret;
}

int HandshakePacket::set_server_version(const String& version)
{
	int ret = SUCCESS;
	server_version_ = version;
	return ret;
}