#include "eof_packet.h"
#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

EofPacket::EofPacket()
	:field_count_(0xfe),
	warning_count_(0),
	server_status_(0)
{

}

EofPacket::~EofPacket()
{

}

int EofPacket::serialize(char* buffer, int64_t len, int64_t& pos)
{
	int ret = SUCCESS;

	if (NULL == buffer || len <= 0 || pos < 0)
	{
		Log(LOG_ERR,"EofPacket", "invalid argument buffer=%p, len=%ld, pos=%ld",
			buffer, len, pos);
		ret = INVALID_ARGUMENT;
	}
	else
	{
		if (SUCCESS != (ret = Util::store_int1(buffer, len, field_count_, pos)))
		{
			Log(LOG_ERR, "EofPacket", "serialize field_count failed, buffer=%p, len=%ld, field_count=%u, pos=%ld, ret=%d",
				buffer, len, field_count_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, warning_count_, pos)))
		{
			Log(LOG_ERR, "EofPacket", "serialize warning_count failed, buffer=%p, len=%ld, warning_count=%u, pos=%ld, ret=%d",
				buffer, len, warning_count_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, server_status_, pos)))
		{
			Log(LOG_ERR, "EofPacket", "serialize marker failed, buffer=%p, len=%ld, marker=%c, pos=%ld, ret=%d",
				buffer, len, server_status_, pos, ret);
		}
	}
	return ret;
}

uint64_t EofPacket::get_serialize_size()
{
	uint64_t len = 0;
	len += 1;                 // field_count_
	len += 2;                 // warning_count_
	len += 2;                 // server_status_
	return len;
}