#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;

ColumnDesc::ColumnDesc()
	:table_id(0),
	column_id(0)
{
}

void ColumnDesc::set_tid(u32 table_id)
{
	this->table_id = table_id;
}

void ColumnDesc::set_cid(u32 column_id)
{
	this->column_id = column_id;
}

void ColumnDesc::set_tid_cid(u32 table_id, u32 column_id)
{
	this->table_id = table_id;
	this->column_id = column_id;
}

void ColumnDesc::get_tid_cid(u32 & table_id, u32 & column_id) const
{
	table_id = this->table_id;
	column_id = this->column_id;
}

int ColumnDesc::operator==(const ColumnDesc & other) const
{
	return table_id == other.table_id && column_id == other.column_id;
}

u32 ColumnDesc::get_data_type()const
{
	return data_type;
}

void ColumnDesc::set_data_type(u32 type)
{
	data_type = type;
}

RowDesc::RowDesc()
	:column_count(0)
{
}

RowDesc::RowDesc(u32 column_count)
	:column_count(column_count),
	column_desc(column_count)
{
}

u32 RowDesc::get_column_idx(const ColumnDesc & c_desc, u32 & idx) const
{
	bool found = false;
	for (u32 i = 0; i < column_count; ++i){
		if (c_desc == column_desc[i]){
			found = true;
			idx = i;
			break;
		}
	}
	if (!found){
		return ERR_COL_DESC;
	}else{
		return SUCCESS;
	}
}

u32 RowDesc::get_column_desc(u32 idx, ColumnDesc & c_desc) const
{
	if (idx >= column_count){
		return ERR_COL_DESC;
	}else{
		c_desc = column_desc[idx];
		return SUCCESS;
	}
}

u32 RowDesc::set_column_desc(u32 idx, const ColumnDesc & c_desc)
{
	column_desc[idx] = c_desc;
	return SUCCESS;
}

u32 RowDesc::get_column_num() const
{
	return column_count;
}

u32 RowDesc::add_column_desc(const ColumnDesc & col_desc)
{
	column_desc.push_back(col_desc);
	++column_count;
	return SUCCESS;
}

Row::Row(const RowDesc &row_desc)
	:cells(row_desc.get_column_num()),
	row_desc(row_desc),
	row_id(0)
{
}

Row::~Row()
{
	
}

Row_s Row::make_row(const RowDesc &row_desc)
{
	Row* row = new Row(row_desc);
	return Row_s(row);
}

Row_s Row::deep_copy(const Row_s &other)
{
	Row* row = new Row(other->row_desc);
	row->row_id = other->row_id;
	row->cells = other->cells;
	return Row_s(row);
}

void Row::assign(const Row & other)
{

}

void Row::set_row_id(u32 id)
{
	this->row_id = id;
}

u32 Row::get_row_id() const
{
	return row_id;
}

RowDesc & Row::get_row_desc()
{
	return row_desc;
}

void Row::set_row_desc(const RowDesc & row_desc)
{
	this->row_desc = row_desc;
}

u32 Row::get_cell(const ColumnDesc & c_desc, Object_s & cell) const
{
	u32 idx;
	u32 ret = row_desc.get_column_idx(c_desc, idx);
	if (ret != SUCCESS){
		u32 table_id, column_id;
		c_desc.get_tid_cid(table_id, column_id);
		LOG_WARN("column not found in row", K(c_desc));
		return ret;
	}else{
		cell = cells[idx];
		return SUCCESS;
	}
}

u32 Row::get_cell(u32 idx, Object_s & cell) const
{
	if (idx >= row_desc.get_column_num()){
		LOG_WARN("column not found in row", K(idx));
		return ERR_COL_INDEX;
	}else{
		cell = cells[idx];
		return SUCCESS;
	}
}

u32 Row::set_cell(const ColumnDesc & c_desc, Object_s cell)
{
	u32 idx;
	u32 ret = row_desc.get_column_idx(c_desc, idx);
	if (ret != SUCCESS || !cell){
		return ret;
	}else{
		cells[idx] = cell;
		return SUCCESS;
	}
}

u32 Row::set_cell(u32 idx, Object_s cell)
{
	if (idx >= row_desc.get_column_num()){
		return ERR_COL_INDEX;
	}else{
		cells[idx] = cell;
		return SUCCESS;
	}
}

RowAgent::RowAgent()
	:Row(RowDesc(0))
{
}

Row_s RowAgent::make_agent_row(const Row_s & row1, const Row_s & row2)
{
	RowAgent* row = new RowAgent();
	row->agent_row.push_back(row1);
	row->agent_row.push_back(row2);
	return Row_s(row);
}

u32 RowAgent::add_row(const Row_s & row)
{
	agent_row.push_back(row);
	return SUCCESS;
}

u32 RowAgent::get_cell(const ColumnDesc & c_desc, Object_s & cell) const
{
	for (u32 i = 0; i < agent_row.size(); ++i){
		if (agent_row[i]->get_cell(c_desc, cell) == SUCCESS) {
			return SUCCESS;
		}
	}
	return ERR_COL_DESC;
}

bool Row::equal(const Row_s& other) const
{
	if ( cells.size() != other->cells.size()) {
		return false;
	}
	for (u32 i = 0; i < cells.size(); ++i) {
		Object_s result = cells[i]->operator==(other->cells[i]);
		if (!result->bool_value()) {
			return false;
		}
	}
	return true;
}