#include "row_packet.h"
#include "object.h"
#include "error.h"
#include "util.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Server;

RowPacket::RowPacket(const Row_s& row)
	:row_(row)
{

}

int RowPacket::serialize(char* buffer, int64_t length, int64_t& pos)
{
	for (u32 i = 0; i < row_->get_cell_num(); ++i)
	{
		Object_s cell;
		row_->get_cell(i, cell);
		cell_str(cell, buffer, length, pos, i);
	}
	return SUCCESS;
}

uint64_t RowPacket::get_serialize_size()
{
	return 1020;
}

int RowPacket::cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos, int64_t cell_index) const
{
	int ret = SUCCESS;
	if (obj->is_null()) {
		return null_cell_str(obj, buf, len, pos, cell_index);
	}
	switch (obj->get_type())
	{
	case T_DATETIME:
		ret = datetime_cell_str(obj, buf, len, pos);
		break;
	case T_NUMBER:
		ret = number_cell_str(obj, buf, len, pos);
		break;
	case T_VARCHAR:
		ret = varchar_cell_str(obj, buf, len, pos);
		break;
	default:
		LOG_ERR("invalid data type", K(obj));
		Util::store_obstr(buf, len, obj->to_string(), pos);
		break;
	}
	return ret;
}

int RowPacket::null_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos, int64_t cell_index) const
{
	int ret = SUCCESS;
	if (len - pos <= 0)
	{
		ret = SIZE_OVERFLOW;
	}
	else
	{
		ret = Util::store_null(buf, len, pos);
	}
	return ret;
}

int RowPacket::int_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos) const
{
	int ret = SUCCESS;
	if (len - pos < 29)
	{
		ret = SIZE_OVERFLOW;
	}
	else
	{
		Util::store_obstr(buf, len, obj->to_string(), pos);
	}
	return ret;
}

int RowPacket::bool_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos) const
{
	bool bool_val = false;
	int ret = SUCCESS;
	uint64_t length = 0;
	if (len - pos < 2)
	{
		ret = SIZE_OVERFLOW;
	}
	else
	{
		bool_val = obj->value();
		/* skip 1 byte to store length */
		length = snprintf(buf + pos + 1, len - pos - 1, "%d", bool_val ? 1 : 0);
		Util::store_length(buf, len, length, pos);
		pos += length;
	}
	return ret;
}

int RowPacket::decimal_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos) const
{
	int ret = SUCCESS;
	if (len - pos < 29)
	{
		ret = SIZE_OVERFLOW;
	}
	else
	{
		Util::store_obstr(buf, len, obj->to_string(), pos);
	}
	return ret;
}

int RowPacket::datetime_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos) const
{
	int ret = SUCCESS;
	if (len - pos < 21)
	{
		ret = SIZE_OVERFLOW;
	}
	else
	{
		Util::store_obstr(buf, len, obj->to_string(), pos);
	}
	return ret;
}

int RowPacket::varchar_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos) const
{
	String str = obj->to_string();
	int ret = SUCCESS;
	uint64_t length;
	int64_t pos_bk = pos;

	if (str.length() < len - pos)
	{
		length = static_cast<uint64_t>(str.length());
		if ((ret = Util::store_length(buf, len, length, pos)) == SUCCESS)
		{
			if (len - pos >= str.length())
			{
				memcpy(buf + pos, str.c_str(), length);
				pos += length;
			}
			else
			{
				pos = pos_bk;
				ret = SIZE_OVERFLOW;
			}
		}
		else
		{
			LOG_ERR("serialize data len failed len = %lu", length);
		}
	}
	else
	{
		ret = SIZE_OVERFLOW;
	}
	return ret;
}

int RowPacket::number_cell_str(const Object_s & obj, char * buf, const int64_t len, int64_t & pos) const
{
	int ret = SUCCESS;
	if (len - pos < 29)
	{
		ret = SIZE_OVERFLOW;
	}
	else
	{
		Util::store_obstr(buf, len, obj->to_string(), pos);
	}
	return ret;
}
