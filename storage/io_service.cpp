#include <stdio.h>
#include <stdlib.h>
#include "io_service.h"
#include "error.h"
#include "log.h"

#ifdef _WIN32
#define SET_FILE_POS(fpos, u64_v)	(fpos) = (u64_v);
#define GET_FILE_POS(u64_v, fpos)	(u64_v) = (fpos);
#else
	#ifdef __linux__
	#define SET_FILE_POS(fpos, u64_v)	(fpos.__pos) = (u64_v);
	#define GET_FILE_POS(u64_v, fpos)	(u64_v) = (fpos.__pos);
	#else
	#define SET_FILE_POS(fpos, u64_v)	(fpos) = (u64_v);
	#define GET_FILE_POS(u64_v, fpos)	(u64_v) = (fpos);
	#endif //__linux__
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
	LOG_TRACE("open table", K(table_file));
	file_handle = fopen(table_file.c_str(), "rb+");
	if (file_handle) {
		return SUCCESS;
	} else {
		file_handle = fopen(table_file.c_str(), "wb+");
		if (file_handle) {
			fclose(file_handle);
			file_handle = fopen(table_file.c_str(), "rb+");
			return SUCCESS;
		} else {
			LOG_ERR("open table file failed!");
			return TABLE_FILE_NOT_EXISTS;
		}
	}
}

bool CatDB::Storage::IoService::is_open() const
{
	return file_handle != 0;
}

u32 IoService::read_buffer(char* buffer, u64 offset, u64 &size)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	fpos_t real_offset;
	SET_FILE_POS(real_offset, offset);
	if (fsetpos(file_handle, &real_offset) != 0) {
		LOG_ERR("set offset error when read buffer!");
		return UNKNOWN_PAGE_OFFSET;
	}
	size = fread(buffer, 1, size, file_handle);
	LOG_TRACE("read buffer success", K(offset), K(size));
	return SUCCESS;
}

u32 IoService::write_buffer(const char* buffer, u64 offset, u64 &size)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	fpos_t real_offset;
	SET_FILE_POS(real_offset, offset);
	if (fsetpos(file_handle, &real_offset) != 0) {
		LOG_ERR("set offset error when write buffer!");
		return UNKNOWN_PAGE_OFFSET;
	}
	size = fwrite(buffer, 1, size, file_handle);
	LOG_TRACE("write buffer succees", K(offset), K(size));
	return SUCCESS;
}

u32 IoService::read_buffer(char* buffer, u64 &size)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	size = fread(buffer, 1, size, file_handle);
	LOG_TRACE("read buffer success", K(size));
	return SUCCESS;
}

u32 IoService::write_buffer(const char* buffer, u64 &size)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	fseek(file_handle, 0, SEEK_END);
	size = fwrite(buffer, 1, size, file_handle);
	LOG_TRACE("write buffer succees", K(size));
	return SUCCESS;
}

u32 IoService::end_offset(u64& offset)
{
	if (!file_handle) {
		return TABLE_FILE_NOT_EXISTS;
	}
	fseek(file_handle, 0, SEEK_END);
	fpos_t pos;
	fgetpos(file_handle, &pos);
	GET_FILE_POS(offset, pos);
    return SUCCESS;
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
	LOG_TRACE("delete table file", K(table_file));
	if (remove(table_file.c_str()) == 0) {
		return SUCCESS;
	} else {
		return TABLE_FILE_NOT_EXISTS;
	}
}

u32 IoService::clear_file(const String& table_file)
{
	LOG_TRACE("clear table file", K(table_file));
	FILE *file_handle = fopen(table_file.c_str(), "w");
	if (file_handle) {
		fclose(file_handle);
		file_handle = 0;
		return SUCCESS;
	} else {
		return TABLE_FILE_NOT_EXISTS;
	}
}

u64 IoService::get_file_size(const String & table_file)
{
	FILE* fp = fopen(table_file.c_str(), "rb+");
	if (!fp) {
		return 0;
	} else {
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
	String cmd = "mkdir -p " + dir;
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
