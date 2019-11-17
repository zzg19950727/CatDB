#include "IoService.h"
#include "storage.h"
#include "buffer.h"
#include "error.h"
#include "page.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Storage;

CatEngine::CatEngine()
	:data_dir("D:\\CatDB\\build\\data\\"),
	max_buffer_blocks(2553)
{
	init_cache_manager();
}

CatEngine::~CatEngine()
{
}

CatEngine_s CatEngine::make_cat_engine()
{
	static CatEngine_s manager(new CatEngine());
	return manager;
}

void CatEngine::set_data_dir(const String & dir)
{
	data_dir = dir;
}

String CatEngine::get_data_dir() const
{
	return data_dir;
}

void CatEngine::set_max_blocks(u32 count)
{
	max_buffer_blocks = count;
}

u32 CatEngine::get_max_blocks() const
{
	return max_buffer_blocks;
}

u32 CatEngine::end_page_offset(const String & database, const String & table, u32 & offset)
{
	String path = get_table_path(database, table);
	IoService_s io;
	u32 ret = open_table(path, io);
	if (ret == SUCCESS) {
		ret = io->end_offset(offset);
	}
	return ret;
}

u32 CatEngine::delete_all_row(const String & database, const String & table)
{
	String path = get_table_path(database, table);
	IoService_s io;
	u32 ret = open_table(path, io);
	if (ret == SUCCESS) {
		ret = io->clear_file(path);
	}
	return ret;
}

u64 CatEngine::get_table_size(const String & database, const String & table)
{
	String path = get_table_path(database, table);
	IoService_s io;
	u32 ret = open_table(path, io);
	if (ret == SUCCESS) {
		return io->get_file_size(path);
	}
	else {
		return 0;
	}
}

u32 CatEngine::delete_table(const String & database, const String & table)
{
	String table_path = get_table_path(database, table);
	IoService_s io;
	u32 ret = open_table(table_path, io);
	if (ret == SUCCESS) {
		ret = io->delete_file(table_path);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "CatEngine", "delete table %s error:%s", table_name.c_str(), err_string(ret));
			return ret;
		}
		else {
			return SUCCESS;
		}
	}
	return ret;
}

u32 CatEngine::create_table(const String & database, const String & table)
{
	String table_path = get_table_path(database, table);
	IoService_s io;
	u32 ret = open_table(table_path, io);
	if (ret == SUCCESS) {
		ret = io->open(table_path);
		if (ret != SUCCESS) {
			Log(LOG_ERR, "CatEngine", "create table %s error:%s", table_name.c_str(), err_string(ret));
			return ret;
		}
		else {
			io->close();
			return SUCCESS;
		}
	}
	return ret;
}

u32 CatEngine::delete_database(const String & database)
{
	String path = get_table_path(database, "");
	return IoService::remove_dir(path);
}

u32 CatEngine::create_database(const String & database)
{
	String path = get_table_path(database, "");
	return IoService::make_dir(path);
}

u32 CatEngine::create_new_page_buffer(
	const String & database, 
	const String & table, 
	u32 page_offset, 
	Buffer_s & buffer)
{
	String table_path = get_table_path(database, table);
	auto iter = io_services.find(table_path);
	IoService_s io;
	if (iter == io_services.end()) {
		u32 ret = open_table(table_path, io);
		if (ret != SUCCESS) {
			return ret;
		}
		io_services[table_path] = io;
	}
	else {
		io = io_services[table_path];
	}
	buffer = Buffer::make_buffer(PAGE_SIZE);
	assert(buffer);
	buffer->is_dirty = true;
	Node node;
	node.ref = 1;
	node.buf = buffer;
	node.key = table_path;
	node.offset = page_offset;
	using_node_map[table_path][page_offset] = node;
	return SUCCESS;
}

u32 CatEngine::get_page_buffer(const String & database, 
	const String & table, 
	u32 page_offset, 
	Buffer_s & buffer)
{
	String path = get_table_path(database, table);
	//查看引用缓冲池内是否有表的缓存数据
	auto iter_using_table = using_node_map.find(path);
	if (iter_using_table != using_node_map.end()) {
		//查询当前表的缓冲区内是否是指定page的缓冲数据
		auto iter_using_page = iter_using_table->second.find(page_offset);
		if (iter_using_page != iter_using_table->second.end()) {
			buffer = iter_using_page->second.buf;
			++iter_using_page->second.ref;
			return SUCCESS;
		}
	}
	//查询空闲缓存池内是否有表的缓存数据
	auto iter_free_table = free_node_map.find(path);
	if (iter_free_table != free_node_map.end()) {
		//查询当前表的缓冲区内是否是指定page的缓冲数据
		auto iter_free_page = iter_free_table->second.find(page_offset);
		if (iter_free_page != iter_free_table->second.end()) {
			Node node = *(iter_free_page->second);
			free_node_list.erase(iter_free_page->second);
			iter_free_table->second.erase(iter_free_page);
			++node.ref;
			buffer = node.buf;

			using_node_map[path][page_offset] = node;
			return SUCCESS;
		}
	}
	//缓冲池内没有则从磁盘上读取
	return read_page_buffer(path, page_offset, buffer);
}

u32 CatEngine::free_page_buffer(const String & database, 
	const String & table, 
	u32 page_offset)
{
	String path = get_table_path(database, table);
	//查看引用缓冲池内是否有表的缓存数据
	auto iter_using_table = using_node_map.find(path);
	if (iter_using_table != using_node_map.end()) {
		//查询当前表的缓冲区内是否是指定page的缓冲数据
		auto iter_using_page = iter_using_table->second.find(page_offset);
		if (iter_using_page != iter_using_table->second.end()) {
			--iter_using_page->second.ref;
			if (iter_using_page->second.ref == 0) {
				if (iter_using_page->second.buf->is_dirty) {
					u32 ret = io_services[path]->write_page(page_offset, iter_using_page->second.buf);
					if (ret == SUCCESS) {
						iter_using_page->second.buf->is_dirty = false;
					}
				}
				if (cache_size() < max_buffer_blocks) {
					free_node_list.push_front(iter_using_page->second);
					free_node_map[path][page_offset] = free_node_list.begin();
					iter_using_table->second.erase(iter_using_page);
				}
				else {
					iter_using_table->second.erase(iter_using_page);
				}
			}
			return SUCCESS;
		}
	}
	Log(LOG_ERR, "CatEngine", "free page error");
	return UNKNOWN_PAGE_BUFFER;
}

u32 CatEngine::free_page_memory(u32 count)
{
	while (count > 0 && !free_node_list.empty()) {
		auto iter = free_node_list.end();
		--iter;
		auto iter_free_table = free_node_map.find(iter->key);
		if (iter_free_table != free_node_map.end()) {
			auto iter_free_page = iter_free_table->second.find(iter->offset);
			if (iter_free_page != iter_free_table->second.end()) {
				iter_free_table->second.erase(iter_free_page);
			}
		}
		free_node_list.erase(iter);
		--count;
	}
	return SUCCESS;
}

void CatEngine::init_cache_manager()
{
}

void CatEngine::timer_for_free_cache()
{
}

void CatEngine::timer_for_preread_cache()
{
}

String CatEngine::get_table_path(const String & database, const String & table) const
{
	return data_dir + "\\" + database + "\\" + table;
}

u32 CatEngine::read_page_buffer(const String& table_path,
	u32 page_offset, 
	Buffer_s & buffer)
{
	auto iter = io_services.find(table_path);
	IoService_s io;
	if (iter == io_services.end()) {
		u32 ret = open_table(table_path, io);
		if (ret != SUCCESS) {
			return ret;
		}
		io_services[table_path] = io;
	}
	else {
		io = io_services[table_path];
	}
	buffer = Buffer::make_buffer(PAGE_SIZE);
	assert(buffer);
	u32 ret = io->read_page(page_offset, buffer);
	if (ret == SUCCESS) {
		Node node;
		node.ref = 1;
		node.buf = buffer;
		node.key = table_path;
		node.offset = page_offset;
		using_node_map[table_path][page_offset] = node;

		if (cache_size() >= max_buffer_blocks) {
			free_page_memory(100);
		}
	}
	return ret;
}

u32 CatEngine::open_table(const String& table_path,
	IoService_s & file_handle)
{
	file_handle = IoService::make_io_service();
	u32 ret = file_handle->open(table_path);
	if (ret == SUCCESS) {
		io_services[table_path] = file_handle;
	}
	return ret;
}

u32 CatEngine::cache_size() const
{
	u32 count = 0;
	for (auto iter = using_node_map.cbegin(); iter != using_node_map.cend(); ++iter) {
		count += iter->second.size();
	}
	count += free_node_list.size();
	
	return count;
}
