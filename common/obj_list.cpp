#include "object.h"
#include "buffer.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Common;

ObjList::ObjList()
{
	obj_width = 0;
	obj_type = T_LIST;
	Log(LOG_TRACE, "Object", "make list object");
}

Object_s ObjList::make_object()
{
	return Object_s(new ObjList());
}

void ObjList::add_object(const Object_s & obj)
{
	assert(obj);
	list.push_back(obj);
}

u32 ObjList::size() const
{
	return list.size();
}

u32 ObjList::serialization(u8 *& buffer)
{
	return OPERATION_NOT_SUPPORT;
}

bool ObjList::is_fixed_length()
{
	return false;
}

bool ObjList::bool_value()
{
	return !list.empty();
}

u32 ObjList::hash()
{
	if (is_null()) {
		return 0;
	}
	else {
		Hash<u32> hash;
		u32 ret = 0;
		for (u32 i = 0; i < list.size(); ++i) {
			ret += list[i]->hash();
		}
		return hash(ret);
	}
}

String ObjList::to_string() const
{
	String str;
	for (u32 i = 0; i < list.size(); ++i) {
		str += list[i]->to_string() + ",";
	}
	if (!str.empty()) {
		str[str.size() - 1] = 0;
	}
	return str;
}

Object_s ObjList::in(const Object_s & other)
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else if (other->is_null()) {
		if (list.empty()) {
			return Bool::make_object(true);
		}
		else {
			return Bool::make_object(false);
		}
	}
	else {
		bool find = false;
		for (u32 i = 0; i < list.size(); ++i) {
			if (list[i]->operator==(other)->bool_value()) {
				find = true;
				break;
			}
		}
		return Bool::make_object(find);
	}
}

Object_s ObjList::not_in(const Object_s & other)
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else if (other->is_null()) {
		if (list.empty()) {
			return Bool::make_object(true);
		}
		else {
			return Bool::make_object(false);
		}
	}
	else {
		bool find = false;
		for (u32 i = 0; i < list.size(); ++i) {
			if (list[i]->operator==(other)->bool_value()) {
				find = true;
				break;
			}
		}
		return Bool::make_object(!find);
	}
}

Object_s ObjList::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else {
		return Bool::make_object(!list.empty());
	}
}
