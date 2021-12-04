#include "cat_table_space.h"
#include "cat_io_service.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Storage;
using namespace CatDB::Common;

CatTableSpace::CatTableSpace()
	:cur_page_offset(0)
{
	io = CatIoService::make_cat_io_service();
}

CatTableSpace::~CatTableSpace()
{
}

TableSpace_s CatTableSpace::make_table_space(const String& table_name, 
											 const String & database, 
											 const Vector<String> &args,
											 double sample_size)
{
	CatTableSpace* table_space = new CatTableSpace;
	table_space->database = database;
	table_space->table_name = table_name;
	if (sample_size > 0.99) {
		table_space->page_skip_size = 0;
	} else {
		table_space->page_skip_size = 1.0 / sample_size;
	}
	return TableSpace_s(table_space);
}

u32 CatTableSpace::open()
{
	LOG_TRACE("open table space", K(database), K(table_name));
	String path = table_path(database, table_name);
	io->open(path);
	cur_page_offset = 0;
	reset_all_page();
	return SUCCESS;
}

u32 CatTableSpace::get_next_row(Row_s & row)
{
	Page_s page;
	u32 ret = get_page_from_offset(cur_page_offset, page);
	if (ret == END_OF_TABLE_SPACE) {
		return ret;
	}
	else if (ret != SUCCESS){
		LOG_ERR("can not get page", K(cur_page_offset), err_string(ret));
		return ret;
	}
	while (!page->have_row()){
		LOG_TRACE("page have read end, load next page", K(cur_page_offset));
		//读取下一页
		pages.erase(pages.find(cur_page_offset));
		cur_page_offset = page->next_page_offset() + page_skip_size;
		//下一页不在内存中
		ret = get_page_from_offset(cur_page_offset, page);
		if (ret == END_OF_TABLE_SPACE) {
			return ret;
		}
		else if (ret != SUCCESS){
			LOG_ERR("get page failed", K(cur_page_offset), err_string(ret));
			return ret;
		}
	}
	
	//TODO filter 放这还是下压到page层面？
	return page->get_next_row(row);
}

u32 CatTableSpace::reset()
{
	cur_page_offset = 0;
	reset_all_page();
	return SUCCESS;
}

u32 CatTableSpace::close()
{
	for (auto iter = pages.begin(); iter != pages.end(); ++iter) {
		iter->second->close();
		pages_copy.push_back(iter->second);
	}
	pages.clear();
	pages_copy.clear();
	LOG_TRACE("close table space", K(table_name));
	return SUCCESS;
}

u32 CatTableSpace::insert_row(const Row_s & row)
{
	u32 row_id;
	Page_s page;
	get_last_page(page);
	//当前页能够存放下行
	if (page->have_free_space_insert(row)){
		return page->insert_row(row_id, row);
	}else{//创建新的页存放
		LOG_TRACE("page have no free space to insert row", K(page));
		u32 offset = page->next_page_offset();
		pages.erase(page);
		create_page(offset, page);
		return page->insert_row(row_id, row);
	}
}

u32 CatTableSpace::update_row(const Row_s & row)
{
	u32 ret = SUCCESS;
	u32 row_id = row->get_row_id();
	Page_s page;
	CHECK(get_page_from_row_id(row_id, page));
	Row_s old_row = row;
	ret = page->update_row(row_id, old_row);
	if (ret == ROW_DATA_TOO_LONG) {
		CHECK(insert_row(old_row));
		CHECK(delete_row(row_id));
	}
	return ret;
}

u32 CatTableSpace::delete_row(u32 row_id)
{
	Page_s page;
	u32 ret = get_page_from_row_id(row_id, page);
	if (ret == SUCCESS){
		return page->delete_row(row_id);
	}else{
		LOG_ERR("can not delete row ", K(row_id), err_string(ret));
		return ret;
	}
}

u32 CatTableSpace::delete_all_row()
{
	String path = table_path(database, table_name);
	u32 ret = io->clear_file(path);
	if (ret != SUCCESS) {
		LOG_ERR("delete table all rows", K(table_name), err_string(ret));
		return ret;
	}
	else {
		return SUCCESS;
	}
}

u32 CatTableSpace::get_page_from_row_id(u32 row_id, Page_s& page)
{
	u32 page_offset = get_page_offset_from_row_id(row_id);
	if (pages.find(page_offset) == pages.end()) {
		LOG_TRACE("page is not in memory", K(page_offset));
		u32 ret = read_page(page_offset, page);
		return ret;
	}
	else {
		page = pages[page_offset];
		return SUCCESS;
	}
}

u32 CatTableSpace::get_page_from_offset(u32 page_offset, Page_s& page)
{
	if (pages.find(page_offset) == pages.end()) {
		LOG_TRACE("page is not in memory", K(page_offset));
		u32 ret = read_page(page_offset, page);
		return ret;
	}
	else {
		page = pages[page_offset];
		return SUCCESS;
	}
}

u32 CatTableSpace::read_page(u32 page_offset, Page_s& page)
{
	u64 offset = 0;
	u32 ret = io->end_offset(offset);
	if (ret == EMPTY_TABLE_SPACE || offset < page_offset) {
		LOG_TRACE("page not in table space", K(page_offset));
		return END_OF_TABLE_SPACE;
	}
	page = Page::make_page(io, page_offset, 0, 0, page_offset);
	
	ret = io->read_page(page);
	if (ret != SUCCESS) {
		return ret;
	}
	else {
		ret = page->open();
		if (ret != SUCCESS) {
			LOG_ERR("open page failed", K(cur_page_offset), err_string(ret));
			return ret;
		}
		pages[page_offset] = page;
		return ret;
	}
}

u32 CatTableSpace::create_page(u32 page_offset, Page_s& page)
{
	u32 page_pre, page_next;
	if (page_offset == 0)
		page_pre = 0;
	else
		page_pre = page_offset - 1;
	page_next = page_offset + 1;
	u32 beg_row_id = get_beg_row_id_from_page_offset(page_offset);
	page = Page::make_page(io, page_offset, page_pre, page_next, beg_row_id);
	pages[page_offset] = page;
	page->open();
	return SUCCESS;
}

u32 CatTableSpace::get_last_page(Page_s& page)
{
	u64 offset = 0;
	u32 ret = io->end_offset(offset);
	//空表
	if (ret == EMPTY_TABLE_SPACE) {
		//内存中也没有数据页
		if (pages.empty()) {
			create_page(offset, page);
			return SUCCESS;
		}
		else {//否则内存中最后一页
			auto iter = pages.end();
			--iter;
			page = iter->second;
			return SUCCESS;
		}
	}
	else {
		//获取磁盘的最后一页
		u32 ret = get_page_from_offset(offset, page);
		if (ret != SUCCESS) {
			return ret;
		}
		auto iter = pages.end();
		--iter;
		//内存最后一页在磁盘最后一页之后
		if (iter->first > offset)
			page = iter->second;
		return SUCCESS;
	}
}

void CatTableSpace::reset_all_page()
{
	for (auto iter = pages.begin(); iter != pages.end(); ++iter) {
		iter->second->reset();
	}
}
