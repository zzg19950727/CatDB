#ifndef ERROR_PACKET_H
#define ERROR_PACKET_H

#include "packet_header.h"
#include "packet.h"
#include "error.h"
#include "type.h"
#include "log.h"
#include "state.h"

namespace CatDB
{
	namespace Server
	{
		class ErrorPacket : public Packet
		{
		public:
			static const uint64_t SQLSTATE_SIZE = 5;
			static const uint8_t MARKER = '#';
		public:
			ErrorPacket();
			~ErrorPacket();

			int serialize(char* buffer, int64_t len, int64_t& pos);

			uint64_t get_serialize_size();
			virtual int encode(char *buffer, int64_t length, int64_t & pos);

			int set_message(const String& message);

			/**
			* set error code and sql state string.
			*
			* @note OB_ERROR shouldn't passing to this function for
			* hostorical reason.
			*
			* @note It will make a negation and truncate it from 4 bytes
			* (int) to 2 bytes (uint16_t) for fitting msyql protocol.
			*
			* @param errcode oceanbase error code
			*/
			int set_oberrcode(int errcode)
			{
				errcode_ = static_cast<uint16_t>(-errcode);
				return set_sqlstate(State::get_instance().get_jdbc_state(errcode));
			}

		private:
			inline void set_errcode(uint16_t code)
			{
				errcode_ = code;
			}

			int set_sqlstate(const char *sqlstate)
			{
				int ret = SUCCESS;
				if (SQLSTATE_SIZE == strlen(sqlstate))
				{
					sqlstate_ = sqlstate;
				}
				else
				{
					ret = INVALID_ARGUMENT;
				}
				return ret;
			}

			//for test
			inline uint8_t get_field_count() const
			{
				return field_count_;
			}

			inline uint16_t get_err_code() const
			{
				return errcode_;
			}

			inline const char* get_sql_state()
			{
				return sqlstate_;
			}

			inline String& get_message()
			{
				return message_;
			}

		private:
			uint8_t field_count_;   /* Always 0xff */
			uint16_t errcode_;
			const char *sqlstate_;
			String message_;
		};
	}
}

#endif	//ERROR_PACKET_H
