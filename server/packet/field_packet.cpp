#include "field_packet.h"
#include "object.h"
using namespace CatDB::Common;
using namespace CatDB::Server;

FieldPacket::FieldPacket(const Object_s  field)
	:field_(field)
{

}

int FieldPacket::serialize(char* buffer, int64_t length, int64_t& pos)
{
	return -1;
}

uint64_t FieldPacket::get_serialize_size()
{
	// @bug
	assert(0);
	return 0;
}