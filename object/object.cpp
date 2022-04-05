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
	}
	if (ret) {
		ret->set_null();
	}
	return ret;
}

bool Object::is_null() const
{
	return is_null_obj;
}

void Object::set_null(const bool value)
{
	is_null_obj = value;
}