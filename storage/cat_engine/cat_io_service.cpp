#include "cat_io_service.h"
#include "io_service.h"
#include "object.h"
#include "buffer.h"
#include "error.h"
#include "page.h"
#include "log.h"

using namespace CatDB::Storage;

CatIoService::CatIoService()
{
	
}

CatIoService::~CatIoService()
{

}

CatIoService_s CatIoService::make_cat_io_service()
{
	return CatIoService_s(new CatIoService());
}

u32 CatIoService::read_page(Page_s & page)
{
	u32 ret = SUCCESS;
	u32 offset = page->page_offset();
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	u64 real_offset = page->get_real_page_offset(offset);
	CHECK(read_buffer((char*)buffer->buf, real_offset, size));
	if (0 == size) {
		ret = END_OF_TABLE_SPACE;
		LOG_TRACE("read at end", K(offset), K(real_offset));
	} else if (size != page->page_size()) {
		ret = BAD_PAGE_IN_FILE;
		LOG_ERR("failed read page", K(ret));
	} else {
		LOG_TRACE("read page success", K(offset), K(size));
	}
	return ret;
}

u32 CatIoService::read_next_page(Page_s & page)
{
	u32 ret = SUCCESS;
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	CHECK(read_buffer((char*)buffer->buf, size));
	if (0 == size) {
		ret = END_OF_TABLE_SPACE;
		LOG_TRACE("read at end");
	} else if (size != page->page_size()) {
		ret = BAD_PAGE_IN_FILE;
		LOG_ERR("failed read page", K(ret));
	} else {
		LOG_TRACE("read next page success", K(size));
	}
	return ret;
}

u32 CatIoService::read_first_page(Page_s & page)
{
	u32 ret = SUCCESS;
	u32 offset = 0;
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	u64 real_offset = page->get_real_page_offset(offset);
	CHECK(read_buffer((char*)buffer->buf, real_offset, size));
	if (0 == size) {
		ret = END_OF_TABLE_SPACE;
		LOG_TRACE("read at end", K(offset), K(real_offset));
	} else if (size != page->page_size()) {
		ret = BAD_PAGE_IN_FILE;
		LOG_ERR("failed read page", K(ret));
	} else {
		LOG_TRACE("read page success", K(offset), K(size));
	}
	return ret;
}

u32 CatIoService::read_last_page(Page_s & page)
{
	u32 ret = SUCCESS;
	u64 offset = 0;
	ret = end_offset(offset);
	if (ret != SUCCESS) {
		return ret;
	}
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	CHECK(read_buffer((char*)buffer->buf, offset, size));
	if (0 == size) {
		ret = END_OF_TABLE_SPACE;
		LOG_TRACE("read at end", K(offset));
	} else if (size != page->page_size()) {
		ret = BAD_PAGE_IN_FILE;
		LOG_ERR("failed read page", K(ret));
	} else {
		LOG_TRACE("read page success", K(offset), K(size));
	}
	return ret;
}

u32 CatIoService::write_page(const Page_s & page)
{
	u32 ret = SUCCESS;
	u32 offset = page->page_offset();
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	u64 real_offset = page->get_real_page_offset(offset);
	CHECK(write_buffer((const char*)buffer->buf, real_offset, size));
	if (size != page->page_size()) {
		ret = WRITE_PAGE_ERROR;
		LOG_ERR("failed to write page", K(page), K(ret));
	} else {
		LOG_TRACE("write page succees", K(offset), K(size));
	}
	return ret;
}

u32 CatIoService::write_next_page(const Page_s & page)
{
	u32 ret = SUCCESS;
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	CHECK(write_buffer((const char*)buffer->buf, size));
	if (size != page->page_size()) {
		ret = WRITE_PAGE_ERROR;
		LOG_ERR("failed to write page", K(page), K(ret));
	} else {
		LOG_TRACE("write next page succees", K(size));
	}
	return ret;
}

u32 CatIoService::end_offset(u64& offset)
{
	u32 ret = SUCCESS;
	IoService::end_offset(offset);
	if (offset >= PAGE_SIZE) {
		offset = offset - PAGE_SIZE;
		ret = SUCCESS;
	} else {
		offset = 0;
		ret = EMPTY_TABLE_SPACE;
	}
	return ret;
}
