#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Bool::Bool()
	:value(false)
{
	obj_width = sizeof(value);
	obj_type = T_BOOL;
}

Object_s Bool::make_object(bool value)
{
	Bool* object = new Bool();
	object->set_value(value);
	return Object_s(object);
}

u32 Bool::serialization(u8 *& buffer)
{
	RawData* rdata = RawData::make_row_data(buffer);
	rdata->type = obj_type;
	rdata->length = obj_width;
	memcpy(rdata->data, &value, obj_width);
	buffer += rdata->size();
	return SUCCESS;
}

bool Bool::is_fixed_length()
{
	return true;
}

bool Bool::bool_value()
{
	return (!is_null()) && value;
}

u32 Bool::set_value(bool value)
{
	this->value = value;
	obj_type = T_BOOL;
	return SUCCESS;
}

u32 Bool::get_value(bool & value)
{
	value = this->value;
	return SUCCESS;
}

String Bool::to_string() const
{
	if (is_null())
		return String("NULL");
	else if (value)
		return String("true");
	else
		return String("false");
}

Object_s Bool::copy()
{
	return Bool::make_object(value);
}

Object_s Bool::operator+(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not add ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value | rhs->value);
	}
}

Object_s Bool::operator-(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not sub ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value ^ rhs->value);
	}
}

Object_s Bool::operator*(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not mul ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value & rhs->value);
	}
}

Object_s Bool::operator/(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not div ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value & rhs->value);
	}
}

Object_s Bool::operator==(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not compare ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value == rhs->value);
	}
}

Object_s Bool::operator>(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not compare ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value > rhs->value);
	}
}

Object_s Bool::operator<(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_BOOL) {
		LOG_ERR("bool type can not compare ", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Bool* rhs = dynamic_cast<Bool*>((other.get()));
		return Bool::make_object(value < rhs->value);
	}
}

Object_s Bool::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}else {
		return Bool::make_object(true);
	}
}
