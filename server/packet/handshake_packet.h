#ifndef HANDSHAKE_PACKET_H
#define HANDSHAKE_PACKET_H

#include "packet_header.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		class HandshakePacket
		{
		public:
			static const int32_t SCRAMBLE_SIZE = 8;
			static const int32_t PLUGIN_SIZE = 13;
			static const int32_t PLUGIN2_SIZE = 12;

		public:
			HandshakePacket();

			//TODO use ob server info to init handshake packet
			//HandshakePacket(ServerInfo& info);
			~HandshakePacket();


			int set_server_version(const String& version);

			inline void set_server_capability(uint16_t capability)
			{
				server_capabilities_ = capability;
			}

			inline void set_server_status(uint16_t status)
			{
				server_status_ = status;
			}

			inline void set_server_language(uint8_t language)
			{
				server_language_ = language;
			}

			/**
			* Serialize all data not include packet header to buffer
			* @param buffer  buffer
			* @param len     buffer length
			* @param pos     buffer pos
			*/
			int serialize(char* buffer, int64_t len, int64_t& pos);

			void set_thread_id(const uint32_t id);

			/**
			* Get packet serialize size not include
			* packet header
			*/
			int32_t get_serialize_size();

		private:
			int serialize_header(char* buffer, int64_t length, uint32_t pkt_length, int64_t& pos);

		private:
			PacketHeader header_;
			uint8_t protocol_version_;
			String server_version_;// human-readable server version
			uint32_t thread_id_;// connection_id
			char scramble_buff_[8];// auth_plugin_data_part_1 : first 8 bytes of the auth-plugin data
			char filler_;                  /* always 0x00 */
			uint16_t server_capabilities_;  /* set value to use 4.1protocol */
			uint8_t server_language_;
			uint16_t server_status_;
			char plugin_[13];        /* always 0x00 */
			char plugin2_[12];
			char terminated_;
		};

		inline void HandshakePacket::set_thread_id(const uint32_t id)
		{
			thread_id_ = id;
		}
	}
}
#endif	//HANDSHAKE_PACKET_H
