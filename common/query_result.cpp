#include "query_result.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;

QueryResult::QueryResult()
{
	obj_width = 0;
	obj_type = T_QUERY_RESULT;
}

QueryResult_s QueryResult::make_query_result()
{
	return QueryResult_s(new QueryResult());
}

u32 QueryResult::size() const
{
	return list.size();
}

u32 QueryResult::add_row(const Row_s & row)
{
	list.push_back(Row::deep_copy(row));
	return SUCCESS;
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
	return !list.empty();
}

u32 QueryResult::hash()
{
	return OPERATION_NOT_SUPPORT;
}

String QueryResult::to_string() const
{
	return String("");
}

Object_s QueryResult::in(const Object_s & other)
{
	if (is_null()) {
		return Object::make_null_object();
	}
	else if (other->is_null()) {
		return Object::make_null_object();
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
			return Object::make_null_object();
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
	} else {
		return Bool::make_object(!list.empty());
	}
}

u32  QueryResult::init_title_for_explain(Row_s & result_title)
{
	RowDesc row_desc(5);
	result_title = Row::make_row(row_desc);
	Object_s label = Varchar::make_object("ID");
	result_title->set_cell(0, label);
	label = Varchar::make_object("OPERATOR");
	result_title->set_cell(1, label);
	label = Varchar::make_object("NAME");
	result_title->set_cell(2, label);
	label = Varchar::make_object("ROW COUNT");
	result_title->set_cell(3, label);
	label = Varchar::make_object("COST");
	result_title->set_cell(4, label);
	return SUCCESS;
}

u32  QueryResult::add_operation_info(u32 depth, const String& operator_name,
	const String& table_name, double rows, u32 times)
{
	Row_s row;
	RowDesc row_desc(5);
	row = Row::make_row(row_desc);
	Object_s id = Number::make_object(list.size(), -1);
	row->set_cell(0, id);

	Object_s name = Varchar::make_object(String(depth, ' ')+operator_name);
	row->set_cell(1, name);

	Object_s table = Varchar::make_object(table_name);
	row->set_cell(2, table);

	Object_s ret_rows = Number::make_object(rows, -1);
	row->set_cell(3, ret_rows);

	Object_s time_point = Number::make_object(times, -1);
	row->set_cell(4, time_point);

	list.push_back(row);
	return SUCCESS;
}
