#include "error_packet.h"
#include "util.h"
#include "log.h"

using namespace CatDB::Server;

ErrorPacket::ErrorPacket() 
	:field_count_(0xff)
{
	errcode_ = 1007;
	sqlstate_ = "01007";
	message_ = "test messge";
}

ErrorPacket::~ErrorPacket()
{
}

int ErrorPacket::encode(char* buffer, int64_t length, int64_t& pos)
{
	int ret = SUCCESS;
	int64_t start_pos = pos;
	if (NULL == buffer || 0 >= length || pos < 0)
	{
		LOG_ERR("invalid argument", K(length), K(pos));
		ret = INVALID_ARGUMENT;
	}
	else
	{
		pos += PACKET_HEADER_SIZE;
		ret = serialize(buffer, length, pos);
		if (SUCCESS == ret)
		{
			uint32_t pkt_len = static_cast<uint32_t>(pos - start_pos - PACKET_HEADER_SIZE);
			if (SUCCESS != (ret = Util::store_int3(buffer, length, pkt_len, start_pos)))
			{
				LOG_ERR("serialize packet haader size failed", K(length), K(pkt_len), K(start_pos));
			}
			else if (SUCCESS != (ret = Util::store_int1(buffer, length, 2, start_pos)))
			{
				LOG_ERR("serialize packet haader seq failed,seq number=2", K(length), K(start_pos));
			}
		}
		else
		{
			LOG_ERR("encode error packet data failed");
		}
	}
	return ret;

}
int ErrorPacket::serialize(char* buffer, int64_t len, int64_t& pos)
{
	int ret = SUCCESS;

	if (NULL == buffer || 0 >= len || pos < 0)
	{
		LOG_ERR("invalid argument", K(len), K(pos));
		ret = INVALID_ARGUMENT;
	}
	else
	{
		if (SUCCESS != (ret = Util::store_int1(buffer, len, field_count_, pos)))
		{
			LOG_ERR("serialize field_count failed", K(len), K(field_count_), K(pos));
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, errcode_, pos)))
		{
			LOG_ERR("serialize errcode failed", K(len), K(errcode_), K(pos));
		}
		else if (SUCCESS != (ret = Util::store_int1(buffer, len, MARKER, pos)))
		{
			LOG_ERR("serialize marker failed, marker=#", K(len), K(pos));
		}

		if (SUCCESS == ret)
		{
			if ((buffer + pos + SQLSTATE_SIZE) < buffer + len)
			{
				memcpy(buffer + pos, sqlstate_, SQLSTATE_SIZE);
				pos += SQLSTATE_SIZE;
			}
			else
			{
				LOG_ERR("not enough buffer to serialize sqlstate", K(len), K(pos));
				ret = ERR_UNEXPECTED;
			}
		}

		if (SUCCESS == ret)
		{
			ret = Util::store_obstr_nzt(buffer, len, message_, pos);
			if (SUCCESS != ret)
			{
				LOG_ERR("serialize message failed", K(len), message_, K(pos));
			}
		}
	}
	return ret;
}

uint64_t ErrorPacket::get_serialize_size()
{
	uint64_t len = 0;
	len += 9;/*1byte field_count + 2bytes errno + 1byte sqlmarker + 5bytes sqlstate*/
	len += MAX_STORE_LENGTH + message_.length();
	return len;
}

int ErrorPacket::set_message(const String& message)
{
	int ret = SUCCESS;
	message_ = message;
	return ret;
}