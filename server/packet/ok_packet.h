#ifndef OK_PACKET_H
#define OK_PACKET_H

#include "packet_header.h"
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		class OKPacket : public Packet
		{
		public:
			OKPacket();
			~OKPacket();

			/**
			* serialize all data into thread buffer
			* not include packet header
			* called whenever send ok packet except login ok
			*/
			int serialize(char* buffer, int64_t length, int64_t& pos);

			uint64_t get_serialize_size();


			/**
			* This function used to serialize OK packet
			* to thread buffer include packet header
			* called when send login_ok packet
			*/
			virtual int encode(char* buffer, int64_t length, int64_t& pos);

			//int32_t get_encode_size() const;

			int set_message(const String& message);

			inline void set_affected_rows(uint64_t row)
			{
				affected_rows_ = row;
			}

			inline void set_insert_id(uint64_t id)
			{
				insert_id_ = id;
			}

			inline void set_server_status(uint16_t status)
			{
				server_status_ = status;
			}

			inline void set_warning_count(uint16_t warning_count)
			{
				warning_count_ = warning_count;
			}

			//for test
			inline uint8_t get_field_count() const
			{
				return field_count_;
			}

			inline uint64_t get_affected_rows() const
			{
				return affected_rows_;
			}

			inline uint64_t get_insert_id() const
			{
				return insert_id_;
			}

			inline uint16_t get_server_status() const
			{
				return server_status_;
			}

			inline uint16_t get_warning_count() const
			{
				return warning_count_;
			}

			inline String& get_message()
			{
				return message_;
			}

		private:
			DISALLOW_COPY_AND_ASSIGN(OKPacket);

			uint8_t  field_count_;         /* always 0x00 */
			uint64_t affected_rows_;
			uint64_t insert_id_;
			uint16_t server_status_;
			uint16_t warning_count_;
			String message_;
			String str_buf_;
		};
	}
}
#endif	//OK_PACKET_H
