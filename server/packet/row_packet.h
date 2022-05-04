#ifndef ROW_PACKET_H
#define ROW_PACKET_H
#include "packet.h"
#include "type.h"

namespace CatDB
{
	namespace Common {
		DECLARE(Object);
	}
	namespace Common {
		DECLARE(Row);
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
		 * ���л�һ��cell��buf + pos��λ�á�
		 *
		 * @param [in] obj ��Ҫ���л���cell
		 * @param [in] buf �����buf
		 * @param [in] len buf�Ĵ�С
		 * @param [in,out] pos д��buf��λ��
		 * @param [in] cell index for binary protocol
		 *
		 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
		 */
			int cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos, int64_t cell_index) const;
			/**
			 * ���л�һ��null���͵�cell��buf + pos��λ�á�
			 *
			 * @param [in] obj ��Ҫ���л���cell
			 * @param [in] buf �����buf
			 * @param [in] len buf�Ĵ�С
			 * @param [in,out] pos д��buf��λ��
			 * @param [in] field index
			 *
			 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
			 */
			int null_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos, int64_t cell_index) const;
			/**
			 * ���л�һ�����͵�cell��buf + pos��λ�á�
			 * (ObBoolType, ObIntType)
			 *
			 * @param [in] obj ��Ҫ���л���cell
			 * @param [in] buf �����buf
			 * @param [in] len buf�Ĵ�С
			 * @param [in,out] pos д��buf��λ��
			 *
			 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
			 */
			int int_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			int bool_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * ���л�һ���������͵�cell��buf + pos��λ�á�
			 * (ObDecimalType)
			 *
			 * @param [in] obj ��Ҫ���л���cell
			 * @param [in] buf �����buf
			 * @param [in] len buf�Ĵ�С
			 * @param [in,out] pos д��buf��λ��
			 *
			 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
			 */
			int decimal_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * ���л�һ��datetime�͵�cell��buf + pos��λ�á�
			 * (ObDateTimeType, ObPreciseDateTimeType, ObCreateTimeType, ObModifyTimeType)
			 *
			 * @param [in] obj ��Ҫ���л���cell
			 * @param [in] buf �����buf
			 * @param [in] len buf�Ĵ�С
			 * @param [in,out] pos д��buf��λ��
			 *
			 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
			 */
			int datetime_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * ���л�һ���ַ������͵�cell��buf + pos��λ�á�
			 * (ObVarcharType)
			 *
			 * @param [in] obj ��Ҫ���л���cell
			 * @param [in] buf �����buf
			 * @param [in] len buf�Ĵ�С
			 * @param [in,out] pos д��buf��λ��
			 *
			 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
			 */
			int varchar_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
			/**
			 * ���л�һ���������͵�cell��buf + pos��λ�á�
			 * (ObFloatType, ObDoubleType)
			 *
			 * @param [in] obj ��Ҫ���л���cell
			 * @param [in] buf �����buf
			 * @param [in] len buf�Ĵ�С
			 * @param [in,out] pos д��buf��λ��
			 *
			 * @return �ɹ�����OB_SUCCESS�� ʧ�ܷ���oceanbase error code
			 */
			int number_cell_str(const Object_s &obj, char *buf, const int64_t len, int64_t &pos) const;
		private:
			DISALLOW_COPY_AND_ASSIGN(RowPacket);
			static const int MAX_ROW_SIZE = 16*1024;
			Row_s row_;
			int packet_len;
		};
	}
}
#endif	//ROW_PACKET_H
