#ifndef OBJ_VARCHAR_H
#define OBJ_VARCHAR_H
#include "object.h"
#include "type.h"

namespace CatDB {
	namespace Common {
        DECLARE(Number);
		DECLARE(Varchar);
		class Varchar : public Object
		{
		    friend class Object;
		private:
			Varchar(const String& str);
			Varchar(const char* str, u32 len);
			Varchar(const char* buf, u32 size, const DataType& type);
		public:	
			~Varchar();
			static Varchar_s make_object(const char* str, u32 len);
			static Varchar_s make_object(const String& value);
			u32 init(const u8* buf, u32 size, const DataType& type) override;
			u32 init(const char* buf, u32 size, const DataType& type) override;
			u32 serialization(u8*& buffer) const override;
			u32 hash()const override;
			String to_string()const override;
            OBJ_TYPE get_type()const override;
			u32 width()const override;
			u32 compare(const Object_s& rhs, int& res)const override;
			u32 cast_to(const DataType& type, Object_s &res) override;

			static u32 like(const Varchar_s& str, const String& pattern, const String& escape, bool &res);
            static u32 substr(const Varchar_s& str, u32 pos, u32 size, Varchar_s &res);
            static u32 str_length(const Varchar_s& str, Number_s &len);
            
			KV_STRING_OVERRIDE(
                KV(data, to_string())
            );

		private:
            u64 length;
			char* data;
		};
	}
}
#endif	//OBJ_VARCHAR_H
