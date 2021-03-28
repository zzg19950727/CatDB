#include "field_packet.h"
#include "util.h"

using namespace CatDB::Server;

FieldPacket::FieldPacket(const String& field)
	:field_(field),
	type(T_VARCHAR)
{

}

void FieldPacket::set_type(u32 type)
{
	this->type = type;
}

int FieldPacket::serialize(char* buffer, int64_t length, int64_t& pos)
{
	EMySQLFieldType mysql_type;
	uint8_t num_decimals;
	uint32_t len;
	Util::get_mysql_type(type, mysql_type, num_decimals, len);
	//catlog: always "def"
	Util::store_obstr(buffer, length, "def", pos);
	//database name
	Util::store_obstr(buffer, length, "", pos);
	//table alias name
	Util::store_obstr(buffer, length, "", pos);
	//table ori name
	Util::store_obstr(buffer, length, "", pos);
	//field alias name
	Util::store_obstr(buffer, length, field_, pos);
	//field ori name
	Util::store_obstr(buffer, length, "", pos);
	//fill_1
	Util::store_int1(buffer, length, 0xc, pos);
	//character code
	Util::store_int2(buffer, length, 0x08, pos);
	//field length
	Util::store_int4(buffer, length, len, pos);
	//field type
	Util::store_int1(buffer, length, mysql_type, pos);
	//field flag
	Util::store_int2(buffer, length, 0, pos);
	//nums decimal
	Util::store_int1(buffer, length, num_decimals, pos);
	//fill_2
	Util::store_int2(buffer, length, 0, pos);
	Util::store_length(buffer, length, 0, pos);
	//Util::store_obstr_zt(buffer, len, "NULL", pos);
	return SUCCESS;
}

uint64_t FieldPacket::get_serialize_size()
{
	return 1020;
}
