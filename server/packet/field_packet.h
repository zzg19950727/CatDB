#ifndef FIELD_PACKET_H
#define FIELD_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		class FieldPacket : public Packet
		{
		public:
			explicit FieldPacket(const String&  field);
			int serialize(char* buffer, int64_t length, int64_t& pos);
			virtual uint64_t get_serialize_size();
		private:
			String field_;
		};
	}
}
#endif	//FIELD_PACKET_H
