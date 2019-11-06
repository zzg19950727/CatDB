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
	if (is_null()) {
		return 0;
	}
	else {
		Hash<double> hash;
		return hash(data);
	}
}

double Number::value() const
{
	return data;
}

String Number::to_string() const
{
	if (is_null())
		return String("NULL");
	String str = std::to_string(data);
	u32 pos = str.size();
	for (u32 i = 0; i < str.size(); ++i) {
		if (str[i] == '.') {
			pos = i;
			break;
		}
	}
	if (pos == str.size()) {
		return str;
	}
	for (u32 i = str.size() - 1; i >= pos; --i) {
		if (str[i] == '0' || str[i] == '.') {
			str[i] = 0;
		}
		else {
			break;
		}
	}
	while (*str.rbegin() == 0) {
		str.erase(str.end()-1);
	}
	return str;
}

Object_s Number::copy()
{
	return Number::make_object(data);
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

Object_s Number::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else {
		return Bool::make_object(true);
	}
}

Object_s Number::between(const Object_s & left, const Object_s & right)
{
	if (is_null() || left->is_null() || right->is_null()) {
		return Object::make_null_object();
	}
	else if (left->get_type() != T_NUMBER || right->get_type() != T_NUMBER) {
		Log(LOG_ERR, "Object", "number type can not comapre %u", left->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Number* lhs = dynamic_cast<Number*>((left.get()));
		Number* rhs = dynamic_cast<Number*>((right.get()));
		return Bool::make_object((data <= rhs->data) && (data >= lhs->data));
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