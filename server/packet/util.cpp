#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

const uint64_t Util::null_value = UINT64_MAX;
const TypeMap Util::type_maps_[T_MAX_TYPE] =
{
	/* MinType */
	{ MYSQL_TYPE_NULL, 0 },
	{ MYSQL_TYPE_TINY, 0 },
	{ MYSQL_TYPE_DOUBLE, 0 },
	{ MYSQL_TYPE_DATETIME, 0 },
	{ MYSQL_TYPE_VAR_STRING, 0 },
	/* MaxType */
};

//TODO avoid coredump if field_index is too large
//http://dev.mysql.com/doc/internals/en/prepared-statements.html#null-bitmap
//offset is 2
void Util::update_null_bitmap(char *&bitmap, int64_t field_index)
{
	int byte_pos = static_cast<int>((field_index + 2) / 8);
	int bit_pos = static_cast<int>((field_index + 2) % 8);
	bitmap[byte_pos] |= static_cast<char>(1 << bit_pos);
}

//called by handle COM_STMT_EXECUTE offset is 0
bool Util::update_from_bitmap(Object_s &param, const char *bitmap, int64_t field_index)
{
	bool ret = false;
	int byte_pos = static_cast<int>(field_index / 8);
	int bit_pos = static_cast<int>(field_index % 8);
	char value = bitmap[byte_pos];
	if (value & (1 << bit_pos))
	{
		ret = true;
		param->set_null();
	}
	return ret;
}

int Util::store_length(char *buf, int64_t len, uint64_t length, int64_t &pos)
{
	int ret = SUCCESS;
	if (len < 0 || pos < 0 || len <= pos)
	{
		ret = SIZE_OVERFLOW;
		LOG_ERR("Store length fail, buffer over flow!", K(len), K(pos), K(length), K(ret));
	}

	int64_t remain = len - pos;
	if (SUCCESS == ret)
	{
		if (length < (uint64_t)251 && remain >= 1)
		{
			ret = store_int1(buf, len, (uint8_t)length, pos);
		}
		/* 251 is reserved for NULL */
		else if (length < (uint64_t)0X10000 && remain >= 3)
		{
			ret = store_int1(buf, len, static_cast<int8_t>(252), pos);
			if (SUCCESS == ret)
			{
				ret = store_int2(buf, len, (uint16_t)length, pos);
				if (SUCCESS != ret)
				{
					pos--;
				}
			}
		}
		else if (length < (uint64_t)0X1000000 && remain >= 4)
		{
			ret = store_int1(buf, len, (uint8_t)253, pos);
			if (SUCCESS == ret)
			{
				ret = store_int3(buf, len, (uint32_t)length, pos);
				if (SUCCESS != ret)
				{
					pos--;
				}
			}
		}
		else if (length < UINT64_MAX && remain >= 9)
		{
			ret = store_int1(buf, len, (uint8_t)254, pos);
			if (SUCCESS == ret)
			{
				ret = store_int8(buf, len, (uint64_t)length, pos);
				if (SUCCESS != ret)
				{
					pos--;
				}
			}
		}
		else if (length == UINT64_MAX) /* null_value == UINT64_MAX */
		{
			ret = store_null(buf, len, pos);
		}
		else
		{
			ret = SIZE_OVERFLOW;
		}
	}

	return ret;
}

int Util::get_length(char *&pos, uint64_t &length)
{
	uint8_t sentinel = 0;
	uint16_t s2 = 0;
	uint32_t s4 = 0;
	int ret = SUCCESS;

	get_uint1(pos, sentinel);
	if (sentinel < 251)
	{
		length = sentinel;
	}
	else if (sentinel == 251)
	{
		length = null_value;
	}
	else if (sentinel == 252)
	{
		get_uint2(pos, s2);
		length = s2;
	}
	else if (sentinel == 253)
	{
		get_uint3(pos, s4);
		length = s4;
	}
	else if (sentinel == 254)
	{
		get_uint8(pos, length);
	}
	else
	{
		// 255??? won't get here.
		pos--;                  // roll back
		ret = INVALID_ARGUMENT;
	}

	return ret;
}

int Util::store_str(char *buf, int64_t len, const char *str, int64_t &pos)
{
	uint64_t length = strlen(str);
	return store_str_v(buf, len, str, length, pos);
}

int Util::store_str_v(char *buf, int64_t len, const char *str,
	const uint64_t length, int64_t &pos)
{
	int ret = SUCCESS;
	int64_t pos_bk = pos;

	if (SUCCESS != (ret = store_length(buf, len, length, pos)))
	{
		LOG_ERR("Store length fail!", K(len), K(pos), K(length), K(ret));
	}
	else if (len >= pos && length <= static_cast<uint64_t>(len - pos))
	{
		memcpy(buf + pos, str, length);
		pos += length;
	}
	else
	{
		pos = pos_bk;        // roll back
		ret = SIZE_OVERFLOW;
	}

	return ret;
}

int Util::store_obstr(char *buf, int64_t len, const String& str, int64_t &pos)
{
	return store_str_v(buf, len, str.c_str(), str.length(), pos);
}

int Util::store_str_zt(char *buf, int64_t len, const char *str, int64_t &pos)
{
	uint64_t length = strlen(str);
	return store_str_vzt(buf, len, str, length, pos);
}

int Util::store_str_vzt(char *buf, int64_t len, const char *str,
	const uint64_t length, int64_t &pos)
{
	int ret = SUCCESS;
	if (len > 0 && pos > 0 && len > pos && static_cast<uint64_t>(len - pos) > length)
	{
		memcpy(buf + pos, str, length);
		pos += length;
		buf[pos++] = '\0';
	}
	else
	{
		ret = SIZE_OVERFLOW;
	}
	return ret;
}

int Util::store_obstr_zt(char *buf, int64_t len, const String& str, int64_t &pos)
{
	return store_str_vzt(buf, len, str.c_str(), str.length(), pos);
}

int Util::store_obstr_nzt(char *buf, int64_t len, const String& str, int64_t &pos)
{
	int ret = SUCCESS;
	if (len > 0 && pos > 0 && len > pos && len - pos >= str.length())
	{
		memcpy(buf + pos, str.c_str(), str.length());
		pos += str.length();
	}
	else
	{
		ret = SIZE_OVERFLOW;
	}
	return ret;
}