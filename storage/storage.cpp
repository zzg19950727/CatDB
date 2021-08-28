#include "IoService.h"
#include "storage.h"
#include "error.h"
#include "page.h"
#include "log.h"
using namespace CatDB::Storage;

PageManager::PageManager(const String& database, const String& table_name)
{
	io = IoService::make_io_service();
	io->open(table_name);
}

PageManager::~PageManager()
{

}

u32 PageManager::get_page_from_row_id(u32 row_id, Page_s& page)
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

u32 PageManager::get_page_from_offset(u32 page_offset, Page_s& page)
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

u32 PageManager::read_page(u32 page_offset, Page_s& page)
{
	u32 offset = 0;
	u32 ret = io->end_offset(offset);
	if (ret == EMPTY_TABLE_SPACE || offset < page_offset) {
		LOG_TRACE("page not in table space", K(page_offset));
		return END_OF_TABLE_SPACE;
	}
	page = Page::make_page(io, get_table_id(), page_offset, 0, 0, page_offset);
	ret = io->read_page(page);
	if (ret != SUCCESS) {
		return ret;
	}
	else {
		ret = page->open();
		if (ret != SUCCESS) {
			return ret;
		}
		pages[page_offset] = page;
		return ret;
	}
}

u32 PageManager::create_page(u32 page_offset, Page_s& page)
{
	u32 page_pre, page_next;
	if (page_offset == 0)
		page_pre = 0;
	else
		page_pre = page_offset - PAGE_SIZE;
	page_next = page_offset + PAGE_SIZE;
	u32 beg_row_id = get_beg_row_id_from_page_offset(page_offset);
	page = Page::make_page(io, get_table_id(), page_offset, page_pre, page_next, beg_row_id);
	pages[page_offset] = page;
	return SUCCESS;
}

u32 PageManager::get_last_page(Page_s& page)
{
	u32 offset = 0;
	u32 ret = io->end_offset(offset);
	
	if (ret == EMPTY_TABLE_SPACE) {
		
		if (pages.empty()) {
			create_page(offset, page);
			return SUCCESS;
		}
		else {
			auto iter = pages.end();
			--iter;
			page = iter->second;
			return SUCCESS;
		}
	}
	else {
		
		u32 ret = get_page_from_offset(offset, page);
		if (ret != SUCCESS) {
			return ret;
		}
		auto iter = pages.end();
		--iter;
		
		if (iter->first > offset)
			page = iter->second;
		return SUCCESS;
	}
}

u32 PageManager::delete_all_row()
{
	u32 ret = io->clear_file(table_name);
	if (ret != SUCCESS) {
		LOG_ERR("delete table all rows failed", K(table_name), err_string(ret));
		return ret;
	}
	else {
		return SUCCESS;
	}
}

u32 PageManager::clear()
{
	for (auto iter = pages.begin(); iter != pages.end(); ++iter) {
		iter->second->close();
		pages_copy.push_back(iter->second);
	}
	pages.clear();
	pages_copy.clear();
	LOG_TRACE("clear page manager", K(table_name));
	return SUCCESS;
}

u32 PageManager::get_table_id() const
{
	Hash<String> hash;
	return hash(database + "." + table_name);
}

void PageManager::reset_all_page()
{
	for (auto iter = pages.begin(); iter != pages.end(); ++iter) {
		iter->second->reset();
	}
}

void PageManager::set_data_dir(const String& dir)
{

}