#ifndef OBJ_DATETIME_H
#define OBJ_DATETIME_H
#include "my_decimal.h"
#include "object.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Number);
        DECLARE(DateTime);
		class DateTime : public Object
		{
		    friend class Object;
			friend class Varchar;
        private:
            static long long get_seconds(const char* str, u32 size);
			DateTime(const my_decimal& data);
            DateTime(long long seconds);
			DateTime(const u8 *data, u32 size, const DataType& type);
			DateTime(const char *data, u32 size, const DataType& type);
		public:
			static DateTime_s make_object(const String& str);
			u32 init(const u8* buf, u32 size, const DataType& type) override;
			u32 init(const char* buf, u32 size, const DataType& type) override;
			u32 serialization(u8*& buffer)const override;
            OBJ_TYPE get_type()const override;
			u32 width()const override;
			u32 hash() const override;
			double value() const override;
			String to_string()const override;
			u32 compare(const Object_s& rhs, int& res)const override;
			u32 cast_to(const DataType& type, Object_s &res) override;

            static bool is_valid_datetime(const String& str);
			static u32 current_datetime(DateTime_s &cur);
            static u32 format_datetime(const DateTime_s &time, const String &format, String &str);
            static u32 add_seconds(const DateTime_s &time, const Number_s& seconds, DateTime_s &res);
			static u32 sub_seconds(const DateTime_s &time, const Number_s& seconds, DateTime_s &res);
			static u32 make_second_from_day(u32 day, Number_s& seconds);
			static u32 make_second_from_month(u32 month, Number_s& seconds);
			static u32 make_second_from_year(u32 year, Number_s& seconds);

			KV_STRING_OVERRIDE(
					KV(data, to_string()));

		private:
			u32 inner_format_datetime(const String &format, String &str) const;
		private:
			my_decimal data;
			TimeType type;
		};
	}
}
#endif	//OBJ_DATETIME_H
