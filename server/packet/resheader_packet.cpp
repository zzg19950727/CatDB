#include "resheader_packet.h"
#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

ResheaderPacket::ResheaderPacket() : field_count_(0)
{

}

ResheaderPacket::~ResheaderPacket()
{

}

int ResheaderPacket::serialize(char* buffer, int64_t len, int64_t& pos)
{
	int ret = SUCCESS;

	if (NULL == buffer || len <= 0 || pos < 0)
	{
		LOG_ERR("invalid argument buffer=%p, len=%ld, pos=%ld",
			buffer, len, pos);
		ret = INVALID_ARGUMENT;
	}
	else
	{
		if (SUCCESS != (ret = Util::store_length(buffer, len, field_count_, pos)))
		{
			LOG_ERR("serialize field_count failed, buffer=%p, len=%ld, field_count=%lu, pos=%ld, ret=%d",
				buffer, len, field_count_, pos, ret);
		}
	}
	return ret;
}

uint64_t ResheaderPacket::get_serialize_size()
{
	uint64_t len = 0;
	len += MAX_STORE_LENGTH; // field_count_
	return len;
}