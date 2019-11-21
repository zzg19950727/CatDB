#include "packet.h"
#include "util.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Server;

int Packet::encode(char* buffer, int64_t length, int64_t& pos)
{
	int ret = SUCCESS;
	int64_t start_pos = pos;
	if (NULL == buffer || 0 >= length || pos < 0)
	{
		Log(LOG_ERR, "Packet", "invalid argument buffer=%p, length=%ld, pos=%ld",
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
				Log(LOG_ERR, "Packet", "serialize packet haader size failed, buffer=%p, buffer length=%ld, packet length=%d, pos=%ld",
					buffer, length, pkt_len, start_pos);
			}
			else if (SUCCESS != (ret = Util::store_int1(buffer, length, header_.seq_, start_pos)))
			{
				Log(LOG_ERR, "Packet", "serialize packet haader seq failed, buffer=%p, buffer length=%ld, seq number=%u, pos=%ld",
					buffer, length, header_.seq_, start_pos);
			}
		}
		else
		{
			Log(LOG_ERR, "Packet", "encode packet data failed, ret is %d", ret);
		}

		if (SUCCESS != ret)
		{
			pos = start_pos;
		}
	}
	return ret;
}
