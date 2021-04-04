#include "ok_packet.h"
#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

OKPacket::OKPacket()
	:field_count_(0x00),
	affected_rows_(0),
	insert_id_(0),
	server_status_(0x22),
	warning_count_(0)
{
	//affected_row_ = 0;
	//insert_is_ = 0;
	//memset(this, 0, sizeof(OKPacket));
	//server_status_ = 0x22;//dump MySQL packet get this value
}

OKPacket::~OKPacket()
{
	str_buf_.clear();
}

int OKPacket::set_message(String& message)
{
	str_buf_ = message;
	message_ = message;
	return SUCCESS;
}

int OKPacket::encode(char* buffer, int64_t length, int64_t& pos)
{
	int ret = SUCCESS;
	int64_t start_pos = pos;
	if (NULL == buffer || 0 >= length || pos < 0)
	{
		LOG_ERR("invalid argument buffer=%p, length=%ld, pos=%ld",
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
				LOG_ERR("serialize packet haader size failed, buffer=%p, buffer length=%ld, packet length=%d, pos=%ld",
					buffer, length, pkt_len, start_pos);
			}
			else if (SUCCESS != (ret = Util::store_int1(buffer, length, 2, start_pos)))
			{
				LOG_ERR("serialize packet haader seq failed, buffer=%p, buffer length=%ld, seq number=%d, pos=%ld",
					buffer, length, 2, start_pos);
			}
		}
		else
		{
			LOG_ERR("encode ok packet data failed");
		}
	}
	return ret;
}

uint64_t OKPacket::get_serialize_size()
{
	uint64_t len = 0;
	len += 5; /*1byte field_count + 2bytes server_status + 2bytes warning_count see MySQL protocol*/
	len += 9; /*max length for unit64_t*/
	len += 9; /*max length for store_length*/
	len += 9 + message_.length();
	return len;
}

int OKPacket::serialize(char* buffer, int64_t length, int64_t& pos)
{
	int ret = SUCCESS;

	if (NULL == buffer || 0 >= length || pos < 0)
	{
		LOG_ERR("invalid argument buffer=%p, length=%ld, pos=%ld",
			buffer, length, pos);
		ret = INVALID_ARGUMENT;
	}
	else
	{
		if (SUCCESS != (ret = Util::store_int1(buffer, length, field_count_, pos)))
		{
			LOG_ERR("serialize field_count failed, buffer=%p, length=%ld, field_count=%u,"
				"pos=%ld", buffer, length, field_count_, pos);
		}
		else if (SUCCESS != (ret = Util::store_length(buffer, length, affected_rows_, pos)))
		{
			LOG_ERR("serialize affected_row failed, buffer=%p, length=%ld, affected_rows=%lu,"
				"pos=%ld", buffer, length, affected_rows_, pos);
		}
		else if (SUCCESS != (ret = Util::store_length(buffer, length, insert_id_, pos)))
		{
			LOG_ERR("serialize insert_id failed, buffer=%p, length=%ld, insert_id=%lu,"
				"pos=%ld", buffer, length, insert_id_, pos);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, length, server_status_, pos)))
		{
			LOG_ERR("serialize server_status failed, buffer=%p, length=%ld, server_status=%u,"
				"pos=%ld", buffer, length, server_status_, pos);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, length, warning_count_, pos)))
		{
			LOG_ERR("serialize warning_count failed, buffer=%p, length=%ld, warning_count=%u,"
				"pos=%ld", buffer, length, warning_count_, pos);
		}
		else if (0 != message_.length())
		{
			if (SUCCESS != (ret = Util::store_obstr(buffer, length, message_, pos)))
			{
				LOG_ERR("serialize message failed, buffer=%p, length=%ld, insert_id=%lu,"
					"pos=%ld", buffer, length, insert_id_, pos);
			}
		}
	}
	return ret;
}