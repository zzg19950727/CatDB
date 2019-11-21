#ifndef FIELD_PACKET_H
#define FIELD_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Common {
		DECLARE(Object);
	}
	namespace Server
	{
		using Common::Object_s;
		class FieldPacket : public Packet
		{
		public:
			explicit FieldPacket(const Object_s  field);
			int serialize(char* buffer, int64_t length, int64_t& pos);
			virtual uint64_t get_serialize_size();
		private:
			Object_s field_;
		};
	}
}
#endif	//FIELD_PACKET_H
