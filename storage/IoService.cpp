#include <stdio.h>
#include <stdlib.h>
#include "IoService.h"
#include "buffer.h"
#include "error.h"
#include "page.h"
#include "log.h"

#ifdef _WIN32
#define SET_FILE_POS(fpos, u64_v)	(fpos) = (u64_v);
#define GET_FILE_POS(u64_v, fpos)	(u64_v) = (fpos);
#else
#define SET_FILE_POS(fpos, u64_v)	(fpos.__pos) = (u64_v);
#define GET_FILE_POS(u64_v, fpos)	(u64_v) = (fpos.__pos);
#endif	//_WIN32
using namespace CatDB::Storage;

IoService::IoService()
	:file_handle(0)
{
	
}

IoService::~IoService()
{
	if (file_handle){
		fclose(file_handle);
	}
}

IoService_s IoService::make_io_service()
{
	return IoService_s(new IoService());
}

u32 IoService::open(const String & table_file)
{
	Log(LOG_INFO, "IoService", "open table %s", table_file.c_str());
	file_handle = fopen(table_file.c_str(), "rb+");
	if (file_handle){
		return SUCCESS;
	}else{
		file_handle = fopen(table_file.c_str(), "wb+");
		if (file_handle){
			fclose(file_handle);
			file_handle = fopen(table_file.c_str(), "rb+");
			return SUCCESS;
		}else{
			Log(LOG_ERR, "IoService", "open table file failed!");
			return TABLE_FILE_NOT_EXISTS;
		}
	}
}

bool CatDB::Storage::IoService::is_open() const
{
	return file_handle != 0;
}

u32 IoService::read_page(Page_s & page)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	u32 offset = page->page_offset();
	u32 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	
	u64 value = get_real_page_offset(offset);
	fpos_t real_offset;
	SET_FILE_POS(real_offset, value);
	if (fsetpos(file_handle, &real_offset) != 0) {
		Log(LOG_ERR, "IoService", "set offset error when read page!");
		return UNKNOWN_PAGE_OFFSET;
	}
	u32 ret = fread(buffer->buf, 1, size, file_handle);
	if (ret != size){
		Log(LOG_ERR, "IoService", "read page error:%u", ret);
		return BAD_PAGE_IN_FILE;
	}
	page->reset_page();
	Log(LOG_TRACE, "IoService", "read page %u, size %u", offset, size);
	return SUCCESS;
}

u32 IoService::write_page(const Page_s & page)
{
	return write_page(page.get());
}

u32 IoService::write_page(const Page * page)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	u32 offset = page->page_offset();
	u32 size = page->page_size();
	const Buffer_s& buffer = page->page_buffer();
	u64 value = get_real_page_offset(offset);
	fpos_t real_offset;
	SET_FILE_POS(real_offset, value);
	if (fsetpos(file_handle, &real_offset) != 0) {
		Log(LOG_ERR, "IoService", "set offset error when write page!");
		return UNKNOWN_PAGE_OFFSET;
	}
	u32 ret = fwrite(buffer->buf, 1, size, file_handle);
	if (ret != size){
		Log(LOG_ERR, "IoService", "write page error:%u", ret);
		return WRITE_PAGE_ERROR;
	}
	Log(LOG_TRACE, "IoService", "read page %u, size %u", offset, size);
	return SUCCESS;
}

u32 IoService::end_offset(u32& offset)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	fseek(file_handle, 0, SEEK_END);
	fpos_t pos;
	fgetpos(file_handle, &pos);
	u64 size = 0;
	GET_FILE_POS(size, pos);
	if (size >= PAGE_SIZE){
		offset = get_virtual_page_offset(size - PAGE_SIZE);
		return SUCCESS;
	}else{
		offset = 0;
		return EMPTY_TABLE_SPACE;
	}
}

void IoService::close()
{
	fclose(file_handle);
	file_handle = 0;
}

bool IoService::eof() const
{
	if (!file_handle) {
		return true;
	}
	return feof(file_handle) != 0;
}

u32 IoService::delete_file(const String& table_file)
{
	Log(LOG_TRACE, "IoService", "delete table file %s", table_file.c_str());
	if (remove(table_file.c_str()) == 0) {
		return SUCCESS;
	}else {
		return TABLE_FILE_NOT_EXISTS;
	}
}

u32 IoService::clear_file(const String& table_file)
{
	Log(LOG_TRACE, "IoService", "clear table file %s", table_file.c_str());
	file_handle = fopen(table_file.c_str(), "w");
	if (file_handle) {
		fclose(file_handle);
		file_handle = 0;
		return SUCCESS;
	}else {
		return TABLE_FILE_NOT_EXISTS;
	}
}

u64 IoService::get_file_size(const String & table_file)
{
	FILE* fp = fopen(table_file.c_str(), "rb+");
	if (!fp) {
		return 0;
	}
	else {
		fseek(fp, 0, SEEK_END);
		fpos_t pos;
		fgetpos(fp, &pos);
		fclose(fp);
		u64 size = 0;
		GET_FILE_POS(size, pos);
		return size;
	}
}

u32 IoService::make_dir(const String & dir)
{
	String cmd = "mkdir " + dir;
	int ret = system(cmd.c_str());
	return SUCCESS;
}

u32 IoService::remove_dir(const String & dir)
{
#ifdef _WIN32
	String cmd = "rmdir /s /q " + dir;
#else
	String cmd = "rm -rf " + dir;
#endif
	int ret = system(cmd.c_str());
	return SUCCESS;
}

u32 IoService::move_dir(const String& src_dir, const String& dst_dir)
{
#ifdef _WIN32
	String cmd = "move " + src_dir + " " + dst_dir;
#else
	String cmd = "mv " + src_dir + " " + dst_dir;
#endif
	int ret = system(cmd.c_str());
	return SUCCESS;
}
