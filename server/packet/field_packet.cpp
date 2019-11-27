#include "field_packet.h"
#include "util.h"

using namespace CatDB::Server;

FieldPacket::FieldPacket(const String& field)
	:field_(field)
{

}

int FieldPacket::serialize(char* buffer, int64_t length, int64_t& pos)
{
	Util::store_obstr(buffer, length, "def", pos);
	Util::store_obstr(buffer, length, "", pos);
	Util::store_obstr(buffer, length, "", pos);
	Util::store_obstr(buffer, length, "", pos);
	Util::store_obstr(buffer, length, field_, pos);
	Util::store_obstr(buffer, length, "", pos);
	Util::store_int1(buffer, length, 0xc, pos);
	Util::store_int2(buffer, length, 0x08, pos);
	Util::store_int4(buffer, length, 0x1f, pos);
	Util::store_int1(buffer, length, MYSQL_TYPE_STRING, pos);
	Util::store_int2(buffer, length, 0, pos);
	Util::store_int1(buffer, length, 0x1f, pos);
	Util::store_int2(buffer, length, 0, pos);
	return SUCCESS;
}

uint64_t FieldPacket::get_serialize_size()
{
	return 1020;
}