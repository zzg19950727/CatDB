#include "query_result.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;

QueryResult::QueryResult()
	:err_code(SUCCESS)
{
}

Object_s QueryResult::make_query_result()
{
	return Object_s(new QueryResult());
}

u32 QueryResult::size() const
{
	return list.size();
}

void QueryResult::add_row(const Row_s & row)
{
	list.push_back(row);
}

u32 QueryResult::get_row(u32 idx, Row_s & row)
{
	if (idx >= list.size()) {
		return ERROR_INDEX;
	}
	else {
		row = list[idx];
		return SUCCESS;
	}
}

u32 QueryResult::set_error_code(u32 code)
{
	err_code = code;
	return SUCCESS;
}

u32 QueryResult::get_error_code() const
{
	return err_code;
}

u32 QueryResult::serialization(u8 *& buffer)
{
	return OPERATION_NOT_SUPPORT;
}

bool QueryResult::is_fixed_length()
{
	return false;
}

bool QueryResult::bool_value()
{
	return err_code == SUCCESS;
}

u32 QueryResult::hash()
{
	return OPERATION_NOT_SUPPORT;
}

String QueryResult::to_string() const
{
	return String(err_string(err_code));
}
