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
}

Varchar::Varchar(const String & str)
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

Object_s Varchar::make_object(const String & buf)
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
	if (is_null()) {
		return 0;
	}
	else {
		Hash<String> hash;
		return hash(String((char*)data->buf));
	}
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
		LOG_ERR("varchar type can not compare", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar_s rhs = other;
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
		LOG_ERR("varchar type can not compare", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar_s rhs = other;
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
		LOG_ERR("varchar type can not compare", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar_s rhs = other;
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

Object_s Varchar::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else {
		return Bool::make_object(true);
	}
}

Object_s Varchar::between(const Object_s & left, const Object_s & right)
{
	if (is_null() || left->is_null() || right->is_null()) {
		return Object::make_null_object();
	}
	else if (left->get_type() != T_DATETIME || right->get_type() != T_DATETIME) {
		LOG_ERR("datetime type can not comapre", K(left), K(right));
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
	int next[1024];
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
	}
	else if (!end_width(str1, str2, beg1, beg2, end1, end2)) {
		return false;
	}
	else if (!contain(str1, str2, beg1, beg2, end1, end2)) {
		return false;
	}
	else {
		return true;
	}
}

Object_s Varchar::like(const Object_s & other)
{
	if (is_null() || other->is_null()) {
		return Object::make_null_object();
	}
	else if (other->get_type() != T_VARCHAR) {
		LOG_ERR("varchar type can not compare", K(other));
		return Error::make_object(OPERATION_NOT_SUPPORT);
	}
	else {
		Varchar_s rhs = other;
		return Bool::make_object(string_like((char*)data->buf, (char*)rhs->data->buf));
	}
}
