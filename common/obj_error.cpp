#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Error::Error(u32 err_code)
	:err_code(err_code)
{
	obj_width = sizeof(err_code);
	obj_type = T_ERROR_RESULT;
}

Object_s Error::make_object(u32 err_code)
{
	Error* object = new Error(err_code);
	return Object_s(object);
}

u32 Error::serialization(u8 *& buffer)
{
	RawData* rdata = RawData::make_row_data(buffer);
	rdata->type = T_ERROR_RESULT;
	rdata->length = obj_width;
	memcpy(rdata->data, &err_code, obj_width);
	buffer += rdata->size();
	return SUCCESS;
}

bool Error::is_fixed_length()
{
	return true;
}

bool Error::bool_value()
{
	return false;
}

u32 Error::set_error_code(u32 code)
{
	err_code = code;
	return SUCCESS;
}

u32 Error::get_error_code(u32 & code)
{
	code = err_code;
	return SUCCESS;
}

String Error::to_string() const
{
	return String(err_string(err_code));
}

Object_s Error:: op_and (const Object_s & other)
{
	Log(LOG_ERR, "Object", "error object do not support and operation");
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Error:: op_or (const Object_s & other)
{
	Log(LOG_ERR, "Object", "error object do not support or operation");
	return Error::make_object(OPERATION_NOT_SUPPORT);
}

Object_s Error::op_not()
{
	Log(LOG_ERR, "Object", "error object do not support not operation");
	return Error::make_object(OPERATION_NOT_SUPPORT);
}