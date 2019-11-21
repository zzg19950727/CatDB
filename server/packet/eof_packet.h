#ifndef EOF_PACKET_H
#define EOF_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		class EofPacket : public Packet
		{
		public:
			EofPacket();
			~EofPacket();

			int serialize(char* buffer, int64_t len, int64_t& pos);

			inline void set_warning_count(uint16_t count)
			{
				warning_count_ = count;
			}

			inline void set_server_status(uint16_t status)
			{
				server_status_ = status;
			}

			virtual uint64_t get_serialize_size();

			//for test
			inline uint8_t get_field_count() const
			{
				return field_count_;
			}

			inline uint16_t get_warning_count() const
			{
				return warning_count_;
			}

			inline uint16_t get_server_status() const
			{
				return server_status_;
			}

		private:
			DISALLOW_COPY_AND_ASSIGN(EofPacket);

			uint8_t field_count_;
			uint16_t warning_count_;
			uint16_t server_status_;
		};
	}
}
#endif	//EOF_PACKET_H
