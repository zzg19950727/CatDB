#include "obj_number.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Number::Number(const my_decimal& data)
	:data(data)
{
}

Number::Number(f64 value)
	:data(value)
{
}

Number::Number(longlong value)
	:data(value)
{
}

Number::Number(const String & str)
	:data(str)
{
}

Number::Number(const u8* data, u32 size, const DataType& type)
	:data(data, size, type.precision, type.scale)
{
	int prec, scale;
	memcpy(&prec, data, sizeof(prec));
	memcpy(&scale, data + sizeof(prec), sizeof(scale));
}

Number::Number(const char* data, u32 size, const DataType& type)
	:data(data, size)
{
	this->data.cast_to_decimal(type.precision, type.scale, this->data);
}

Number_s Number::make_object(const my_decimal& data)
{
	Number *num = new Number(data);
	return Number_s(num);
}

Number_s Number::make_float_object(f64 value)
{
	Number* num = new Number(value);
	return Number_s(num);
}

Number_s Number::make_int_object(longlong value)
{
	Number* num = new Number(value);
	return Number_s(num);
}

Number_s Number::make_object(const String & value)
{
	return Number_s(new Number(value));
}

Number_s Number::make_object(Number_s value)
{
	return Number_s(new Number(value->data));
}

u32 Number::init(const u8* buf, u32 size, const DataType& type)
{
	if (NULL != buf) {
		data.init(buf, size, type.precision, type.scale);
	}
	return SUCCESS;
}

u32 Number::init(const char* buf, u32 size, const DataType& type)
{
	if (NULL != buf) {
		data.init(buf, size);
		data.cast_to_decimal(type.precision, type.scale, this->data);
	}
	return SUCCESS;
}

u32 Number::init(longlong value)
{
	data.init(value);
	return SUCCESS;
}

u32 Number::serialization(u8* & buffer) const
{
	u32 ret = data.to_binary(buffer);
	buffer += width();
	return ret;
}

u32 Number::hash() const
{
	if (is_null()) {
		return 0;
	} else {
		Hash<String> hash;
		return hash(data.to_string());
	}
}

String Number::to_string() const
{
	if (is_null()) {
		return String("NULL");
	} else {
		return data.to_string();
	}
}

double Number::value() const
{
	return data.to_double();
}

OBJ_TYPE Number::get_type()const
{
	return T_NUMBER;
}

u32 Number::width()const
{
	return data.size();
}

u32 Number::accumulate(Number_s rhs)
{
	u32 ret = SUCCESS;
	if (is_null() || rhs->is_null()) {
		set_null();
	} else {
		my_decimal res;
		int r = my_decimal::my_decimal_add(res, data, rhs->data);
		MY_ASSERT(0 == r);
		std::swap(res, data);
	}
	return ret;
}

u32 Number::compare(const Object_s& rhs, int& res) const
{
	u32 ret = SUCCESS;
	if (is_null() || rhs->is_null()) {
		res = CMP_RES_NULL;
		return ret;
	}
	Number_s r = rhs;
	res = data.cmp(r->data);
	return ret;
}

u32 Number::cast_to(const DataType& type, Object_s &res)
{
	u32 ret = SUCCESS;
	if (!type.is_number()) {
		ret = INVALID_CAST;
	} else {
		res = Number::make_int_object((longlong)0);
		Number_s num = res;
		data.cast_to_decimal(type.precision, type.scale, num->data);
	}
	return ret;
}

u32 Number::num_add(const Number_s& lhs, const Number_s& rhs, Number_s& res)
{
	u32 ret = SUCCESS;
	if (lhs->is_null() || rhs->is_null()) {
		res = Number::make_int_object(0);
		res->set_null();
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_add(dst, lhs->data, rhs->data);
		MY_ASSERT(0 == r);
		res = Number::make_object(dst);
	}
	return ret;
}

u32 Number::num_sub(const Number_s& lhs, const Number_s& rhs, Number_s& res)
{
	u32 ret = SUCCESS;
	if (lhs->is_null() || rhs->is_null()) {
		res = Number::make_int_object(0);
		res->set_null();
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_sub(dst, lhs->data, rhs->data);
		MY_ASSERT(0 == r);
		res = Number::make_object(dst);
	}
	return ret;
}

u32 Number::num_mul(const Number_s& lhs, const Number_s& rhs, Number_s& res)
{
	u32 ret = SUCCESS;
	if (lhs->is_null() || rhs->is_null()) {
		res = Number::make_int_object(0);
		res->set_null();
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_mul(dst, lhs->data, rhs->data);
		MY_ASSERT(0 == r);
		res = Number::make_object(dst);
	}
	return ret;
}

u32 Number::num_div(const Number_s& lhs, const Number_s& rhs, Number_s& res)
{
	u32 ret = SUCCESS;
	if (lhs->is_null() || rhs->is_null()) {
		res = Number::make_int_object(0);
		res->set_null();
	} else if (rhs->data.is_zero()) {
		ret = DEVISOR_IS_ZERO;
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_div(dst, lhs->data, rhs->data, 0);
		MY_ASSERT(0 == r);
		res = Number::make_object(dst);
	}
	return ret;
}

u32 Number::num_mod(const Number_s& lhs, const Number_s& rhs, Number_s& res)
{
	u32 ret = SUCCESS;
	if (lhs->is_null() || rhs->is_null()) {
		res = Number::make_int_object(0);
		res->set_null();
	} else if (rhs->data.is_zero()) {
		ret = DEVISOR_IS_ZERO;
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_mod(dst, lhs->data, rhs->data);
		MY_ASSERT(0 == r);
		res = Number::make_object(dst);
	}
	return ret;
}

u32 Number::num_minus(const Number_s& lhs, Number_s& res)
{
	u32 ret = SUCCESS;
	res = Number::make_object(lhs->data);
	if (lhs->is_null()) {
		res->set_null();
	} else {
		res->data.minus();
	}
	return ret;
}

bool Number::is_valid_number(const String& value)
{
	bool is_valid = true;
	bool has_dot = false;
	for (u32 i = 0; is_valid && i < value.size(); ++i) {
		if (value[i] == '.') {
			if (has_dot) {
				is_valid = false;
			} else {
				has_dot = true;
			}
		} else if (value[i] < '0' || value[i] > '9') {
			is_valid = false;
		}
	}
	return is_valid;
}