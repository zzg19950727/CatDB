#ifndef SPR_PACKET_H
#define SPR_PACKET_H

#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		/**
		* This packet is response to COM_STMT_PREPARE
		* following with param desc && column desc packets
		*
		*  status (1) -- [00] OK
		*  statement_id (4) -- statement-id
		*  num_columns (2) -- number of columns
		*  num_params (2) -- number of params
		*  reserved_1 (1) -- [00] filler
		*  warning_count (2) -- number of warnings
		*/
		class SPRPacket : public Packet
		{
		public:
			SPRPacket();
			~SPRPacket();


			int serialize(char* buffer, int64_t length, int64_t& pos);
			virtual uint64_t get_serialize_size();

			inline void set_statement_id(uint32_t id)
			{
				statement_id_ = id;
			}

			inline void set_column_num(uint16_t num)
			{
				column_num_ = num;
			}

			inline void set_param_num(uint16_t num)
			{
				param_num_ = num;
			}

			inline void set_warning_count(uint16_t count)
			{
				warning_count_ = count;
			}


		private:
			DISALLOW_COPY_AND_ASSIGN(SPRPacket);
			uint8_t  status_;
			uint32_t statement_id_;
			uint16_t column_num_;
			uint16_t param_num_;
			uint8_t  reserved_;
			uint16_t warning_count_;
		};
	}
}
#endif	//SPR_PACKET_H
