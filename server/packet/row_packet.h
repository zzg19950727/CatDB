#ifndef ROW_PACKET_H
#define ROW_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Server
	{
		using Common::Row_s;
		using Common::Object_s;
		class RowPacket : public Packet
		{
		public:
			RowPacket(const Row_s& row);
			int serialize(char* buffer, int64_t length, int64_t& pos);
			virtual uint64_t get_serialize_size();
		private:
			/**
		 * 序列化一个cell到buf + pos的位置。
		 *
		 * @param [in] obj 需要序列化的cell
		 * @param [in] buf 输出的buf
		 * @param [in] len buf的大小
		 * @param [in,out] pos 写入buf的位置
		 * @param [in] cell index for binary protocol
		 *
		 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
		 */
			int cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos, int64_t cell_index) const;
			/**
			 * 序列化一个null类型的cell到buf + pos的位置。
			 *
			 * @param [in] obj 需要序列化的cell
			 * @param [in] buf 输出的buf
			 * @param [in] len buf的大小
			 * @param [in,out] pos 写入buf的位置
			 * @param [in] field index
			 *
			 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
			 */
			int null_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos, int64_t cell_index) const;
			/**
			 * 序列化一个整型的cell到buf + pos的位置。
			 * (ObBoolType, ObIntType)
			 *
			 * @param [in] obj 需要序列化的cell
			 * @param [in] buf 输出的buf
			 * @param [in] len buf的大小
			 * @param [in,out] pos 写入buf的位置
			 *
			 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
			 */
			int int_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			int bool_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * 序列化一个定点数型的cell到buf + pos的位置。
			 * (ObDecimalType)
			 *
			 * @param [in] obj 需要序列化的cell
			 * @param [in] buf 输出的buf
			 * @param [in] len buf的大小
			 * @param [in,out] pos 写入buf的位置
			 *
			 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
			 */
			int decimal_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * 序列化一个datetime型的cell到buf + pos的位置。
			 * (ObDateTimeType, ObPreciseDateTimeType, ObCreateTimeType, ObModifyTimeType)
			 *
			 * @param [in] obj 需要序列化的cell
			 * @param [in] buf 输出的buf
			 * @param [in] len buf的大小
			 * @param [in,out] pos 写入buf的位置
			 *
			 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
			 */
			int datetime_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * 序列化一个字符串类型的cell到buf + pos的位置。
			 * (ObVarcharType)
			 *
			 * @param [in] obj 需要序列化的cell
			 * @param [in] buf 输出的buf
			 * @param [in] len buf的大小
			 * @param [in,out] pos 写入buf的位置
			 *
			 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
			 */
			int varchar_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * 序列化一个浮点数型的cell到buf + pos的位置。
			 * (ObFloatType, ObDoubleType)
			 *
			 * @param [in] obj 需要序列化的cell
			 * @param [in] buf 输出的buf
			 * @param [in] len buf的大小
			 * @param [in,out] pos 写入buf的位置
			 *
			 * @return 成功返回OB_SUCCESS， 失败返回oceanbase error code
			 */
			int number_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
		private:
			DISALLOW_COPY_AND_ASSIGN(RowPacket);
			Row_s row_;
		};
	}
}
#endif	//ROW_PACKET_H
