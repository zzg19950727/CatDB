﻿#ifndef OBJECT_H
#define OBJECT_H
#include "my_decimal.h"
#include "type.h"
#include "log.h"

#define T_NULL		0x80
#define T_NULL_TYPE	0x00
#define T_BOOL		0x01
#define T_NUMBER	0x02
#define T_DATETIME	0x03
#define T_VARCHAR	0x04
#define T_MAX_TYPE	0x05
#define T_LIST		0x06
#define T_QUERY_RESULT	0x07
#define T_ERROR_RESULT	0x08

namespace CatDB {
	namespace Common {
		DECLARE(Buffer);
		DECLARE(Object);
		DECLARE(RawData);
		DECLARE(Error);
		DECLARE(Bool);
		DECLARE(Number);
		DECLARE(DateTime);
		DECLARE(Varchar);
		DECLARE(ObjList);

		//对象序列化后的结构
		class RawData
		{
		public:
			u16 length;		//数据长度
			u8 type;		//数据类型
			u8	data[1];		//真实数据
			u16 size()const;
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
			virtual double value() const;
			virtual Object_s copy();
			String type() const;
			operator bool () { return bool_value(); }

			static u8 string_to_type(const String& str);
			static u32 cast_to(u8 type, Object_s& obj);

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
			virtual Object_s in(const Object_s& other);
			virtual Object_s not_in(const Object_s& other);
			virtual Object_s op_and (const Object_s& other);
			virtual Object_s op_or (const Object_s& other);
			virtual Object_s exists();
			virtual Object_s op_not();
			virtual Object_s op_minus();
			virtual Object_s like(const Object_s& other);
			//自增
			virtual void increase();
			//自身累加
			virtual void accumulate(const Object_s& other);
			VIRTUAL_KV_STRING(
						KV(obj_type,type()),
						K(obj_width));

		protected:
			u16 obj_width;
			u8 obj_type;
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
			Object_s copy()override;

			Object_s op_and (const Object_s& other) override;
			Object_s op_or (const Object_s& other) override;
			Object_s op_not()override;
			KV_STRING_OVERRIDE(
					KV(obj_type,type()),
					K(err_code));

		private:
			u32 err_code;
		};

		class Bool : public Object
		{
		public:
			Bool();
			static Object_s make_object(bool value);
			static Object_s make_object(const String& value);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 set_value(bool value);
			u32 get_value(bool& value);
			String to_string()const override;
			Object_s copy()override;

			Object_s operator+(const Object_s& other) override;
			Object_s operator-(const Object_s& other) override;
			Object_s operator*(const Object_s& other) override;
			Object_s operator/(const Object_s& other) override;
			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;
			Object_s exists()override;
			KV_STRING_OVERRIDE(
					KV(obj_type,type()),
					K(value));

		private:
			bool value;
		};

		class Number : public Object
		{
		public:
			Number(double value);
			Number(const String& str);
			Number(const RawData& data);
			static Object_s make_object(double value, int scale = 0);
			static Object_s make_object(const String& value);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			double value() const override;
			String to_string()const override;
			Object_s copy()override;
			void set_scale(int value);

			Object_s operator+(const Object_s& other) override;
			Object_s operator-(const Object_s& other) override;
			Object_s operator*(const Object_s& other) override;
			Object_s operator/(const Object_s& other) override;
			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;
			Object_s op_minus()override;
			Object_s exists()override;
			Object_s between(const Object_s& left, const Object_s& right)override;
			void increase()override;
			void accumulate(const Object_s& other)override;
			KV_STRING_OVERRIDE(
					KV(obj_type, type()), 
					K(scale), 
					KV(data, to_string()));

		private:
			//double data;
			my_decimal data;
			int scale;
		};

		class DateTime : public Object
		{
		public:
			DateTime(double value);
			DateTime(const RawData& data);
			static Object_s make_object(double value);
			static Object_s make_object(const String& str);
			static Object_s make_object_from_day(u32 day);
			static Object_s make_object_from_month(u32 month);
			static Object_s make_object_from_year(u32 year);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;
			Object_s copy()override;
			double value() const override;
			static time_t StringToDatetime(const String& str);
			static String DatetimeToString(time_t t);
			static String CurrentDatetime();
			static String DatetimeToString(time_t t, const String &format);

			Object_s operator+(const Object_s& other) override;
			Object_s operator-(const Object_s& other) override;
			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;
			Object_s exists()override;
			Object_s between(const Object_s& left, const Object_s& right)override;
			void accumulate(const Object_s& other)override;
			KV_STRING_OVERRIDE(
					KV(obj_type,type()), 
					KV(data, to_string()));

		private:
			double data;
		};

		class Varchar : public Object
		{
		public:
			Varchar(const Buffer_s& buf);
			Varchar(const RawData& data);
			Varchar(const String& str);
			Varchar(const char* buf, u32 size);
			static Object_s make_object(const Buffer_s& buf);
			static Object_s make_object(const String& buf);
			static Object_s make_object(const char* buf, u32 size);
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;
			Object_s copy()override;

			Object_s operator==(const Object_s& other) override;
			Object_s operator>(const Object_s& other) override;
			Object_s operator<(const Object_s& other) override;
			Object_s exists()override;
			Object_s between(const Object_s& left, const Object_s& right)override;
			Object_s like(const Object_s& other)override;
			KV_STRING_OVERRIDE(
				KV(obj_type,type()),
				KV(data, to_string()));

		private:
			Buffer_s data;
		};

		class ObjList : public Object
		{
		public:
			ObjList();
			static Object_s make_object();
			void add_object(const Object_s& obj);
			u32 size()const;
			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;

			Object_s in(const Object_s& other)override;
			Object_s not_in(const Object_s& other)override;
			Object_s exists()override;
			KV_STRING_OVERRIDE(
					KV(obj_type,type()),
					K(list));

		private:
			Vector<Object_s> list;
		};
	}
}
#endif	//OBJECT_H
