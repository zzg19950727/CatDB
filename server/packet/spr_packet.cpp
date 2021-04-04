#include "spr_packet.h"
#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

SPRPacket::SPRPacket() 
	:status_(0), statement_id_(0),
	column_num_(0), param_num_(0),
	reserved_(0), warning_count_(0)
{

}

SPRPacket::~SPRPacket()
{

}

int SPRPacket::serialize(char* buffer, int64_t len, int64_t& pos)
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
		if (SUCCESS != (ret = Util::store_int1(buffer, len, status_, pos)))
		{
			LOG_ERR("serialize status failed, buffer=%p, len=%ld, status=%u, pos=%ld, ret=%d",
				buffer, len, status_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int4(buffer, len, statement_id_, pos)))
		{
			LOG_ERR("serialize warning_count failed, buffer=%p, len=%ld, statement_id_=%u, pos=%ld, ret=%d",
				buffer, len, statement_id_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, column_num_, pos)))
		{
			LOG_ERR("serialize marker failed, buffer=%p, len=%ld, column_num_=%u, pos=%ld, ret=%d",
				buffer, len, column_num_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, param_num_, pos)))
		{
			LOG_ERR("serialize marker failed, buffer=%p, len=%ld, param_num_=%u, pos=%ld, ret=%d",
				buffer, len, param_num_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int1(buffer, len, reserved_, pos)))
		{
			LOG_ERR("serialize marker failed, buffer=%p, len=%ld, reserved_=%u, pos=%ld, ret=%d",
				buffer, len, reserved_, pos, ret);
		}
		else if (SUCCESS != (ret = Util::store_int2(buffer, len, warning_count_, pos)))
		{
			LOG_ERR("serialize marker failed, buffer=%p, len=%ld, warning_count_=%u, pos=%ld, ret=%d",
				buffer, len, warning_count_, pos, ret);
		}
	}
	return ret;
}

uint64_t SPRPacket::get_serialize_size()
{
	uint64_t len = 0;
	len += 1;                 // status
	len += 4;                 // statement id
	len += 2;                 // column num
	len += 2;                 // param num
	len += 1;                 // reserved
	len += 2;                 // warning count
	return len;
}