#include "page_dump.h"
#include "cat_io_service.h"
#include "object.h"
#include "page.h"
#include "error.h"

using namespace CatDB::Storage;

PageDump::PageDump(const String &data_file)
{
    io_service = CatIoService::make_cat_io_service();
    io_service->open(data_file);
    page = Page::make_page();
}

bool PageDump::dump_page(String &page_info)
{
    u32 ret = SUCCESS;
    page_info.clear();
    ret = io_service->read_next_page(page);
    if (FAIL(ret)) {
        return false;
    }

    return true;
}