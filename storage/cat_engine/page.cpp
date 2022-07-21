#include "buffer.h"
#include "obj_number.h"
#include "object.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Storage;
using namespace CatDB::Common;

u32 RawRecord::size() const
{
	return sizeof(column_count) + column_count * sizeof(column_offset);
}

bool RawRecord::is_null(u32 i)
{
	return column_offset[i] & 0x8000;
}

void RawRecord::set_null(u32 i)
{
	column_offset[i] |= 0x8000;
}

u16 RawRecord::get_offset(u32 i)
{
	return column_offset[i] & 0x7fff;
}

RawRecord * RawRecord::make_raw_record(void * ptr)
{
	return reinterpret_cast<RawRecord*>(ptr);
}

Page::Page(const Buffer_s & buffer)
	:buffer_(buffer),
	is_dirty(false),
	row_idx_(0)
{
	page_header_ = reinterpret_cast<PageHeader*>(buffer->buf);
	records_space_ = buffer->buf + sizeof(PageHeader);
	free_space_ = NULL;
	row_info_ = NULL;
}

Page::~Page()
{
	//写回脏数据
	if (is_dirty){
		LOG_ERR("page is dirty and need write to disk", K(page_header_));
	}
}

Page_s Page::make_page()
{
	u32 length = PAGE_SIZE;
	Buffer_s buffer = Buffer::make_buffer(length);
	Page* page = new Page(buffer);
	page->clear_page(0);
	return Page_s(page);
}

Page_s Page::make_page(u32 page_offset, const RowDesc &row_desc)
{
	u32 length = PAGE_SIZE;
	Buffer_s buffer = Buffer::make_buffer(length);
	Page* page = new Page(buffer);
	page->row_desc = row_desc;
	page->clear_page(page_offset);
	return Page_s(page);
}

void Page::clear_page(u32 page_offset)
{
	//初始化page header
	page_header_->page_checksum = 0;
	page_header_->page_offset = page_offset;
	page_header_->page_pre = page_offset - 1;
	page_header_->page_next = page_offset + 1;
	page_header_->row_count = 0;
	page_header_->free_offset = sizeof(PageHeader);
	page_header_->free_size = buffer_->length - sizeof(PageHeader);
	//重新初始化页元信息
	free_space_ = records_space_;
	row_info_ = reinterpret_cast<RowInfo*>(buffer_->buf + buffer_->length);
	row_idx_ = 0;
}

u32 Page::open()
{
	u32 ret = SUCCESS;
	row_idx_ = 0;
	MY_ASSERT(buffer_, page_header_);
	free_space_ = buffer_->buf + page_header_->free_offset;
	row_info_ = reinterpret_cast<RowInfo*>(free_space_ + page_header_->free_size);
	LOG_TRACE("open page", K(page_header_));
	return ret;
}

u32 Page::reset()
{
	row_idx_ = 0;
	LOG_TRACE("reset page", K(page_header_));
	return SUCCESS;
}

u32 Page::close()
{
	row_idx_ = 0;
	LOG_TRACE("close page", K(page_header_));
	return SUCCESS;
}

u32 Page::get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	while (row_idx_ < page_header_->row_count && is_row_deleted(row_info_ + get_idx_from_row_id(row_idx_))) {
		++row_idx_;
	}
	if (row_idx_ >= page_header_->row_count) {
		return NO_MORE_ROWS;
	}
	u32 row_id = get_row_id_from_idx(row_idx_);
	u32 idx = get_idx_from_row_id(row_idx_);
	CHECK(deserialize_row(&row_info_[idx], row_id, row));
	++row_idx_;
	return ret;
}

u32 Page::insert_row(const Row_s & row)
{
	if (page_header_->free_size < row_width(row)) {
		return NO_MORE_PAGE_FREE_SPACE;
	}
	u32 store_len = 0;
	u32 ret = serialize_row(row, store_len);
	if (ret == SUCCESS) {
		//更新页头信息
		++page_header_->row_count;
		page_header_->free_offset += store_len;
		page_header_->free_size -= (store_len + sizeof(RowInfo));
		//更新记录元信息
		--row_info_;
		row_info_->offset = free_space_ - records_space_;
		//更新页的空闲空间信息
		free_space_ += store_len;
		//设置页含有脏数据
		is_dirty = true;
		LOG_TRACE("insert row into page success", K(page_header_), K(row));
	}
	return ret;
}

u32 Page::get_row(u32 row_id, Row_s & row)
{
	u32 ret = SUCCESS;
	RowInfo* info = nullptr;
	CHECK(search_row(row_id, info));
	CHECK(deserialize_row(info, row_id, row));
	return ret;
}

u32 Page::delete_row(u32 row_id)
{
	RowInfo* info = nullptr;
	u32 ret = SUCCESS;
	CHECK(search_row(row_id, info));
	CHECK(set_row_deleted(info));
	LOG_TRACE("delete row success", K(row_id));
	//设置页含有脏数据
	is_dirty = true;
	return ret;
}

u32 Page::deserialize_row(RowInfo * row_info, u32 row_id, Row_s & row)const
{
	u32 ret = SUCCESS;
	if (is_row_deleted(row_info)) {
		LOG_ERR("get deleted row", K(row_id));
		ret = ERR_UNEXPECTED;
		return ret;
	}
	//定位目标记录
	RawRecord* record = RawRecord::make_raw_record(records_space_ + row_info->offset);
	//反序列化所需的列
	u32 column_id;
	for (u32 i = 0; i < row_desc.get_column_num(); ++i) {
		ColumnDesc col_desc;
		CHECK(row_desc.get_column_desc(i, col_desc));
		u32 column_id = col_desc.get_cid();
		if (column_id < record->column_count) {
			//定位列所在元数据
			u8* data = reinterpret_cast<u8*>(record) + record->get_offset(column_id);
			u32 size = record->get_offset(column_id+1) - record->get_offset(column_id);
			//构建列数据对象
			Object_s cell = Object::make_object(data, size, col_desc.get_data_type());
			if (record->is_null(column_id)) {
				cell->set_null();
			}
			row->set_cell(i, cell);
		} else if (column_id == ROWID_COLUMN_ID) {
			Object_s row_id_value = Number::make_int_object(row_id);
			row->set_cell(i, row_id_value);
		} else {
			LOG_ERR("row desc error when project row", K(row_id), K(col_desc));
			return ERR_ROW_DESC;
		}
	}
	return SUCCESS;
}

u32 Page::serialize_row(const Row_s & row, u32 & store_len)
{
	u32 ret = SUCCESS;
	RawRecord* record = RawRecord::make_raw_record(free_space_);
	record->column_count = row->get_cell_num() + 1;
	record->column_offset[0] = record->size();
	u8* beg = reinterpret_cast<u8*>(record);
	u8* buf = NULL;
	for (u32 i = 0; i < record->column_count - 1; ++i) {
		buf = beg + record->get_offset(i);
		Object_s cell;
		CHECK(row->get_cell(i, cell));
		CHECK(cell->serialization(buf));
		record->column_offset[i + 1] = buf - beg;
		if (cell->is_null()) {
			record->set_null(i);
		}
	}
	store_len = buf - beg;
	return SUCCESS;
}

u32 Page::search_row(u32 row_id, RowInfo *& info)const
{
	u32 idx = get_idx_from_row_id(row_id);
	if (idx >= page_header_->row_count) {
		return ROW_ID_NOT_FOUND;
	} else {
		info = &row_info_[idx];
		return SUCCESS;
	}
}

u32 Page::set_row_deleted(RowInfo* info)
{
	info->offset |= 0x8000;
	return SUCCESS;
}

bool Page::is_row_deleted(RowInfo* info)const
{
	return (info->offset & 0x8000) != 0;
}

u32 Page::row_width(const Row_s & row) const
{
	u32 width = 0;
	u32 count = row->get_cell_num();
	Object_s cell;
	for (u32 i = 0; i < count; ++i) {
		row->get_cell(i, cell);
		width += cell->width();
	}
	width += sizeof(RawRecord);
	width += count * sizeof(RawRecord::column_offset);
	width += sizeof(RowInfo);
	return width;
}