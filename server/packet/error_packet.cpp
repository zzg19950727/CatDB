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
		Log(LOG_ERR,"ErrorPacket", "invalid argument buffer=%p, length=%ld, pos=%ld",
			buffer, length, pos);
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
				Log(LOG_ERR,"ErrorPacket", "serialize packet haader size failed, buffer=%p, buffer length=%ld, packet length=%d, pos=%ld",
					buffer, length, pkt_len, start_pos);
			}
			else if (SUCCESS != (ret = Util::store_int1(buffer, length, 2, start_pos)))
			{
				Log(LOG_ERR,"ErrorPacket", "serialize packet haader seq failed, buffer=%p, buffer length=%ld, seq number=%d, pos=%ld",
					buffer, length, 2, start_pos);
			}
		}
		else
		{
			Log(LOG_ERR,"ErrorPacket", "encode error packet data failed");
		}
	}
	return ret;

}
int ErrorPacket::serialize(char* buffer, int64_t len, int64_t& pos)
{
	int ret = SUCCESS;

	if (NULL == buffer || 0 >= len || pos < 0)
	{
		Log(LOG_ERR,"ErrorPacket", "invalid argument buffer=%p, length=%ld, pos=%ld",
			buffer, len, pos);
		ret = INVALID_ARGUMENT;
	}
	else
	{
		if (SUCCESS != (ret = Util::store_int1(buffer, len, field_count_, pos)))
		{
			Log(LOG_ERR,"ErrorPacket", "serialize field_count failed, buffer=%p, len=%ld, field_count_=%u, pos=%ld",
				buffer, len, field_count_, pos);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, errcode_, pos)))
		{
			Log(LOG_ERR,"ErrorPacket", "serialize errcode failed, buffer=%p, len=%ld, errcode=%u, pos=%ld",
				buffer, len, errcode_, pos);
		}
		else if (SUCCESS != (ret = Util::store_int1(buffer, len, MARKER, pos)))
		{
			Log(LOG_ERR,"ErrorPacket", "serialize marker failed, buffer=%p, len=%ld, marker=%c, pos=%ld",
				buffer, len, '#', pos);
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
				Log(LOG_ERR,"ErrorPacket", "not enough buffer to serialize sqlstate, buffer=%p, len=%ld,"
					"sqlstate length=%ld,pos=%ld", buffer, len, SQLSTATE_SIZE, pos);
				ret = ERR_UNEXPECTED;
			}
		}

		if (SUCCESS == ret)
		{
			ret = Util::store_obstr_nzt(buffer, len, message_, pos);
			if (SUCCESS != ret)
			{
				Log(LOG_ERR,"ErrorPacket", "serialize message failed, buffer=%p, len=%ld, message length=%d,"
					"pos=%ld", buffer, len, message_.length(), pos);
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