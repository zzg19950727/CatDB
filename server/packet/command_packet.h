#ifndef COMMAND_PACKET_H
#define COMMAND_PACKET_H

#include "packet_header.h"
#include "error.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		DECLARE(Request);
		DECLARE(CommandPacket);

		class CommandPacket
		{
			friend class CommandQueue;
		public:
			CommandPacket();
			~CommandPacket();

			String& get_command()
			{
				return command_;
			}

			const String& get_command() const
			{
				return command_;
			}

			const PacketHeader& get_packet_header() const
			{
				return header_;
			}

			inline void set_header(uint32_t pkt_length, uint8_t pkt_seq)
			{
				header_.length_ = pkt_length;
				header_.seq_ = pkt_seq;
			}

			inline void set_type(uint8_t type)
			{
				type_ = type;
			}

			inline uint8_t get_type() const
			{
				return type_;
			}

			int set_request(Request_s req);

			void set_command(char* command, const int32_t length);

			inline Request_s get_request() const
			{
				return req_;
			}

			int32_t get_command_length() const;

			void set_receive_ts(const int64_t &now);
			int64_t get_receive_ts() const;

		private:
			PacketHeader header_;
			uint8_t type_;
			String command_;
			Request_s req_;                 //request pointer for send response
			CommandPacket_s next_;
			int64_t receive_ts_;
		};

		enum CommandType
		{
			Cmd_LOGIN,
			Cmd_LOGOUT,
			Cmd_PREPARE,
			Cmd_EXECUTE,
			Cmd_OTHER
		};

		// used by packet recorder only
		struct CommandPacketRecord
		{
			uint32_t version_;        // version of this struct
			int socket_fd_;
			int cseq_;
			//easy_addr_t addr_;
			uint32_t pkt_length_;         /* MySQL packet length not include packet header */
			uint8_t  pkt_seq_;            /* MySQL packet sequence */
			uint8_t cmd_type_;
			uint8_t obmysql_type_;    /* command type defined by ourself */
			int8_t reserved1_;
			int32_t reserved2_;
			int32_t reserved3_;
			uint64_t stmt_id_;//recode stmt_id when do_com_prepare
			CommandPacketRecord()
				:version_(1), socket_fd_(0),
				cseq_(0), pkt_length_(0),
				pkt_seq_(0), cmd_type_(30), //30 equals COM_END
				obmysql_type_(Cmd_OTHER),
				reserved1_(0),
				reserved2_(0), reserved3_(0),
				stmt_id_(0)
			{
			}
		};
	}
}
#endif		//COMMAND_PACKET_H
