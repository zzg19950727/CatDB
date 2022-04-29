#include "obj_varchar.h"
#include "obj_number.h"
#include "obj_datetime.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

Varchar::Varchar(const String & str)
{
	data = new char[str.size() + 1];
	memcpy(data, str.c_str(), str.size());
	length = str.size();
	data[length] = 0;
}

Varchar::Varchar(const char* str, u32 len)
{
	data = new char[len + 1];
	memcpy(data, str, len);
	length = len;
	data[length] = 0;
}

Varchar::Varchar(const char* buf, u32 size, const DataType& type)
{
	size = size > type.length ? type.length : size;
	data = new char[size + 1];
	memcpy(data, buf, size);
	length = size;
	data[length] = 0;
}

Varchar::~Varchar()
{
	if (data) {
		delete[] data;
		data = NULL;
	}
}

Varchar_s Varchar::make_object(const char* str, u32 len)
{
	return Varchar_s(new Varchar(str, len));
}

Varchar_s Varchar::make_object(const String & buf)
{
	return Varchar_s(new Varchar(buf));
}

u32 Varchar::serialization(u8* & buffer) const
{
	memcpy(buffer, data, length);
	buffer += length;
	return SUCCESS;
}

u32 Varchar::hash()const
{
	if (is_null()) {
		return 0;
	} else {
		Hash<String> hash;
		return hash(String(data));
	}
}

String Varchar::to_string() const
{
	if (is_null())
		return String("NULL");
	else
		return String(data);
}

OBJ_TYPE Varchar::get_type()const
{
	return T_VARCHAR;
}

u32 Varchar::width()const
{
	return length;
}

u32 Varchar::compare(const Object_s & rhs, int &res) const
{
	u32 ret = SUCCESS;
	if (is_null() || rhs->is_null()) {
		res = -2;
		return ret;
	} else {
		Varchar_s r = rhs;
		u64 len = length > r->length ? r->length : length;
		res = memcmp(data, r->data, len);
		if (res != 0) {

		} else if (length > r->length) {
			res = 1;
		} else if (length < r->length) {
			res = -1;
		}
	}
	return ret;
}

u32 Varchar::cast_to(const DataType& type, Object_s &res)
{
	u32 ret = SUCCESS;
	if (type.is_varchar()) {
		res = Object_s(new Varchar(data, length, type));
	} else if (type.is_number()) {
		if (!Number::is_valid_number(String(data, length))) {
			ret = INVALID_NUMBER;
		} else {
			res = Object_s(new Number(data, length, type));
		}
	} else if (type.is_datetime()) {
		if (!DateTime::is_valid_datetime(String(data, length))) {
			ret = INVALID_DATETIME;
		} else {
			res = Object_s(new DateTime(data, length, type));
		}
	} else {
		ret = INVALID_CAST;
	}
	return ret;
}

bool begin_with(const char* str1, const char* str2, int& beg1, int& beg2, int end1, int end2)
{
	while (beg1 <= end1 && beg2 <= end2) {
		if (str2[beg2] == '%') {
			return true;
		}
		else if (str1[beg1] != str2[beg2]) {
			return false;
		}
		++beg1; ++beg2;
	}
	return true;
}

bool end_width(const char* str1, const char* str2, int beg1, int beg2, int& end1, int& end2)
{
	while (beg1 <= end1 && beg2 <= end2) {
		if (str2[end2] == '%') {
			return true;
		}
		else if (str1[end1] != str2[end2]) {
			return false;
		}
		--end1; --end2;
	}
	return true;
}

void kmp_next(const char *s, int beg, int end, int* next)
{
	next[0] = -1;
	int k = -1;
	int i = beg + 1;
	for ( ; i <= end; ++i) {
		while (k > -1 && s[k + 1 + beg] != s[i]) {
			k = next[k];
		}
		if (s[k + 1 + beg] == s[i]) {
			k++;
		}
		next[i] = k;
	}
}

bool kmp(const char *s1, const char *s2, int& beg1, int& beg2, int end1, int end2)
{
	int next[1024] = {-1};
	kmp_next(s2, beg2, end2, next);
	int k = -1;
	int i = beg1;
	for ( ;i <= end1 ; ++i) {
		while (k > -1 && s2[k + 1 + beg2] != s1[i]) {
			k = next[k];
		}
		if (s2[k + 1 + beg2] == s1[i]) {
			k++;
		}
		if (k + beg2 == end2) {
			beg1 = i + 1;
			beg2 = end2;
			return true;
		}
	}
	return false;
}

bool contain(const char* str1, const char* str2, int& beg1, int& beg2, int end1, int end2)
{
	
	int pos1 = beg2;
	while (pos1 <= end2 && str2[pos1] == '%') {
		++pos1;
	}
	
	int pos2 = pos1;
	while (pos2 <= end2 && str2[pos2] != '%') {
		++pos2;
	}
	
	if (pos1 == pos2) {
		return true;
	}
	else if (beg1 > end1) {
		return false;
	}
	if (kmp(str1, str2, beg1, pos1, end1, pos2-1)) {
		beg2 = pos2;
		return contain(str1, str2, beg1, beg2, end1, end2);
	}
	else {
		return false;
	}
}

bool string_like(const char* str1, const char* str2)
{
	int beg1 = 0;
	int beg2 = 0;
	int end1 = strlen(str1);
	int end2 = strlen(str2);
	--end1;
	--end2;
	if (!begin_with(str1, str2, beg1, beg2, end1, end2)) {
		return false;
	} else if (!end_width(str1, str2, beg1, beg2, end1, end2)) {
		return false;
	} else if (!contain(str1, str2, beg1, beg2, end1, end2)) {
		return false;
	} else {
		return true;
	}
}

u32 Varchar::like(const Varchar_s& str, const String& pattern, const String& escape, bool &res)
{
	u32 ret = SUCCESS;
	if (str->is_null()) {
		res = false;
		return ret;
	} else {
		res = string_like(str->data, pattern.c_str());
	}
	return ret;
}

u32 Varchar::substr(const Varchar_s& str, u32 pos, u32 size, Varchar_s &res)
{
	u32 ret = SUCCESS;
	if (0 == pos) {
		++pos;
	}
	if (pos > str->length) {
		res = Varchar_s(new Varchar(""));
	} else {
		--pos;
		size = str->length - pos > size ? size : str->length - pos;
		res = Varchar_s(new Varchar(str->data+pos, size));
	}
	return ret;
}

u32 Varchar::str_length(const Varchar_s& str, Number_s &len)
{
	u32 ret = SUCCESS;
	if (str->is_null()) {
		len = Number::make_int_object(0);
		len->set_null();
	} else {
		len = Number::make_int_object(str->length);
	}
	return ret;
}