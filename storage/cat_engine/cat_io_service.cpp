#include "cat_io_service.h"
#include "io_service.h"
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
	u64 value = get_real_page_offset(offset);
	CHECK(read_buffer((char*)buffer->buf, value, size));
	page->reset_page();
	LOG_TRACE("read page success", K(offset), K(size));
	return ret;
}

u32 CatIoService::write_page(const Page_s & page)
{
	return write_page(page.get());
}

u32 CatIoService::write_page(const Page * page)
{
	u32 ret = SUCCESS;
	u32 offset = page->page_offset();
	u64 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	u64 value = get_real_page_offset(offset);
	CHECK(write_buffer((const char*)buffer->buf, value, size));
	LOG_TRACE("write page succees", K(offset), K(size));
	return ret;
}

u32 CatIoService::end_offset(u64& offset)
{
	u32 ret = SUCCESS;
	IoService::end_offset(offset);
	if (offset >= PAGE_SIZE) {
		offset = get_virtual_page_offset(offset - PAGE_SIZE);
		ret = SUCCESS;
	} else {
		offset = 0;
		ret = EMPTY_TABLE_SPACE;
	}
	return ret;
}
