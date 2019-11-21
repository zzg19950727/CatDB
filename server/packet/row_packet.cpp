#include "row_packet.h"
#include "object.h"
#include "error.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Server;

RowPacket::RowPacket(const Row_s& row)
	:row_(row)
{

}

int RowPacket::serialize(char* buffer, int64_t length, int64_t& pos)
{
	return SUCCESS;
}

uint64_t RowPacket::get_serialize_size()
{
	// @bug
	assert(0);
	return 0;
}