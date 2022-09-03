#include "csv_table_space.h"
#include "io_service.h"
#include "obj_number.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Storage;
using namespace CatDB::Common;

CSVTableSpace::CSVTableSpace(const Vector<String> &args, bool read_only)
{
	u32 i = 1;
	if (i < args.size()) {
		path = args[i++];
	}
	if (i < args.size()) {
		if (!args[i].empty()) {
			field_split = args[i][0];
		}
		++i;
	}
	if (i < args.size()) {
		if (args[i] == "new line") {
			line_split = '\n';
		} else if (!args[i].empty()) {
			line_split = args[i][0];
		}
		++i;
	}
	if (i < args.size()) {
		field_count = std::stod(args[i++]);
	}
	io = IoService::make_io_service();
	if (!read_only) {
		io->move_to_file_end();
	}
	column_pos = new u32[field_count];
	buffer = new char[buffer_size];
	reset();
}

CSVTableSpace::~CSVTableSpace()
{
	if (need_write_buffer) {
		buffer_full();
	}
	delete[] column_pos;
	delete[] buffer;
}

TableSpace_s CSVTableSpace::make_table_space(const String& table_name, 
											 const String & database, 
											 const Vector<String> &args,
											 double sample_value,
											 bool read_only)
{
	CSVTableSpace* table_space = new CSVTableSpace(args, read_only);
	table_space->database = database;
	table_space->table_name = table_name;
	table_space->sample_value = sample_value;
	return TableSpace_s(table_space);
}

u32 CSVTableSpace::open()
{
	LOG_TRACE("open table space", K(database), K(table_name));
	io->open(path);
	return SUCCESS;
}

u32 CSVTableSpace::get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	ret = buffer_read();
	if (SUCC(ret)) {
		CHECK(read_row(row));
	}
	return ret;
}

u32 CSVTableSpace::reset()
{
	buf_start = 0;
	buf_end = 0;
	buf_read_pos = 0;
	column_cnt = 0;
	row_id = 0;
	need_write_buffer = false;
	return SUCCESS;
}

u32 CSVTableSpace::close()
{
	LOG_TRACE("close table space", K(table_name));
	return SUCCESS;
}

u32 CSVTableSpace::insert_row(const Row_s & row)
{
	u32 ret = SUCCESS;
	CHECK(write_row(row));
	need_write_buffer = true;
	return ret;
}

u32 CSVTableSpace::get_row(u32 row_id, Row_s & row)
{
	return OPERATION_NOT_SUPPORT;
}

u32 CSVTableSpace::delete_row(u32 row_id)
{
	return OPERATION_NOT_SUPPORT;
}

u32 CSVTableSpace::delete_all_row()
{
	return OPERATION_NOT_SUPPORT;
}

u32 CSVTableSpace::update_row(u32 row_id, const Row_s& update_row, const Row_s& access_row)
{
	return OPERATION_NOT_SUPPORT;
}

u32 CSVTableSpace::buffer_empty()
{
	u32 ret = SUCCESS;
	u64 size = PAGE_SIZE;
	if (buffer_size - buf_end < size) {
		ret = ROW_DATA_TOO_LONG;
		return ret;
	}
	CHECK(io->read_buffer(buffer + buf_end, size));
	if (size == 0) {
		ret = NO_MORE_ROWS;
	} else {
		buf_end += size;
		ret = buffer_read();
	}
	return ret;
}

u32 CSVTableSpace::buffer_read()
{
	u32 ret = SUCCESS;
	bool have_new_row = false;
	while (buf_read_pos < buf_end) {
		if (buffer[buf_read_pos] == field_split) {
			column_pos[column_cnt++] = buf_read_pos;
		}
		if (buffer[buf_read_pos] == line_split) {
			have_new_row = true;
			++buf_read_pos;
			if (sample_value < 0) {
				break;
			} else if (std::rand() > sample_value){
				++buf_read_pos;
				continue;
			} else {
				break;
			}
		}
		++buf_read_pos;
	}
	if (!have_new_row && buf_read_pos >= buf_end) {
		if (buffer_size - buf_end < PAGE_SIZE) {
			ret = buffer_move();
		} else {
			ret = buffer_empty();
		}
	}
	return ret;
}

u32 CSVTableSpace::buffer_move()
{
	u32 ret = SUCCESS;
	memmove(buffer, buffer+buf_start, buf_end - buf_start);
	for (u32 i = 0; i < column_cnt; ++i) {
		column_pos[i] -= buf_start;
	}
	buf_read_pos -= buf_start;
	buf_end = buf_end - buf_start;
	buf_start = 0;
	ret = buffer_empty();
	return ret;
}

u32 CSVTableSpace::read_row(Row_s& row)
{
	u32 ret = SUCCESS;
	//反序列化所需的列
	u32 column_id;
	for (u32 i = 0; i < row->get_cell_num(); ++i)
	{
		ColumnDesc col_desc;
		CHECK(access_desc.get_column_desc(i, col_desc));
		column_id = col_desc.get_cid();
		if (column_id < column_cnt) {
			u32 start = buf_start;
			u32 end = column_pos[ column_id ];
			if (column_id > 0) {
				start = column_pos[ column_id-1 ] + 1;
			}
			Object_s value = Object::make_object(buffer + start, end - start, col_desc.get_data_type());
			//TODO cast object
			row->set_cell(i, value);
		} else if (column_id == ROWID_COLUMN_ID) {
			Object_s row_id_value = Number::make_int_object(row_id);
			row->set_cell(i, row_id_value);
		} else {
			LOG_ERR("row desc error when project row", K(column_cnt), K(col_desc));
			return ERR_ROW_DESC;
		}
	}
	column_cnt = 0;
	buf_start = buf_read_pos;
	++row_id;
	return ret;
}

u32 CSVTableSpace::write_row(const Row_s& row)
{
	u32 ret = SUCCESS;
	//序列化所有的列
	for (u32 i = 0; i < row->get_cell_num(); ++i)
	{
		Object_s value;
		row->get_cell(i, value);
		String cell = value->to_string();
		if (buffer_size - buf_end < cell.size() + 2) {
			CHECK(buffer_full());
		}
		if (buffer_size - buf_end < cell.size() + 2) {
			ret = ROW_DATA_TOO_LONG;
		}
		memcpy(buffer + buf_end, cell.c_str(), cell.size());
		buf_end += cell.size();
		buffer[buf_end++] = field_split;
	}
	buffer[buf_end++] = line_split;
	return ret;
}

u32 CSVTableSpace::buffer_full()
{
	u32 ret = SUCCESS;
	u64 size = buf_end - buf_start;
	if (size > 0) {
		CHECK(io->write_buffer(buffer, size));
	}
	buf_end = 0;
	return ret;
}