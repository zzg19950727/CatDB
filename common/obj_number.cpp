#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Number::Number(double value)
	:data(std::to_string(value)),
	scale(0)
{
	obj_width = data.size() + sizeof(data.intg) + sizeof(data.frac);
	obj_type = T_NUMBER;
}

Number::Number(const String & str)
	:data(str),
	scale(0)
{
	obj_width = data.size() + sizeof(data.intg) + sizeof(data.frac);
	obj_type = T_NUMBER;
}

Number::Number(const RawData & data)
	:scale(0)
{
	obj_width = data.length;
	obj_type = T_NUMBER;
	const u8* ptr = data.data;
	int intg, frac;
	memcpy(&intg, ptr, sizeof(intg));
	ptr += sizeof(intg);
	memcpy(&frac, ptr, sizeof(frac));
	ptr += sizeof(frac);
	this->data = my_decimal(ptr, intg+frac, frac);
	Log(LOG_TRACE, "Object", "make number object, value %s", this->data.to_string().c_str());
}

Object_s Number::make_object(double value)
{
	return Object_s(new Number(value));
}

Object_s Number::make_object(const String & value)
{
	return Object_s(new Number(value));
}

u32 Number::serialization(u8* & buffer)
{
	RawData* rdata = RawData::make_row_data(buffer);
	rdata->type = obj_type;
	rdata->length = obj_width;
	u8* ptr = rdata->data;
	memcpy(ptr, &data.intg, sizeof(data.intg));
	ptr += sizeof(data.intg);
	memcpy(ptr, &data.frac, sizeof(data.frac));
	ptr += sizeof(data.frac);
	uchar tmp[29];
	data.to_binary(tmp);
	memcpy(ptr, tmp, data.size());
	buffer += rdata->size();
	return SUCCESS;
}

bool Number::is_fixed_length()
{
	return false;
}

bool Number::bool_value()
{
	return (!is_null()) && (data.to_bool());
}

u32 Number::hash()
{
	if (is_null()) {
		return 0;
	}
	else {
		Hash<String> hash;
		return hash(data.to_string(0));
	}
}

double Number::value() const
{
	return data.to_double();
}

String Number::to_string() const
{
	if (is_null())
		return String("NULL");
	String str;
	if (scale < 0) {
		str = data.to_string(1);
		auto pos = str.find('.');
		if (pos != String::npos) {
			for (u32 i = pos; i < str.size(); ++i)
				str[i] = 0;
		}
	}
	else {
		str = data.to_string(scale);
	}
	return str;
}

Object_s Number::copy()
{
	return Number::make_object(data.to_string(0));
}

void Number::set_scale(int value)
{
	scale = value;
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
		my_decimal res;
		my_decimal::my_decimal_add(res, data, rhs->data);
		return Number::make_object(res.to_string(0));
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
		my_decimal res;
		my_decimal::my_decimal_sub(res, data, rhs->data);
		return Number::make_object(res.to_string(0));
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
		my_decimal res;
		my_decimal::my_decimal_mul(res, data, rhs->data);
		return Number::make_object(res.to_string(0));
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
		if (rhs->data.is_zero()) {
			Log(LOG_ERR, "Object", "divisor can not be zero");
			return Error::make_object(DEVISOR_IS_ZERO);
		}
		else {
			my_decimal res;
			my_decimal::my_decimal_div(res, data, rhs->data, data.frac > rhs->data.frac ? data.frac : rhs->data.frac);
			return Number::make_object(res.to_string(0));
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
		return Bool::make_object(data.cmp(rhs->data) == 0);
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
		return Bool::make_object(data.cmp(rhs->data) > 0);
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
		return Bool::make_object(data.cmp(rhs->data) < 0);
	}
}

Object_s Number::op_minus()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else {
		my_decimal res;
		my_decimal::my_decimal_mul(res, data, my_decimal("-1"));
		return Number::make_object(res.to_string(0));
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
		return Bool::make_object( data.cmp(lhs->data) >= 0 && data.cmp(rhs->data) <= 0);
	}
}

void Number::increase()
{
	String one("1");
	my_decimal decimal_one(one);
	my_decimal::my_decimal_add(data, data, decimal_one);
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
		my_decimal::my_decimal_add(data, data, rhs->data);
	}
}