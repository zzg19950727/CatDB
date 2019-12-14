#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
/*
当前计算框架会吞掉最原始的错误码，但是发生错误，最终的计算结果依然是ErrorObject
*/

u16 RawData::size() const
{
	return sizeof(RawData) + length;
}

RawData * RawData::make_row_data(void * ptr)
{/*
 RawData* rd = reinterpret_cast<RawData*>(ptr);
 rd->data = reinterpret_cast<u8*>(rd) + sizeof(RawData);
 return rd;*/
	return reinterpret_cast<RawData*>(ptr);
}

Object::Object()
	:obj_width(0),
	obj_type(T_NULL)
{

}

Object::~Object()
{
}

Object_s Object::make_object(const RawData & data)
{
	switch (data.type)
	{
	case T_NUMBER:
		return Object_s(new Number(data));
	case T_DATETIME:
		return Object_s(new DateTime(data));
	case T_VARCHAR:
		return Object_s(new Varchar(data));
	default:
		return make_null_object();
	}
}

Object_s Object::make_null_object()
{
	Object_s null = Bool::make_object(false);
	null->set_null();
	return null;
}

u32 Object::get_type() const
{
	return obj_type;
}

u32 Object::width() const
{
	return obj_width;
}

bool Object::is_null() const
{
	return (obj_type & T_NULL) != 0;
}

void Object::set_null()
{
	obj_type |= T_NULL;
}

u32 Object::hash()
{
	return 0;
}

String Object::to_string() const
{
	return String();
}

Object_s Object::copy()
{
	return Error::make_object(ERR_EXPR_TYPE);
}

Object_s Object::operator+(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support add operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator-(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support sub operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator*(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support mul operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator/(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support div operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator==(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support equal operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator>(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support greater operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator<(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support less operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::between(const Object_s & left, const Object_s & right)
{
	Log(LOG_ERR, "Object", "object %u do not support between operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::in(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support in operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::not_in(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support not in operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object:: op_and (const Object_s & other)
{
	if (is_null() || other->is_null()){
		return Object::make_null_object();
	}else{
		return Bool::make_object( bool_value() && other->bool_value());
	}
}

Object_s Object:: op_or (const Object_s & other)
{
	return Bool::make_object(bool_value() || other->bool_value());
}

Object_s Object::exists()
{
	Log(LOG_ERR, "Object", "object %u do not support exists operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::op_not()
{
	if (is_null()){
		return Object::make_null_object();
	}else{
		return Bool::make_object(!bool_value());
	}
}

Object_s Object::like(const Object_s & other)
{
	Log(LOG_ERR, "Object", "object %u do not support like operation", obj_type);
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

void Object::increase()
{
	Log(LOG_ERR, "Object", "object %u do not support increase operation", obj_type);
}

void Object::accumulate(const Object_s& other)
{
	Log(LOG_ERR, "Object", "object %u do not support accumulate operation", obj_type);
}

u8 CatDB::Common::string_to_type(const String & str)
{
	u8 type = T_NULL;
	if (str == "number") {
		type = T_NUMBER;
	}
	else if (str == "datetime") {
		type = T_DATETIME;
	}
	else if (str == "varchar") {
		type = T_VARCHAR;
	}
	return type;
}

/*目前支持NULL值转换为对应类型的NULL，varchar转datetime*/
u32 CatDB::Common::cast_to(u8 type, Object_s & obj)
{
	if (!obj) {
		return CAST_DATA_ERROR;
	}
	else if (obj->get_type() == type) {
		return SUCCESS;
	}
	else if (obj->is_null()) {
		switch (type)
		{
		case T_NUMBER:
			obj = Object_s(new Number(0));
			break;
		case T_DATETIME:
			obj = Object_s(new DateTime(0));
			break;
		case T_VARCHAR:
			obj = Object_s(new Varchar(""));
			break;
		}
		if (!obj) {
			return CAST_DATA_ERROR;
		}
		else {
			obj->set_null();
			return SUCCESS;
		}
	}
	else if (obj->get_type() == T_VARCHAR && type == T_DATETIME) {
		obj = DateTime::make_object(obj->to_string());
		if (!obj) {
			return CAST_DATA_ERROR;
		}
		else {
			return SUCCESS;
		}
	}
	else {
		return CAST_DATA_ERROR;
	}
}
