#include "table_space.h"
#include "IoService.h"
#include "storage.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Storage;
using namespace CatDB::Common;
extern StorageEngine engine;

TableSpace::TableSpace()
	:cur_page_offset(0)
{
}

TableSpace::~TableSpace()
{
}

TableSpace_s TableSpace::make_table_space(const String& table_name, const String & database)
{
	TableSpace* table_space = new TableSpace;
	table_space->page_manager = engine.get_page_manager(database, table_name);
	return TableSpace_s(table_space);
}

u32 TableSpace::open()
{
	Log(LOG_TRACE, "TableSpace", "open table space");
	cur_page_offset = 0;
	page_manager->reset_all_page();
	return SUCCESS;
}

u32 TableSpace::get_next_row(Row_s & row)
{
	Page_s page;
	u32 ret = page_manager->get_page_from_offset(cur_page_offset, page);
	if (ret == END_OF_TABLE_SPACE) {
		return ret;
	}
	else if (ret != SUCCESS){
		Log(LOG_ERR, "TableSpace", "can not get page %u, error code:%s", cur_page_offset, err_string(ret));
		return ret;
	}
	
	//当前页扫描完成
	while (!page->have_row()){
		Page_s last_page;
		page_manager->get_last_page(last_page);
		//最后一页
		if (cur_page_offset == last_page->page_offset()){
			Log(LOG_TRACE, "TableSpace", "read end of table space");
			return END_OF_TABLE_SPACE;
		}
		Log(LOG_TRACE, "TableSpace", "page %u have read end, load next page", cur_page_offset);
		//读取下一页
		cur_page_offset = page->next_page_offset();
		//下一页不在内存中
		ret = page_manager->get_page_from_offset(cur_page_offset, page);
		if (ret == END_OF_TABLE_SPACE) {
			return ret;
		}
		else if (ret != SUCCESS){
			Log(LOG_ERR, "TableSpace", "get page %u failed,%s", cur_page_offset, err_string(ret));
			return ret;
		}
	}
	
	//TODO filter 放这还是下压到page层面？
	return page->get_next_row(row);
}

u32 TableSpace::reset()
{
	cur_page_offset = 0;
	page_manager->reset_all_page();
	return SUCCESS;
}

u32 TableSpace::close()
{
	Log(LOG_TRACE, "TableSpace", "close table space %s", table_name.c_str());
	return SUCCESS;
}

u32 TableSpace::insert_row(const Row_s & row)
{
	u32 row_id;
	Page_s page;
	page_manager->get_last_page(page);
	//当前页能够存放下行
	if (page->have_free_space_insert(row)){
		return page->insert_row(row_id, row);
	}else{//创建新的页存放
		Log(LOG_TRACE, "TableSpace", "page %u have no free space to insert row", page->page_offset());
		u32 offset = page->next_page_offset();
		Page_s page;
		page_manager->create_page(offset, page);
		return page->insert_row(row_id, row);
	}
}

u32 TableSpace::update_row(const Row_s & row)
{
	u32 row_id = row->get_row_id();
	Page_s page;
	u32 ret = page_manager->get_page_from_row_id(row_id, page);
	if (ret == SUCCESS){
		return page->update_row(row_id, row);
	}else{
		Log(LOG_ERR, "TableSpace", "can not get row %u `s page,%s", row_id, err_string(ret));
		return ret;
	}
}

u32 TableSpace::delete_row(u32 row_id)
{
	Page_s page;
	u32 ret = page_manager->get_page_from_row_id(row_id, page);
	if (ret == SUCCESS){
		return page->delete_row(row_id);
	}else{
		Log(LOG_ERR, "TableSpace", "can not delete row %u,%s", row_id, err_string(ret));
		return ret;
	}
}

u32 TableSpace::delete_all_row()
{
	return page_manager->delete_all_row();
}

u32 TableSpace::delete_table(const String& database, const String& table_name)
{
	IoService_s io = IoService::make_io_service();
	u32 ret = io->delete_file(table_name);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "TableSpace", "delete table %s error:%s", table_name.c_str(), err_string(ret));
		return ret;
	}else {
		return SUCCESS;
	}
}

u32 TableSpace::create_table(const String& database, const String& table_name)
{
	IoService_s io = IoService::make_io_service();
	u32 ret = io->open(table_name);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "TableSpace", "create table %s error:%s", table_name.c_str(), err_string(ret));
		return ret;
	}else {
		io->close();
		return SUCCESS;
	}
}