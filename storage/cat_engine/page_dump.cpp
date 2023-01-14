#include "page_dump.h"
#include "cat_io_service.h"
#include "object.h"
#include "page.h"
#include "error.h"
#include <sstream>
#include <iostream>

using namespace CatDB::Storage;

PageDump::PageDump(const String &data_file)
{
    io_service = CatIoService::make_cat_io_service();
    u32 ret = io_service->open(data_file);
    page = Page::make_page();
    index = 0;
}

void PageDump::dump_header(Page_s &page, std::stringstream &str_stream)
{
    str_stream << "page_checksum:" << page->page_header_->page_checksum << std::endl;
	str_stream << "page_offset:" << page->page_header_->page_offset << std::endl;
	str_stream << "page_pre:" << page->page_header_->page_pre << std::endl;
	str_stream << "page_next:" << page->page_header_->page_next << std::endl;
	str_stream << "row_count:" << page->page_header_->row_count << std::endl;
	str_stream << "free_offset:" << page->page_header_->free_offset << std::endl;
	str_stream << "free_size:" << page->page_header_->free_size << std::endl;
}

void PageDump::hex_string(const char* data, u32 size, String& str)
{
    static char map[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    for (u32 i = 0; i < size; ++i) {
        str += map[(data[i] >> 4) & 0x0f];
        str += map[data[i] & 0x0f];
    }
}

void PageDump::dump_record(Page_s &page, int idx, int row_size, std::stringstream &str_stream)
{
    RowInfo *info = &page->row_info_[idx];
    if (info->is_row_deleted()) {
        str_stream << "row " << page->page_header_->row_count-idx << ":" << "delete" << std::endl;
        return;
    }
    RawRecord* record = RawRecord::make_raw_record(page->records_space_ + info->offset);
    str_stream << "row " << page->page_header_->row_count-idx << " size:" << row_size << std::endl;
    for (u32 i = 0; i < record->column_count-1; ++i) {
        u32 size = record->get_offset(i+1) - record->get_offset(i);
        str_stream << "\t" << "column\t" << i << ",\tlength\t" << size << ",\tvalue:\t";
        char* data = reinterpret_cast<char*>(record) + record->get_offset(i);
        if (record->is_null(i)) {
            str_stream << "NULL" << std::endl;
        } else {
            String str;
            hex_string(data, size , str);
            str_stream << str << std::endl;
        }
    }
}

bool PageDump::dump_page(String &page_info, u32 page_id)
{
    u32 ret = SUCCESS;
    std::stringstream str_stream;
    page_info.clear();
    page->page_header_->page_offset = page_id;
    ret = io_service->read_page(page);
    if (FAIL(ret)) {
        return false;
    }
    page->open();
    dump_header(page, str_stream);
    for (u32 i = 0; i < page->page_header_->row_count; ++i) {
        int idx = page->get_idx_from_row_id(i);
        RowInfo *info = &page->row_info_[idx];
        int row_size = 0;
        if (i < page->page_header_->row_count - 1) {
            int idx_next = page->get_idx_from_row_id(i+1);
            RowInfo *info_next = &page->row_info_[idx_next];
            row_size = info_next->row_offset() - info->row_offset();
        } else {
            row_size = page->page_header_->free_offset - info->row_offset();
        }
        dump_record(page, idx, row_size, str_stream);
    }
    page->close();
    page_info = str_stream.str();
    return true;
}