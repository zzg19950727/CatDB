#include "query_result.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;

QueryResult::QueryResult()
	:err_code(SUCCESS)
{
	obj_width = 0;
	obj_type = T_QUERY_RESULT;
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

u32 QueryResult::cast_to_simple_object(Object_s & obj)
{
	if (list.size() != 1) {
		return SUBQUERY_RESULT_MUST_BE_ONE_ROW;
	}
	else if(list[0]->get_row_desc().get_column_num() != 1){
		return SUBQUERY_RESULT_MUST_BE_ONE_COLUMN;
	}
	return list[0]->get_cell(0, obj);
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

Object_s QueryResult::in(const Object_s & other)
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
			if (list[i]->get_row_desc().get_column_num() != 1) {
				return Error::make_object(SUBQUERY_RESULT_MUST_BE_ONE_COLUMN);
			}
			Object_s cell;
			list[i]->get_cell(0, cell);
			if (cell->operator==(other)->bool_value()) {
				find = true;
				break;
			}
		}
		return Bool::make_object(find);
	}
}

Object_s QueryResult::not_in(const Object_s & other)
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
			if (list[i]->get_row_desc().get_column_num() != 1) {
				return Error::make_object(SUBQUERY_RESULT_MUST_BE_ONE_COLUMN);
			}
			Object_s cell;
			list[i]->get_cell(0, cell);
			if (cell->operator==(other)->bool_value()) {
				find = true;
				break;
			}
		}
		return Bool::make_object(!find);
	}
}

Object_s QueryResult::exists()
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else if (err_code != SUCCESS) {
		return Error::make_object(err_code);
	}
	else {
		return Bool::make_object(!list.empty());
	}
}
