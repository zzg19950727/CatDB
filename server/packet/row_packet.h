#ifndef ROW_PACKET_H
#define ROW_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Common {
		DECLARE(Row);
	}
	namespace Server
	{
		using Common::Row_s;
		class RowPacket : public Packet
		{
		public:
			RowPacket(const Row_s& row);
			int serialize(char* buffer, int64_t length, int64_t& pos);
			virtual uint64_t get_serialize_size();
		private:
			DISALLOW_COPY_AND_ASSIGN(RowPacket);
			Row_s row_;
		};
	}
}
#endif	//ROW_PACKET_H
