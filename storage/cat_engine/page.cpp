#include "buffer.h"
#include "object.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Storage;
using namespace CatDB::Common;

u64 Page::get_real_page_offset(u32 page_offset)
{
	u64 offset = static_cast<u64>(page_offset);
	return (offset << 14);
}

u32 Page::get_virtual_page_offset(u64 page_offset)
{
	page_offset >>= 14;
	if (page_offset > 0x7FFFFFFF) {
		LOG_ERR("page offset overflow", K(page_offset));
		return 0;
	}
	u32 offset = static_cast<u32>(page_offset);
	return offset;
}

u32 Page::get_page_offset_from_row_id(u32 row_id)
{
	return (row_id >> 10);
}

u32 Page::get_beg_row_id_from_page_offset(u32 page_offset)
{
	return (page_offset) << 10;
}

u32 RawRecord::size() const
{
	return sizeof(RawRecord) + column_count * sizeof(u32);
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

Page_s Page::make_page(u32 page_offset)
{
	u32 length = PAGE_SIZE;
	Buffer_s buffer = Buffer::make_buffer(length);
	Page* page = new Page(buffer);
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
	page_header_->beg_row_id = get_beg_row_id_from_page_offset(page_offset);
	page_header_->end_row_id = get_beg_row_id_from_page_offset(page_offset);
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

u32 Page::get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	while (row_id_deleted(row_info_[row_idx_].row_id) && row_idx_ < page_header_->row_count) {
		++row_idx_;
	}
	if (row_idx_ >= page_header_->row_count) {
		LOG_TRACE("page read at end", K(page_header_));
		return NO_MORE_ROWS;
	}
	CHECK(deserialize_row(&row_info_[row_idx_], row));
	++row_idx_;
	return ret;
}

bool Page::have_row() const
{
	u32 idx = row_idx_;
	while (idx < page_header_->row_count && row_id_deleted(row_info_[idx].row_id)){
		++idx;
	}
	if (idx >= page_header_->row_count){
		return false;
	}else{
		return true;
	}
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

u32 Page::insert_row(const Row_s & row)
{
	if (page_header_->free_size < row_width(row) + sizeof(RowInfo) ) {
		LOG_ERR("page have no more memory to store row", K(page_header_), K(row));
		return NO_MORE_PAGE_FREE_SPACE;
	}
	u32 store_len = 0;
	u32 ret = serialize_row(row, store_len);
	if (ret == SUCCESS) {
		//更新页头信息
		u32 row_id = page_header_->end_row_id;
		++page_header_->end_row_id;
		++page_header_->row_count;
		page_header_->free_offset += store_len;
		page_header_->free_size -= (store_len + sizeof(RowInfo));
		
		//更新记录元信息
		--row_info_;
		row_info_->row_id = row_id;
		row_info_->offset = free_space_ - records_space_;
		//更新页的空闲空间信息
		free_space_ += store_len;
		//设置页含有脏数据
		is_dirty = true;
		row->set_row_id(row_id);
		//LOG_TRACE("insert row into page success", K(page_header_), K(row));
	}
	return ret;
}

u32 Page::update_row(u32 row_id, Row_s & row)
{
	return update_none_fix_row(row_id, row);
	u32 ret = SUCCESS;
	Row_s old_row;

	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i) {
		Object_s cell;
		ret = row->get_cell(i, cell);
		if (ret == SUCCESS) {
			//若有变长数据修改
			//需要先删除旧数据，再插入新数据
			//目前只支持定长数据的原地更新
			if (!cell->is_fixed_length()) {
				//LOG_TRACE("found none fixed column when update row", K(row));
				return update_none_fix_row(row_id, row);
			}
		} else {
			return ret;
		}
	}
	RowInfo* info = nullptr;
	ret = search_row(row_id, info);
	if (ret != SUCCESS) {
		LOG_ERR("row not found when update row", K(row_id));
		return ret;
	}
	//定位目标记录
	RawRecord* record = RawRecord::make_raw_record(records_space_ + info->offset);
	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i) {
		Object_s cell;
		ret = row->get_cell(i, cell);
		ColumnDesc col_desc;
		row->get_row_desc().get_column_desc(i, col_desc);
		u32 column_id = col_desc.get_cid();
		u8* buf = reinterpret_cast<u8*>(record) + record->column_offset[column_id];
		ret = cell->serialization(buf);
		if (ret != SUCCESS) {
			LOG_ERR("seria column error when update row", K(column_id), K(row_id));
			break;
		}
	}
	//成功更新记录
	if (ret == SUCCESS) {
		//LOG_TRACE("update row success", K(row));
		//设置页含有脏数据
		is_dirty = true;
	}
	return ret;
}

u32 Page::delete_row(u32 row_id)
{
	RowInfo* info = nullptr;
	u32 ret = search_row(row_id, info);
	if (ret != SUCCESS) {
		LOG_ERR("row not found when delete row", K(row_id));
		return ret;
	} else {
		ret = set_row_id_deleted(info->row_id);
		if (ret == SUCCESS) {
			//LOG_TRACE("delete row success", K(row_id));
			//设置页含有脏数据
			is_dirty = true;
		}
		return ret;
	}
}

u32 Page::page_size() const
{
	return PAGE_SIZE;
}

u32 Page::page_offset() const
{
	return page_header_->page_offset;
}

u32 Page::next_page_offset() const
{
	return page_header_->page_next;
}

Buffer_s & Page::page_buffer()
{
	return buffer_;
}

bool Page::have_free_space_insert(const Row_s & row)
{
	return page_header_->free_size >= (row_width(row) + sizeof(RowInfo));
}

//如果调用者没有说明投影列，则所有列信息修复行描述
u32 Page::fix_row_desc(RawRecord* record, Row_s & row, u32 table_id)const
{
	RowDesc desc(record->column_count);
	for (u32 i = 0; i < record->column_count; ++i){
		ColumnDesc col_desc;
		col_desc.set_tid_cid(table_id, i);
		desc.set_column_desc(i, col_desc);
	}
	row = Row::make_row(desc);
	return SUCCESS;
}

u32 Page::deserialize_row(RowInfo * row_info, Row_s & row)const
{
	//定位目标记录
	RawRecord* record = RawRecord::make_raw_record(records_space_ + row_info->offset);
	const RowDesc& desc = row->get_row_desc();
	//反序列化所需的列
	u32 column_id;
	for (u32 i = 0; i < desc.get_column_num(); ++i) {
		ColumnDesc col_desc;
		if (desc.get_column_desc(i, col_desc) == SUCCESS) {
			u32 column_id = col_desc.get_cid();
			if (column_id < record->column_count) {
				//定位列所在元数据
				RawData* data = RawData::make_row_data(reinterpret_cast<u8*>(record) + record->column_offset[column_id]);
				//构建列数据对象
				row->set_cell(i, Object::make_object(*data));
			} else if (column_id == ROWID_COLUMN_ID) {
				row->set_cell(i, Number::make_object(row_info->row_id, -1));
			}
		} else {
			LOG_ERR("row desc error when project row", K(row_info));
			return ERR_ROW_DESC;
		}
	}
	row->set_row_id(row_info->row_id);
	return SUCCESS;
}

u32 Page::update_none_fix_row(u32 row_id, Row_s & row)
{
	u32 ret = SUCCESS;
	Row_s old_row;
	RowInfo* info = nullptr;
	CHECK(search_row(row_id, info));
	RawRecord* record = RawRecord::make_raw_record(records_space_ + info->offset);
	RowDesc &desc = row->get_row_desc();
	ColumnDesc col_desc;
	MY_ASSERT(desc.get_column_num() > 0);
	CHECK(desc.get_column_desc(0, col_desc));
	u32 table_id = col_desc.get_tid();
	fix_row_desc(record, old_row, table_id);
	CHECK(deserialize_row(info, old_row));
	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i) {
		ColumnDesc col_desc;
		row->get_row_desc().get_column_desc(i, col_desc);
		u32 idx;
		CHECK(old_row->get_row_desc().get_column_idx(col_desc, idx));
		Object_s cell;
		row->get_cell(i, cell);
		old_row->set_cell(idx, cell);
	}
	row = old_row;
	return ROW_DATA_TOO_LONG;
}

u32 Page::serialize_row(const Row_s & row, u32 & store_len)
{
	RawRecord* record = RawRecord::make_raw_record(free_space_);
	record->column_count = row->get_row_desc().get_column_num();
	if (record->column_count > 0) {
		record->column_offset[0] = record->size();
	}
	u8* beg = reinterpret_cast<u8*>(record);
	for (u32 i = 0; i < record->column_count; ++i) {
		u8* buf = reinterpret_cast<u8*>(record) + record->column_offset[i];
		Object_s cell;
		u32 ret = row->get_cell(i, cell);
		if (ret != SUCCESS) {
			LOG_ERR("get column from row error", K(i), K(row));
			return ret;
		}
		ret = cell->serialization(buf);
		if (ret != SUCCESS) {
			LOG_ERR("serial column from row error", K(i), K(row));
			return ret;
		}
		if (i < record->column_count - 1) {
			record->column_offset[i + 1] = buf - beg;
		}
	}
	store_len = row_width(row);
	return SUCCESS;
}

u32 Page::search_row(u32 row_id, RowInfo *& info)const
{
	if (row_id >= page_header_->end_row_id) {
		return ROW_ID_NOT_FOUND;
	} else {
		u32 offset = page_header_->end_row_id - row_id - 1;
		if (offset > page_header_->row_count) {
			return ROW_ID_NOT_FOUND;
		} else {
			info = &row_info_[offset];
			return SUCCESS;
		}
	}
	
	bool found = false;
	for (u32 i = 0; i < page_header_->row_count; ++i) {
		if (row_id == row_info_[i].row_id && !row_id_deleted(row_info_[i].row_id)) {
			info = &row_info_[i];
			found = true;
			break;
		}
	}
	if (!found) {
		return ROW_ID_NOT_FOUND;
	} else {
		return SUCCESS;
	}
}

u32 Page::set_row_id_deleted(u32 & row_id)
{
	row_id |= 0x80000000;
	return SUCCESS;
}

bool Page::row_id_deleted(u32 row_id)const
{
	return (row_id & 0x80000000) != 0;
}

u32 Page::row_width(const Row_s & row) const
{
	u32 width = 0;
	u32 count = row->get_row_desc().get_column_num();
	Object_s cell;
	for (u32 i = 0; i < count; ++i) {
		row->get_cell(i, cell);
		width += cell->width();
		width += sizeof(RawData);
	}
	width += sizeof(RawRecord);
	width += count * sizeof(u32);
	return width;
}