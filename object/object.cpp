#include "obj_varchar.h"
#include "obj_number.h"
#include "obj_datetime.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;

Object::Object()
	:is_null_obj(false)
{

}

Object::~Object()
{
}

Object_s Object::make_object(const u8* buf, u32 size, const DataType& type)
{
	Object_s ret;
	switch (type.res_type)
	{
	case T_NUMBER:
		ret = Object_s(new Number(buf, size, type));
		break;
	case T_DATETIME:
		ret = Object_s(new DateTime(buf, size, type));
		break;
	case T_VARCHAR:
		ret = Object_s(new Varchar((const char*)buf, size, type));
		break;
	case T_BOOL:
	case T_MAX_TYPE:
	default:
		break;
	}
	return ret;
}

Object_s Object::make_empty_object(const DataType& type)
{
	Object_s ret;
	switch (type.res_type)
	{
	case T_NUMBER:
		ret = Object_s(new Number((longlong)0));
		break;
	case T_DATETIME:
		ret = Object_s(new DateTime(0));
		break;
	case T_VARCHAR:
		ret = Object_s(new Varchar((const char*)NULL, 0, type));
		break;
	case T_BOOL:
	case T_MAX_TYPE:
	default:
		break;
	}
	return ret;
}

Object_s Object::make_object(const char* str, u32 size, const DataType& type)
{
	Object_s ret;
	switch (type.res_type)
	{
	case T_NUMBER:
		ret = Object_s(new Number(str, size, type));
		break;
	case T_DATETIME:
		ret = Object_s(new DateTime(str, size, type));
		break;
	case T_VARCHAR:
		ret = Object_s(new Varchar(str, size, type));
		break;
	case T_BOOL:
	case T_MAX_TYPE:
	default:
		break;
	}
	return ret;
}

Object_s Object::make_null_object()
{
	Object_s ret = Object_s(new Varchar(""));
	ret->set_null();
	return ret;
}

Object_s Object::make_null_object(const DataType& type)
{
	Object_s ret;
	switch (type.res_type)
	{
	case T_NUMBER:
		ret = Object_s(new Number((longlong)0));
		break;
	case T_DATETIME:
		ret = DateTime::make_object("1970-1-1");
		break;
	case T_VARCHAR:
		ret = Object_s(new Varchar(""));
		break;
	case T_BOOL:
	case T_MAX_TYPE:
	default:
		break;
	}
	if (ret) {
		ret->set_null();
	}
	return ret;
}