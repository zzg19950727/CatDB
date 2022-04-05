#include "obj_datetime.h"
#include "obj_number.h"
#include "error.h"
#include "log.h"
#include "time.h"
using namespace CatDB::Common;

long long DateTime::get_seconds(const char* str, u32 size)
{
	tm tm_;
	int year=1970, month=1, day=1, hour=0, minute=0, second=0;
	int type = 0;	//0 none, 1 time, 2 date, 3 datetime
	for (int i = 0; i < size; ++i) {
		if (str[i] == ':') {
			if (type == 0) {
				type = 1;
			} else if (type == 2) {
				type = 3;
			}
		} else if (str[i] == '-' || str[i] == '/') {
			if (type == 0) {
				type = 2;
			} else if (type == 1) {
				type = 3;
			}
		} else if (str[i] == ' ') {

		} else if (str[i] < '0' || str[i] > '9') {
			return -1;
		}
	}
	if(1 == type)
		sscanf(str, "%d:%d:%d", &hour, &minute, &second);
	else if(2 == type)
		sscanf(str, "%d-%d-%d", &year, &month, &day);
	else if (3 == type)
		sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	else
		return -1;

	if (month > 12 || day > 31 || hour > 24 || minute > 60 || second > 60)
		return -1;

	tm_.tm_year = year;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t t_ = mktime(&tm_);
	return (long long)t_;
}

DateTime::DateTime(const my_decimal& data)
	:data(data),
	type(DATETIME)
{

}

DateTime::DateTime(long long seconds)
	:data(seconds),
	type(DATETIME)
{
}

DateTime::DateTime(const u8 *data, u32 size, const DataType& type)
	:data(data, size, type.precision, type.scale),
	type(DATETIME)
{
	this->type = type.time_type;
}

DateTime::DateTime(const char *data, u32 size, const DataType& type)
	:type(DATETIME)
{
	long long seconds = get_seconds(data, size);
	this->data = my_decimal(seconds);
	this->type = type.time_type;
}

DateTime_s DateTime::make_object(const String & str)
{
	return DateTime_s(new DateTime(get_seconds(str.c_str(), str.size())));
}

u32 DateTime::serialization(u8* & buffer) const
{
	u32 ret = data.to_binary(buffer);
	buffer += width();
	return ret;
}

u32 DateTime::hash() const
{
	if (is_null()) {
		return 0;
	} else {
		Hash<String> hash;
		return hash(data.to_string());
	}
}

String DateTime::to_string() const
{
	String ret = "NULL";
	if (!is_null()) {
		char tmp[255];
		time_t value = (time_t)data.to_longlong();
		tm* time = localtime(&value);
		if (time != NULL) {
			switch (type) {
				case TIME:
					sprintf(tmp, "%02d:%02d:%02d",
						time->tm_hour,
						time->tm_min,
						time->tm_sec
					);
					break;
				case DATE:
					sprintf(tmp, "%04d-%02d-%02d",
						time->tm_year,
						time->tm_mon + 1,
						time->tm_mday
					);
					break;
				case DATETIME:
					sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d",
						time->tm_year,
						time->tm_mon + 1,
						time->tm_mday,
						time->tm_hour,
						time->tm_min,
						time->tm_sec
					);
					break;
				case TIMESTAMP:
					sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d",
						time->tm_year,
						time->tm_mon + 1,
						time->tm_mday,
						time->tm_hour,
						time->tm_min,
						time->tm_sec
					);
					break;
			}
			ret = String(tmp);
		}
	}
	return ret;
}

OBJ_TYPE DateTime::get_type()const
{
	return T_DATETIME;
}

u32 DateTime::width()const
{
	return data.size();
}

u32 DateTime::compare(const Object_s& rhs, int& res) const
{
	u32 ret = SUCCESS;
	if (is_null() || rhs->is_null()) {
		res = -2;
		return ret;
	}
	DateTime_s r = rhs;
	res = data.cmp(r->data);
	return ret;
}

u32 DateTime::cast_to(const DataType& type, Object_s &res)
{
	u32 ret = SUCCESS;
	if (!type.is_datetime()) {
		ret = INVALID_CAST;
	} else {
		res = Object_s(new DateTime(data));
	}
	return ret;
}

bool DateTime::is_valid_datetime(const String& str)
{
	return get_seconds(str.c_str(), str.size()) >= 0;
}

u32 DateTime::current_datetime(DateTime_s &cur)
{
	time_t t = Clock::to_time_t(Clock::now());
	cur = Object_s(new DateTime(t));
	return SUCCESS;
}

u32 DateTime::format_datetime(const DateTime_s &src_time, const String &format, String &res)
{
	u32 ret = SUCCESS;
	char tmp[255] = {0};
	bool next = false;
	if (src_time->is_null()) {
		res = "NULL";
		return ret;
	}
	time_t value = (time_t)src_time->data.to_longlong();
	tm* time = localtime(&value);
	if (time == NULL)
	{
		return ERR_UNEXPECTED;
	}
	//format yyyy
	if("" == format || format.find('y') != String::npos) {
		sprintf(tmp + strlen(tmp), "%04d",
			time->tm_year
		);
		next = true;
	} else {
		next = false;
	}
	//format mm
	if("" == format || format.find('m') != String::npos) {
		if (next) {
			sprintf(tmp + strlen(tmp), "-");
		}
		sprintf(tmp + strlen(tmp), "%02d",
			time->tm_mon + 1
		);
		next = true;
	} else {
		next = false;
	}
	//format dd
	if("" == format || format.find('d') != String::npos) {
		if (next) {
			sprintf(tmp + strlen(tmp), "-");
		}
		sprintf(tmp + strlen(tmp), "%02d",
			time->tm_mday
		);
		next = true;
	} else {
		next = false;
	}
	//format hh
	if("" == format || format.find('h') != String::npos) {
		if (next) {
			sprintf(tmp + strlen(tmp), " ");
		}
		sprintf(tmp + strlen(tmp), "%02d",
			time->tm_hour
		);
		next = true;
	} else {
		next = false;
	}
	//format MM
	if("" == format || format.find('M') != String::npos) {
		if (next) {
			sprintf(tmp + strlen(tmp), ":");
		}
		sprintf(tmp + strlen(tmp), "%02d",
			time->tm_min
		);
		next = true;
	} else {
		next = false;
	}
	//format ss
	if("" == format || format.find('s') != String::npos) {
		if (next) {
			sprintf(tmp + strlen(tmp), ":");
		}
		sprintf(tmp + strlen(tmp), "%02d",
			time->tm_sec
		);
		next = true;
	} else {
		next = false;
	}
	res = String(tmp);
	return ret;
}

u32 DateTime::add_seconds(const DateTime_s &time, const Number_s &seconds, DateTime_s &res)
{
	u32 ret = SUCCESS;
	if (time->is_null() || seconds->is_null()) {
		res = DateTime_s(new DateTime(0));
		res->set_null();
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_add(dst, time->data, seconds->data);
		MY_ASSERT(0 == r);
		res = DateTime_s(new DateTime(dst));
	}
	return ret;
}

u32 DateTime::sub_seconds(const DateTime_s &time, const Number_s &seconds, DateTime_s &res)
{
	u32 ret = SUCCESS;
	if (time->is_null() || seconds->is_null()) {
		res = DateTime_s(new DateTime(0));
		res->set_null();
	} else {
		my_decimal dst;
		int r = my_decimal::my_decimal_sub(dst, time->data, seconds->data);
		MY_ASSERT(0 == r);
		res = DateTime_s(new DateTime(dst));
	}
	return ret;
}

u32 DateTime::make_second_from_day(u32 day, Number_s& seconds)
{
	u32 ret = SUCCESS;
	seconds = Number::make_object((longlong)(day * 24 * 60 * 60));
	return ret;
}

u32 DateTime::make_second_from_month(u32 month, Number_s& seconds)
{
	u32 ret = SUCCESS;
	seconds = Number::make_object((longlong)(month * 31 * 24 * 60 * 60));
	return ret;
}

u32 DateTime::make_second_from_year(u32 year, Number_s& seconds)
{
	u32 ret = SUCCESS;
	seconds = Number::make_object((longlong)(year * 365 * 24 * 60 * 60));
	return ret;
}