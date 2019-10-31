#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
#include "time.h"
using namespace CatDB::Common;

DateTime::DateTime(double value)
	:data(value)
{
	obj_width = sizeof(value);
	obj_type = T_DATETIME;
}

DateTime::DateTime(const RawData & data)
{
	obj_width = data.length;
	obj_type = data.type;
	memcpy(&this->data, data.data, obj_width);
	Log(LOG_TRACE, "Object", "make datetime object, value %f", this->data);
}

Object_s DateTime::make_object(double value)
{
	return Object_s(new DateTime(value));
}

time_t StringToDatetime(const String& str)
{
	const char *cha = str.c_str();
	tm tm_;
	int year=1970, month=1, day=1, hour=0, minute=0, second=0;
	if(str.find('-') == String::npos)
		sscanf(cha, "%d:%d:%d", &hour, &minute, &second);
	else if(str.find(':') == String::npos)
		sscanf(cha, "%d-%d-%d", &year, &month, &day);
	else
		sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

	if (month > 12 || day > 31 || hour > 24 || minute > 60 || second > 60)
		return -1;
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day - 1;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t t_ = mktime(&tm_);
	return t_;
}

String DatetimeToString(time_t t)
{
	char tmp[255];
	tm* time = localtime(&t);
	if (time == NULL)
	{
		return "";
	}
	sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d",
		time->tm_year + 1900,
		time->tm_mon + 1,
		time->tm_mday + 1,
		time->tm_hour,
		time->tm_min,
		time->tm_sec
	);
	return String(tmp);
}

Object_s CatDB::Common::DateTime::make_object(const String & str)
{
	double value = static_cast<double>( StringToDatetime(str) );
	if(value < 0)
		return Object_s();
	else
		return Object_s(new DateTime(value));
}

u32 DateTime::serialization(u8* & buffer)
{
	RawData* rdata = RawData::make_row_data(buffer);
	rdata->type = obj_type;
	rdata->length = obj_width;
	memcpy(rdata->data, &data, obj_width);
	buffer += rdata->size();
	return SUCCESS;
}

bool DateTime::is_fixed_length()
{
	return true;
}

bool DateTime::bool_value()
{
	return (!is_null());
}

u32 DateTime::hash()
{
	Hash<double> hash;
	return hash(data);
}

String DateTime::to_string() const
{
	if (is_null())
		return String("NULL");
	else
		return DatetimeToString(data);
}

Object_s DateTime::operator+(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not add %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		DateTime* rhs = dynamic_cast<DateTime*>((other.get()));
		return DateTime::make_object(data + rhs->data);
	}
}

Object_s DateTime::operator-(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not sub %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		DateTime* rhs = dynamic_cast<DateTime*>((other.get()));
		if (data - rhs->data) {
			Log(LOG_WARN, "Object", "uncorrect datetimer after sub");
			return DateTime::make_object(0);
		}
		else {
			return DateTime::make_object(data - rhs->data);
		}
	}
}

Object_s DateTime::operator==(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		DateTime* rhs = dynamic_cast<DateTime*>((other.get()));
		return Bool::make_object(data == rhs->data);
	}
}

Object_s DateTime::operator>(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		DateTime* rhs = dynamic_cast<DateTime*>((other.get()));
		return Bool::make_object(data > rhs->data);
	}
}

Object_s DateTime::operator<(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not compare %u", other->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		DateTime* rhs = dynamic_cast<DateTime*>((other.get()));
		return Bool::make_object(data < rhs->data);
	}
}

Object_s DateTime::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else {
		return Bool::make_object(true);
	}
}

Object_s DateTime::between(const Object_s & left, const Object_s & right)
{
	if (is_null() || left->is_null() || right->is_null()) {
		return Object::make_null_object();
	}
	else if (left->get_type() != T_DATETIME || right->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not comapre %u", left->get_type());
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		DateTime* lhs = dynamic_cast<DateTime*>((left.get()));
		DateTime* rhs = dynamic_cast<DateTime*>((right.get()));
		return Bool::make_object(data <= rhs->data && data >= lhs->data);
	}
}

void DateTime::accumulate(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		set_null();
	}
	else if (other->get_type() != T_DATETIME) {
		Log(LOG_ERR, "Object", "datetime type can not accumulate with %u", other->get_type());
	}
	else {
		DateTime* rhs = dynamic_cast<DateTime*>((other.get()));
		data += rhs->data;
	}
}