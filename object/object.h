#ifndef OBJECT_H
#define OBJECT_H
#include "print_helper.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
		class Object
		{
		protected:
			Object();
		public:
			virtual ~Object();
			//for raw data
			static Object_s make_object(const u8* buf, u32 size, const DataType& type);
			//for string data
			static Object_s make_object(const char* str, u32 size, const DataType& type);
			static Object_s make_null_object();
			static Object_s make_null_object(const DataType& type);
			virtual OBJ_TYPE get_type()const = 0;
			virtual u32 width()const = 0;
			virtual u32 hash() const = 0;
			virtual String to_string()const = 0;
			virtual double value() const { return 0; }
			virtual u32 serialization(u8* & buffer) const = 0;
			virtual u32 compare(const Object_s& rhs, int& res) const = 0;
			virtual u32 cast_to(const DataType& type, Object_s &res) = 0;
			inline bool is_null()const { return is_null_obj; }
			inline void set_null(const bool value = true) { is_null_obj = value; }
			VIRTUAL_KV_STRING("");	

		protected:
			bool is_null_obj;
		private:
			DISALLOW_COPY_AND_ASSIGN(Object)
		};
	}
}
#endif	//OBJECT_H
