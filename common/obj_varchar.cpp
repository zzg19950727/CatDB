#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Varchar::Varchar(const Buffer_s & buf)
{
	obj_width = buf->length;
	obj_type = T_VARCHAR;
	this->data = Buffer::make_buffer(obj_width + 1);
	memcpy(this->data->buf, buf->buf, obj_width);
}

Varchar::Varchar(const RawData & data)
{
	obj_width = data.length;
	obj_type = data.type;
	this->data = Buffer::make_buffer(obj_width + 1);
	memcpy(this->data->buf, data.data, obj_width);
	Log(LOG_TRACE, "Object", "make varchar object, value %s", this->data->buf);
}

CatDB::Common::Varchar::Varchar(const String & str)
{
	obj_width = str.size();
	obj_type = T_VARCHAR;
	this->data = Buffer::make_buffer(obj_width + 1);
	memcpy(this->data->buf, str.c_str(), obj_width);
}

Object_s Varchar::make_object(const Buffer_s & buf)
{
	return Object_s(new Varchar(buf));
}

Object_s CatDB::Common::Varchar::make_object(const String & buf)
{
	return Object_s(new Varchar(buf));
}

u32 Varchar::serialization(u8* & buffer)
{
	RawData* rdata = RawData::make_row_data(buffer);
	rdata->type = obj_type;
	rdata->length = obj_width;
	memcpy(rdata->data, data->buf, obj_width);
	buffer += rdata->size();
	return SUCCESS;
}

bool Varchar::is_fixed_length()
{
	return false;
}

bool Varchar::bool_value()
{
	return (!is_null()) && (data->length != 0);
}

u32 Varchar::hash()
{
	Hash<String> hash;
	return hash(String((char*)data->buf));
}

String Varchar::to_string() const
{
	if (is_null())
		return String("NULL");
	else
		return String((char*)data->buf);
}

Object_s Varchar::copy()
{
	return Varchar::make_object(data);
}

Object_s Varchar::operator==(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_VARCHAR) {
		Log(LOG_ERR, "Object", "varchar type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar* rhs = dynamic_cast<Varchar*>((other.get()));
		if (data->length != rhs->data->length) {
			return Bool::make_object(false);
		}
		else {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, data->length) == 0);
		}
	}
}

Object_s Varchar::operator>(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_VARCHAR) {
		Log(LOG_ERR, "Object", "varchar type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar* rhs = dynamic_cast<Varchar*>((other.get()));
		if (data->length > rhs->data->length) {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, rhs->data->length) >= 0);
		}
		else if (data->length < rhs->data->length) {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, data->length) > 0);
		}
		else {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, data->length) > 0);
		}
	}
}

Object_s Varchar::operator<(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_VARCHAR) {
		Log(LOG_ERR, "Object", "varchar type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar* rhs = dynamic_cast<Varchar*>((other.get()));
		if (data->length > rhs->data->length) {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, rhs->data->length) < 0);
		}
		else if (data->length < rhs->data->length) {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, data->length) <= 0);
		}
		else {
			return Bool::make_object(memcmp(data->buf, rhs->data->buf, data->length) < 0);
		}
	}
}

Object_s CatDB::Common::Varchar::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else {
		return Bool::make_object(true);
	}
}

Object_s CatDB::Common::Varchar::between(const Object_s & left, const Object_s & right)
{
	if (is_null() || left->is_null() || right->is_null()) {
		return Object::make_null_object();
	}
	else if (left->get_type() != T_DATETIME || right->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not comapre %u", left->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Object_s r1 = this->operator>(left);
		Object_s r2 = this->operator==(left);
		Object_s r3 = this->operator<(right);
		Object_s r4 = this->operator==(right);
		r1 = r1->op_or(r2);
		r3 = r3->op_or(r4);
		return r1->op_and(r3);
	}
}

Object_s CatDB::Common::Varchar::like(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_VARCHAR) {
		Log(LOG_ERR, "Object", "varchar type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar* rhs = dynamic_cast<Varchar*>((other.get()));
		return Bool::make_object(strstr((char*)data->buf, (char*)rhs->data->buf));
	}
}
