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
{
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

String Object::type() const
{
	switch(obj_type & (~T_NULL)) {
		case T_BOOL: return String(VAR_NAME(T_BOOL));
		case T_NUMBER: return String(VAR_NAME(T_NUMBER));
		case T_DATETIME: return String(VAR_NAME(T_DATETIME));
		case T_VARCHAR: return String(VAR_NAME(T_VARCHAR));
		case T_MAX_TYPE: return String(VAR_NAME(T_MAX_TYPE));
		case T_LIST: return String(VAR_NAME(T_LIST));
		case T_QUERY_RESULT: return String(VAR_NAME(T_QUERY_RESULT));
		case T_ERROR_RESULT: return String(VAR_NAME(T_ERROR_RESULT));
		default: return String("unknown");
	}
}

Object_s Object::copy()
{
	return Error::make_object(ERR_EXPR_TYPE);
}

double Object::value() const
{
	return 0;
}

Object_s Object::operator+(const Object_s & other)
{
	LOG_ERR("object do not support add operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator-(const Object_s & other)
{
	LOG_ERR("object do not support sub operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator*(const Object_s & other)
{
	LOG_ERR("object do not support mul operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator/(const Object_s & other)
{
	LOG_ERR("object do not support div operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator==(const Object_s & other)
{
	LOG_ERR("object do not support equal operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator>(const Object_s & other)
{
	LOG_ERR("object do not support greater operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::operator<(const Object_s & other)
{
	LOG_ERR("object do not support less operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::between(const Object_s & left, const Object_s & right)
{
	LOG_ERR("object do not support between operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::in(const Object_s & other)
{
	LOG_ERR("object do not support in operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::not_in(const Object_s & other)
{
	LOG_ERR("object do not support not in operation", K(type()));
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
	LOG_ERR("object do not support exists operation", K(type()));
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

Object_s Object::op_minus()
{
	LOG_ERR("object do not support minus operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Object::like(const Object_s & other)
{
	LOG_ERR("object do not support like operation", K(type()));
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

void Object::increase()
{
	LOG_ERR("object do not support increase operation", K(type()));
}

void Object::accumulate(const Object_s& other)
{
	LOG_ERR("object do not support accumulate operation", K(type()));
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
		return SUCCESS;
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
