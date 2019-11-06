#include "IoService.h"
#include "buffer.h"
#include "object.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Storage;
using namespace CatDB::Common;

u64 CatDB::Storage::get_real_page_offset(u32 page_offset)
{
	u64 offset = static_cast<u64>(page_offset);
	return (offset << 14);
}

u32 CatDB::Storage::get_virtual_page_offset(u64 page_offset)
{
	page_offset >>= 14;
	if (page_offset > 0x7FFFFFFF) {
		Log(LOG_ERR, "Storage", "page offset overflow");
		return 0;
	}
	u32 offset = static_cast<u32>(page_offset);
	return offset;
}

u32 CatDB::Storage::get_page_offset_from_row_id(u32 row_id)
{
	return (row_id >> 10);
}

u32 CatDB::Storage::get_beg_row_id_from_page_offset(u32 page_offset)
{
	return (page_offset) << 10;
}

u32 RawRecord::size() const
{
	return sizeof(RawRecord) + column_count * sizeof(u32);
}

RawRecord * RawRecord::make_raw_record(void * ptr)
{/*
	RawRecord* record = reinterpret_cast<RawRecord*>(ptr);
	u8* tail_ptr = reinterpret_cast<u8*>(record) + sizeof(RawRecord);
	record->column_offset = reinterpret_cast<u32*>(tail_ptr);
	return record;*/
	return reinterpret_cast<RawRecord*>(ptr);
}

Page::Page(const Buffer_s & buffer, IoService_s& io_service)
	:buffer_(buffer),
	io_service_(io_service),
	is_dirty(false)
{
	file_header_ = reinterpret_cast<FileHeader*>(buffer->buf);
	page_header_ = reinterpret_cast<PageHeader*>(buffer->buf + sizeof(FileHeader));
	records_space_ = buffer->buf + sizeof(FileHeader) + sizeof(PageHeader);
	free_space_ = buffer->buf + page_header_->free_offset;
	row_info_ = reinterpret_cast<RowInfo*>(free_space_ + page_header_->free_size);
	row_idx_ = page_header_->row_count;
}

Page::~Page()
{
	//写回脏数据
	if (is_dirty){
		if (!io_service_->is_open()){
			Log(LOG_ERR, "Page", "IO service not open");
			return;
		}
		Log(LOG_INFO, "Page", "page %u is dirty and need write to disk", file_header_->page_offset);
		u32 ret = io_service_->write_page(this);
		if (ret != SUCCESS){
			Log(LOG_ERR, "Page", "write page error, error code:%u", ret);
		}
	}
	
}

Page_s Page::make_page(
	IoService_s& io_service,
	u32 table_id,
	u32 page_offset,
	u32 page_pre,
	u32 page_next,
	u32 beg_row_id)
{
	u32 length = PAGE_SIZE;
	Buffer_s buffer = Buffer::make_buffer(length);
	Page* page = new Page(buffer, io_service);
	//初始化file header
	page->file_header_->page_checksum = 0;
	page->file_header_->table_id = table_id;
	page->file_header_->page_offset = page_offset;
	page->file_header_->page_pre = page_pre;
	page->file_header_->page_next = page_next;
	//初始化page header
	page->page_header_->beg_row_id = beg_row_id;
	page->page_header_->end_row_id = beg_row_id;
	page->page_header_->row_count = 0;
	page->page_header_->free_offset = sizeof(FileHeader) + sizeof(PageHeader);
	page->page_header_->free_size = length - sizeof(FileHeader) - sizeof(PageHeader);
	//重新初始化页元信息
	page->free_space_ = page->records_space_;
	page->row_info_ = reinterpret_cast<RowInfo*>(buffer->buf + buffer->length);
	page->row_idx_ = 0;
	return Page_s(page);
}

u32 Page::open()
{
	row_idx_ = 0;
	Log(LOG_TRACE, "Page", "open page %u", file_header_->page_offset);
	return SUCCESS;
}

u32 Page::get_next_row(Row_s & row)
{
	while (row_id_deleted(row_info_[row_idx_].row_id) && row_idx_ < page_header_->row_count){
		++row_idx_;
	}
	if (row_idx_ >= page_header_->row_count){
		Log(LOG_TRACE, "Page", "page %u read at end", file_header_->page_offset);
		return NO_MORE_ROWS;
	}
	u32 ret = project_row(&row_info_[row_idx_], row);
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
	Log(LOG_TRACE, "Page", "reset page %u", file_header_->page_offset);
	return SUCCESS;
}

u32 Page::close()
{
	row_idx_ = 0;
	Log(LOG_TRACE, "Page", "close page %u", file_header_->page_offset);
	return SUCCESS;
}

u32 Page::get_free_space()const
{
	return page_header_->free_size;
}
//反序列化指定row_id记录
u32 Page::select_row(u32 row_id, Row_s & row)const
{
	RowInfo* info = nullptr;
	u32 ret = search_row(row_id, info);
	if (ret != SUCCESS){
		Log(LOG_ERR, "Page", "row %u not found when select row", row_id);
		return ret;
	}
	return project_row(info, row);
}

u32 Page::insert_row(u32& row_id, const Row_s & row)
{
	if (page_header_->free_size < row_width(row) + sizeof(RowInfo) ){
		Log(LOG_ERR, "Page", "page %u have no more memory to store row %u", file_header_->page_offset, row->get_row_id());
		return NO_MORE_PAGE_FREE_SPACE;
	}
	RawRecord* record = RawRecord::make_raw_record(free_space_);
	record->column_count = row->get_row_desc().get_column_num();
	u32 ret = write_row(record, row);
	if (ret == SUCCESS){
		//更新页头信息
		row_id = page_header_->end_row_id;
		++page_header_->end_row_id;
		++page_header_->row_count;
		page_header_->free_offset += row_width(row);
		page_header_->free_size -= (row_width(row) + sizeof(RowInfo));
		
		//更新记录元信息
		--row_info_;
		row_info_->row_id = row_id;
		row_info_->offset = free_space_ - records_space_;
		//更新页的空闲空间信息
		free_space_ += row_width(row);
		//设置页含有脏数据
		is_dirty = true;
		row->set_row_id(row_id);
		Log(LOG_TRACE, "Page", "insert row %u into page %u success", row->get_row_id(), file_header_->page_offset);
	}
	return ret;
}

u32 Page::update_row(u32 row_id, Row_s & row)
{
	Row_s old_row;
	u32 ret = SUCCESS;

	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i){
		Object_s cell;
		ret = row->get_cell(i, cell);
		if (ret == SUCCESS){
			//若有变长数据修改
			//需要先删除旧数据，再插入新数据
			//目前只支持定长数据的原地更新
			if (!cell->is_fixed_length()){
				Log(LOG_INFO, "Page", "found none fixed column when update row %u", row->get_row_id());
				return update_none_fix_row(row_id, row);
			}
		}else{
			return ret;
		}
	}
	RowInfo* info = nullptr;
	ret = search_row(row_id, info);
	if (ret != SUCCESS){
		Log(LOG_ERR, "Page", "row %u not found when update row", row_id);
		return ret;
	}
	//定位目标记录
	RawRecord* record = RawRecord::make_raw_record(records_space_ + info->offset);
	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i){
		Object_s cell;
		ret = row->get_cell(i, cell);
		u32 table_id, column_id;
		ColumnDesc col_desc;
		row->get_row_desc().get_column_desc(i, col_desc);
		col_desc.get_tid_cid(table_id, column_id);
		u8* buf = reinterpret_cast<u8*>(record) + record->column_offset[column_id];
		ret = cell->serialization(buf);
		if (ret != SUCCESS){
			Log(LOG_ERR, "Page", "seria column %u error when update row %u", column_id, row_id);
			break;
		}
	}
	//成功更新记录
	if (ret == SUCCESS){
		Log(LOG_TRACE, "Page", "update row %u success", row_id);
		//设置页含有脏数据
		is_dirty = true;
	}
	return ret;
}

u32 Page::delete_row(u32 row_id)
{
	RowInfo* info = nullptr;
	u32 ret = search_row(row_id, info);
	if (ret != SUCCESS){
		Log(LOG_ERR, "Page", "row %u not found when delete row", row_id);
		return ret;
	}else{
		ret = set_row_id_deleted(info->row_id);
		if (ret == SUCCESS){
			Log(LOG_TRACE, "Page", "delete row %u success", row_id);
			//设置页含有脏数据
			is_dirty = true;
		}
		return ret;
	}
}

u32 Page::page_size() const
{
	return PAGE_SIZE;
	return file_header_->page_next - file_header_->page_offset;
}

u32 Page::page_offset() const
{
	return file_header_->page_offset;
}

u32 Page::next_page_offset() const
{
	return file_header_->page_next;
}

const Buffer_s & Page::page_buffer()const
{
	return buffer_;
}

void Page::reset_page()
{
	free_space_ = buffer_->buf + page_header_->free_offset;
	row_info_ = reinterpret_cast<RowInfo*>(free_space_ + page_header_->free_size);
	row_idx_ = page_header_->row_count;
}

u32 Page::row_id_exists(u32 row_id) const
{
	if (row_id < page_header_->beg_row_id
		|| row_id >= page_header_->end_row_id){
		return ROW_ID_NOT_FOUND;
	}
	
	for (u32 i = 0; i < page_header_->row_count; ++i){
		if (row_id == row_info_[i].row_id){
			if (row_id_deleted(row_info_[i].row_id)){
				return ROW_ID_DELETED;
			}else{
				return SUCCESS;
			}
		}
	}
	return ROW_ID_NOT_FOUND;
}

bool Page::have_free_space_insert(const Row_s & row)
{
	return page_header_->free_size >= (row_width(row) + sizeof(RowInfo));
}

u32 Page::project_all_column(RawRecord* record, Row_s & row)const
{
	RowDesc desc(record->column_count);
	for (u32 i = 0; i < record->column_count; ++i){
		ColumnDesc col_desc;
		col_desc.set_tid_cid(file_header_->table_id, i);
		desc.set_column_desc(i, col_desc);
	}
	row = Row::make_row(desc);
	return SUCCESS;
}

u32 Page::project_row(RowInfo * row_info, Row_s & row)const
{
	//定位目标记录
	RawRecord* record = RawRecord::make_raw_record(records_space_ + row_info->offset);
	//如果行没有任何列描述，默认为投影所有列
	if (!row){
		Log(LOG_INFO, "Page", "project row %u with no row_desc and then project all column", row_info->row_id);
		project_all_column(record, row);
	}
	const RowDesc& desc = row->get_row_desc();
	//反序列化所需的列
	for (u32 i = 0; i < desc.get_column_num(); ++i){
		ColumnDesc col_desc;
		if (desc.get_column_desc(i, col_desc) == SUCCESS){
			u32 table_id, column_id;
			col_desc.get_tid_cid(table_id, column_id);
			if (column_id < record->column_count){
				//定位列所在元数据
				RawData* data = RawData::make_row_data(reinterpret_cast<u8*>(record) + record->column_offset[column_id]);
				//构建列数据对象
				row->set_cell(i, Object::make_object(*data));
			}
		}else{
			Log(LOG_ERR, "Page", "row desc error when project row %u", row_info->row_id);
			return ERR_ROW_DESC;
		}
	}
	u32 id = row_info->row_id;
	row->set_row_id(id);
	row->set_alias_table_id(file_header_->table_id);
	Log(LOG_TRACE, "Page", "project row %u success", id);
	return SUCCESS;
}

u32 Page::update_none_fix_row(u32 row_id, Row_s & row)
{
	Row_s old_row;
	u32 ret = SUCCESS;

	RowInfo* info = nullptr;
	ret = search_row(row_id, info);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "Page", "row %u not found when update row", row_id);
		return ret;
	}
	ret = project_row(info, old_row);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "Page", "row %u not found when update row", row_id);
		return ret;
	}
	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i) {
		ColumnDesc col_desc;
		row->get_row_desc().get_column_desc(i, col_desc);
		u32 idx;
		ret = old_row->get_row_desc().get_column_idx(col_desc, idx);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "Page", "column %u not found when update row", i);
			break;
		}
		Object_s cell;
		row->get_cell(i, cell);
		old_row->set_cell(idx, cell);
	}
	row = old_row;
	return ROW_DATA_TOO_LONG;
}

u32 Page::write_row(RawRecord * record, const Row_s & row)
{
	if (record->column_count > 0){
		record->column_offset[0] = record->size();
	}
	u8* beg = reinterpret_cast<u8*>(record);
	for (u32 i = 0; i < record->column_count; ++i){
		u8* buf = reinterpret_cast<u8*>(record) + record->column_offset[i];
		Object_s cell;
		u32 ret = row->get_cell(i, cell);
		if (ret != SUCCESS){
			Log(LOG_ERR, "Page", "get column %u from row %u error", i, row->get_row_id());
			return ret;
		}
		ret = cell->serialization(buf);
		if (ret != SUCCESS){
			Log(LOG_ERR, "Page", "serial column %u from row %u error", i, row->get_row_id());
			return ret;
		}
		if(i < record->column_count-1 )
			record->column_offset[i + 1] = buf - beg;
	}
	return SUCCESS;
}

u32 Page::search_row(u32 row_id, RowInfo *& info)const
{
	bool found = false;
	for (u32 i = 0; i < page_header_->row_count; ++i){
		if (row_id == row_info_[i].row_id && !row_id_deleted(row_info_[i].row_id)){
			info = &row_info_[i];
			found = true;
			break;
		}
	}
	if (!found){
		return ROW_ID_NOT_FOUND;
	}else{
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

u32 CatDB::Storage::Page::row_width(const Row_s & row) const
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
