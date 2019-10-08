#ifndef OBJECT_H
#define OBJECT_H
#include "type.h"

#define T_NULL		0x80000000
#define T_BOOL		0x1
#define T_NUMBER	0x02
#define T_DATETIME	0x03
#define T_VARCHAR	0x04
#define T_LIST		0x5
#define T_ERROR_RESULT	0x6

namespace CatDB {
	namespace Common {
		DECLARE(Buffer);
		DECLARE(Object);
		DECLARE(RawData);
		//对象序列化后的结构
		class RawData
		{
		public:
			u32 type;		//数据类型
			u32 length;		//数据长度
			u8	data[1];		//真实数据
			u32 size()const;
			static RawData* make_row_data(void* ptr);
		};

		class Object
		{
		protected:
			Object();
		public:
			virtual ~Object();
			static Object_s make_object(const RawData& data);
			static Object_s make_null_object();
			virtual u32 get_type()const;
			virtual u32 width()const;
			virtual bool is_null()const;
			virtual void set_null();
			virtual u32 serialization(u8* & buffer) = 0;
			virtual bool is_fixed_length() = 0;
			virtual bool bool_value() = 0;
			virtual u32 hash();
			virtual String to_string()const;

			//要求实现的类型支持一下所有运算
			//不支持的类型返回对应的错误对象
			virtual Object_s operator+(const Object_s& other);
			virtual Object_s operator-(const Object_s& other);
			virtual Object_s operator*(const Object_s& other);
			virtual Object_s operator/(const Object_s& other);
			virtual Object_s operator==(const Object_s& other);
			virtual Object_s operator>(const Object_s& other);
			virtual Object_s operator<(const Object_s& other);
			virtual Object_s between(const Object_s& left, const Object_s& right);
			virtual Object_s is(const Object_s& other);
			virtual Object_s is_not(const Object_s& other);
			virtual Object_s in(const Object_s& other);
			virtual Object_s not_in(const Object_s& other);
			virtual Object_s and (const Object_s& other);
			virtual Object_s or (const Object_s& other);
			virtual Object_s exists();
			virtual Object_s not_exists();
			virtual Object_s not();
			//自增
			virtual void increase();
			//自身累加
			virtual void accumulate(const Object_s& other);

		protected:
			u32 obj_width;
			u32 table_id;
			u32 column_id;
			u32 obj_type;
		private:
			DISALLOW_COPY_AND_ASSIGN(Object)
		};

		class Error : public Object
		{
		public:
			Error(u32 err_code);
			static Object_s make_object(u32 err_code);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 set_error_code(u32 code);
			u32 get_error_code(u32& code);
			String to_string()const override;

			Object_s and (const Object_s& other) override;
			Object_s or (const Object_s& other) override;
			Object_s not();

		private:
			u32 err_code;
		};

		class Bool : public Object
		{
		public:
			Bool();
			static Object_s make_object(bool value);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 set_value(bool value);
			u32 get_value(bool& value);
			String to_string()const override;

			Object_s operator+(const Object_s& other) override;
			Object_s operator-(const Object_s& other) override;
			Object_s operator*(const Object_s& other) override;
			Object_s operator/(const Object_s& other) override;
			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;

		private:
			bool value;
		};

		class Number : public Object
		{
		public:
			Number(double value);
			Number(const RawData& data);
			static Object_s make_object(double value);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;

			Object_s operator+(const Object_s& other) override;
			Object_s operator-(const Object_s& other) override;
			Object_s operator*(const Object_s& other) override;
			Object_s operator/(const Object_s& other) override;
			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;
			void increase()override;
			void accumulate(const Object_s& other)override;

		private:
			double data;
		};

		class DateTime : public Object
		{
		public:
			DateTime(double value);
			DateTime(const RawData& data);
			static Object_s make_object(double value);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;

			Object_s operator+(const Object_s& other) override;
			Object_s operator-(const Object_s& other) override;
			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;
			void accumulate(const Object_s& other)override;

		private:
			double data;
		};

		class Varchar : public Object
		{
		public:
			Varchar(const Buffer_s& buf);
			Varchar(const RawData& data);
			static Object_s make_object(const Buffer_s& buf);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;

			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;

		private:
			Buffer_s data;
		};
		/*
		class List :public Object
		{
		public:
			List();
			u32 serialization(u8*& buffer);
			bool is_fixed_length();
			u32 add_object(Object_s& object);

		};*/
	}
}
#endif	//OBJECT_H
