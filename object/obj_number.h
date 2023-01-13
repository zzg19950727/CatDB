#ifndef OBJECT_NUMBER_H
#define OBJECT_NUMBER_H
#include "my_decimal.h"
#include "object.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Number);
		DECLARE(DateTime);
		class Number : public Object
		{
			friend class Object;
			friend class DateTime;
			friend class Varchar;
		private:
			Number(const my_decimal& data);
			Number(f64 value);
			Number(longlong value);
			Number(const String& str);
			Number(const u8* buf, u32 size, const DataType& type);
			Number(const char* buf, u32 size, const DataType& type);
			static Number_s make_object(const my_decimal& data);
		public:	
			static Number_s make_float_object(f64 value);
			static Number_s make_int_object(longlong value);
			static Number_s make_object(const String& value);
			static Number_s make_object(Number_s value);
			u32 init(const u8* buf, u32 size, const DataType& type) override;
			u32 init(const char* buf, u32 size, const DataType& type) override;
			u32 init(longlong value);
			u32 serialization(u8*& buffer) const override;
			u32 hash()const override;
			String to_string()const override;
			double value() const override;
            OBJ_TYPE get_type()const override;
			u32 width()const override;
			u32 accumulate(Number_s rhs);
			u32 compare(const Object_s& rhs, int& res)const override;
			u32 cast_to(const DataType& type, Object_s &res) override;

			static u32 num_add(const Number_s& lhs, const Number_s& rhs, Number_s& res);
			static u32 num_sub(const Number_s& lhs, const Number_s& rhs, Number_s& res);
			static u32 num_mul(const Number_s& lhs, const Number_s& rhs, Number_s& res);
			static u32 num_div(const Number_s& lhs, const Number_s& rhs, Number_s& res);
            static u32 num_mod(const Number_s& lhs, const Number_s& rhs, Number_s& res);
			static u32 num_minus(const Number_s& lhs, Number_s& res);
			static bool is_valid_number(const String& value);
            
			KV_STRING_OVERRIDE(
                KV(data, to_string())
            );

		private:
			my_decimal data;
		};
	}
}
#endif	//OBJECT_NUMBER_H
