#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Number::Number(double value)
	:data(value)
{
	obj_width = sizeof(value);
	obj_type = T_NUMBER;
}

Number::Number(const RawData & data)
{
	obj_width = data.length;
	obj_type = T_NUMBER;
	memcpy(&this->data, data.data, obj_width);
	Log(LOG_TRACE, "Object", "make number object, value %f", this->data);
}

Object_s Number::make_object(double value)
{
	return Object_s(new Number(value));
}

u32 Number::serialization(u8* & buffer)
{
	RawData* rdata = RawData::make_row_data(buffer);
	rdata->type = obj_type;
	rdata->length = obj_width;
	memcpy(rdata->data, &data, obj_width);
	buffer += rdata->size();
	return SUCCESS;
}

bool Number::is_fixed_length()
{
	return true;
}

bool Number::bool_value()
{
	return (!is_null()) && (data != 0);
}

u32 Number::hash()
{
	Hash<double> hash;
	return hash(data);
}

String Number::to_string() const
{
	if (is_null())
		return String("NULL");
	else
		return std::to_string(static_cast<u32>(data));
}

Object_s Number::operator+(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not add %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		return Number::make_object(data + rhs->data);
	}
}

Object_s Number::operator-(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not sub %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		return Number::make_object(data - rhs->data);
	}
}

Object_s Number::operator*(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not mul %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		return Number::make_object(data * rhs->data);
	}
}

Object_s Number::operator/(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		if (rhs->data == 0) {
			Log(LOG_ERR, "Object", "divisor can not be zero");
			return Error::make_object(DEVISOR_IS_ZERO);
		}
		else {
			return Number::make_object(data / rhs->data);
		}
	}
}

Object_s Number::operator==(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not comapre %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		return Bool::make_object(data == rhs->data);
	}
}

Object_s Number::operator>(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not comapre %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		return Bool::make_object(data > rhs->data);
	}
}

Object_s Number::operator<(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not comapre %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		return Bool::make_object(data < rhs->data);
	}
}

void Number::increase()
{
	++data;
}

void Number::accumulate(const Object_s& other)
{
	if (is_null() || other->is_null()) {
		set_null();
	}
	else if (other->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not accumulate with %u", other->get_type());
	}
	else {
		Number* rhs = dynamic_cast<Number*>((other.get()));
		data += rhs->data;
	}
}