#ifndef RESHEADER_PACKET_H
#define RESHEADER_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		class ResheaderPacket : public Packet
		{
		public:
			ResheaderPacket();
			~ResheaderPacket();

			int serialize(char* buffer, int64_t len, int64_t& pos);
			uint64_t get_serialize_size();
			inline void set_field_count(uint64_t count)
			{
				field_count_ = count;
			}

			//for test
			inline uint64_t get_field_count() const
			{
				return field_count_;
			}

		private:
			DISALLOW_COPY_AND_ASSIGN(ResheaderPacket);

			uint64_t field_count_;
		};
	}
}
#endif	//RESHEADER_PACKET_H
