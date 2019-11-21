#ifndef PACKET_HEADER_H
#define PACKET_HEADER_H
#include <stdint.h>

namespace CatDB
{
	namespace Server
	{
		class PacketHeader
		{
		public:
			PacketHeader() : length_(0), seq_(0)
			{
			}
		public:
			uint32_t length_;         /* MySQL packet length not include packet header */
			uint8_t  seq_;            /* MySQL packet sequence */
		};
	}
}
#endif	//PACKET_HEADER_H
