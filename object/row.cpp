#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;

ColumnDesc::ColumnDesc()
	:column_id(0)
{
}


void ColumnDesc::set_cid(u32 column_id)
{
	this->column_id = column_id;
}

u32 ColumnDesc::get_cid() const
{
	return column_id;
}

const DataType& ColumnDesc::get_data_type()const
{
	return data_type;
}

void ColumnDesc::set_data_type(const DataType& type)
{
	data_type = type;
}

RowDesc::RowDesc()
{
}

RowDesc::RowDesc(u32 column_count)
	:column_desc(column_count)
{
}

u32 RowDesc::get_column_desc(u32 idx, ColumnDesc & c_desc) const
{
	c_desc = column_desc[idx];
	return SUCCESS;
}

u32 RowDesc::set_column_desc(u32 idx, const ColumnDesc & c_desc)
{
	if (idx >= column_desc.size()){
		return ERR_COL_DESC;
	}else{
		column_desc[idx] = c_desc;
		return SUCCESS;
	}
}

u32 RowDesc::get_column_num() const
{
	return column_desc.size();
}

u32 RowDesc::add_column_desc(const ColumnDesc & col_desc)
{
	column_desc.push_back(col_desc);
	return SUCCESS;
}

Row::Row(int column_count)
	:cells(column_count)
{
}

Row::~Row()
{
	
}

Row_s Row::make_row(int column_count)
{
	Row* row = new Row(column_count);
	return Row_s(row);
}

Row_s Row::deep_copy(const Row_s &other)
{
	Row* row = new Row(other->get_cell_num());
	row->cells = other->cells;
	row->op_id = other->op_id;
	return Row_s(row);
}

u32 Row::get_cell_num() const
{
	return cells.size();
}

u32 Row::get_cell(u32 idx, Object_s &cell) const
{
	cell = cells[idx];
	return SUCCESS;
}

u32 Row::set_cell(u32 idx, Object_s &cell)
{
	cells[idx] = cell;
	return SUCCESS;
}

u32 Row::equal(const Row_s& other, bool &res) const
{
	u32 ret = SUCCESS;
	int cmp_res = 0;
	if ( cells.size() != other->cells.size()) {
		ret = ERR_UNEXPECTED;
		return ret;
	}
	res = true;
	for (u32 i = 0; res && i < cells.size(); ++i) {
		CHECK(cells[i]->compare(other->cells[i], cmp_res));
		res = 0 == cmp_res;
	}
	return ret;
}