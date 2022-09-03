#include "cat_table_space.h"
#include "cat_io_service.h"
#include "object.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Storage;
using namespace CatDB::Common;

LRUManager::LRUManager()
{
	head.pre = &tail;
	head.next = &tail;
	tail.pre = &head;
	tail.next = &head;
}

LRUManager::~LRUManager()
{
	Node *node = head.next;
	while(node && node != &tail) {
		if (node->page->is_page_dirty()) {
			(io->write_page(node->page));
			node->page->write_page_to_disk();
		}
		node = node->next;
	}
}

bool LRUManager::find_page(u32 offset, Page_s &page)
{
	bool find = false;
	Node *node = head.next;
	while(node && node != &tail) {
		if (node->page->page_offset() == offset) {
			find = true;
			page = node->page;
			break;
		}
		node = node->next;
	}
	if (find) {
		visit_node(node);
	}
	return find;
}

bool LRUManager::add_page(Page_s &new_page, Page_s &old_page)
{
	if (head.count >= MAX_SIZE) {
		Node *node = tail.pre;
		old_page = node->page;
		del_node(node);
		node->page = new_page;
		add_node_before(&tail, node);
		visit_node(node);
		return true;
	} else {
		Node *node = new Node;
		node->page = new_page;
		add_node_before(&tail, node);
		visit_node(node);
		return false;
	}
}

void LRUManager::visit_node(Node* node)
{
	if (node == &head || node == &tail) {
		return;
	}
	Node *last_node = &head;
	Node *first_node = node;
	++node->count;
	while (node->pre != &head) {
		if (node->count >= node->pre->count) {
			node = node->pre;
		} else {
			last_node = node->pre; 
			break;
		}
	}
	del_node(first_node);
	add_node_after(last_node, first_node);
}

void LRUManager::del_node(Node* node)
{
	if (node == &head || node == &tail) {
		return;
	}
	Node *parent = node->pre;
	Node *child = node->next;
	parent->next = child;
	child->pre = parent;
}

void LRUManager::add_node_after(Node *head, Node *node)
{
	Node *child = head->next;
	node->pre = head;
	node->next = child;
	head->next = node;
	child->pre = node;
}

void LRUManager::add_node_before(Node *tail, Node *node)
{
	Node *parent = tail->pre;
	node->pre = parent;
	node->next = tail;
	tail->pre = node;
	parent->next = node;
}

CatTableSpace::CatTableSpace()
	:is_empty_table_space(false),
	read_only(true)
{
	io = CatIoService::make_cat_io_service();
	page_manager.io = io;
}

CatTableSpace::~CatTableSpace()
{
}

TableSpace_s CatTableSpace::make_table_space(const String& table_name, 
											 const String & database, 
											 const Vector<String> &args,
											 double sample_value,
											 bool read_only)
{
	CatTableSpace* table_space = new CatTableSpace;
	table_space->database = database;
	table_space->table_name = table_name;
	table_space->read_only = read_only;
	table_space->sample_value = sample_value;
	return TableSpace_s(table_space);
}

u32 CatTableSpace::open()
{
	LOG_TRACE("open table space", K(database), K(table_name));
	String path = table_path(database, table_name);
	u32 ret = SUCCESS;
	CHECK(io->open(path));
	cur_page = Page::make_page(0, access_desc);
	if (read_only) {
		ret = io->read_first_page(cur_page);
		if (ret == END_OF_TABLE_SPACE) {
			is_empty_table_space = true;
			ret = SUCCESS;
		}
	} else {
		ret = io->read_last_page(cur_page);
		if (ret == EMPTY_TABLE_SPACE) {
			ret = SUCCESS;
		}
	}
	CHECK(cur_page->open());
	return ret;
}

u32 CatTableSpace::get_next_row(Row_s & row)
{
	if (is_empty_table_space) {
		return END_OF_TABLE_SPACE;
	}
	u32 ret = SUCCESS;
	MY_ASSERT(cur_page);
	while (NO_MORE_ROWS == (ret=cur_page->get_next_row(row))) {
		LOG_TRACE("page have read end, load next page", K(cur_page));
		//读取下一页
		ret = io->read_next_page(cur_page);
		if (ret == END_OF_TABLE_SPACE) {
			return ret;
		} else if (ret != SUCCESS) {
			LOG_ERR("get page failed", K(ret));
			return ret;
		} else {
			CHECK(cur_page->open());
			cur_page->set_sample_value(sample_value);
		}
	}
	//TODO filter 放这还是下压到page层面？
	return ret;
}

u32 CatTableSpace::reset()
{
	u32 ret = SUCCESS;
	if (read_only) {
		ret = io->read_first_page(cur_page);
		if (ret == END_OF_TABLE_SPACE) {
			is_empty_table_space = true;
			cur_page->clear_page(0);
			ret = SUCCESS;
		} else {
			CHECK(cur_page->open());
		}
	}
	return ret;
}

u32 CatTableSpace::close()
{
	u32 ret = SUCCESS;
	LOG_TRACE("close table space", K(table_name));
	CHECK(write_page_to_disk(cur_page));
	return SUCCESS;
}

u32 CatTableSpace::insert_row(const Row_s & row)
{
	u32 ret = SUCCESS;
	MY_ASSERT(cur_page);
	ret = cur_page->insert_row(row);
	//当前页能够存放下行
	if (NO_MORE_PAGE_FREE_SPACE == ret) {
		//创建新的页存放
		LOG_TRACE("page have no free space to insert row", K(cur_page));
		CHECK(write_page_to_disk(cur_page));
		cur_page->clear_page(cur_page->next_page_offset());
		CHECK(cur_page->open());
		CHECK(cur_page->insert_row(row));
	}
	return ret;
}

u32 CatTableSpace::get_row(u32 row_id, Row_s & row)
{
	u32 ret = SUCCESS;
	Page_s page;
	CHECK(get_page_from_row_id(row_id, page));
	CHECK(page->get_row(row_id, row));
	return ret;
}

u32 CatTableSpace::delete_row(u32 row_id)
{
	u32 ret = SUCCESS;
	Page_s page;
	CHECK(get_page_from_row_id(row_id, page));
	CHECK(page->delete_row(row_id));
	return ret;
}

u32 CatTableSpace::delete_all_row()
{
	u32 ret = SUCCESS;
	String path = table_path(database, table_name);
	CHECK(io->clear_file(path));
	return ret;
}

u32 CatTableSpace::update_row(u32 row_id, 
							  const Row_s& update_row, 
							  const Row_s& access_row)
{
	u32 ret = SUCCESS;
	Object_s cell;
	u32 column_id = 0;
	ColumnDesc col_desc;
	Row_s old_row = access_row;
	CHECK(get_row(row_id, old_row));
	for (u32 i = 0; i < update_desc.get_column_num(); ++i) {
		CHECK(update_row->get_cell(i, cell));
		CHECK(update_desc.get_column_desc(i, col_desc));
		column_id = col_desc.get_cid();
		CHECK(old_row->set_cell(column_id, cell));
	}
	CHECK(delete_row(row_id));
	CHECK(insert_row(old_row));
	return ret;
}

u32 CatTableSpace::get_page_from_row_id(u32 row_id, Page_s& page)
{
	u32 ret = SUCCESS;
	u32 page_offset = Page::get_page_offset_from_row_id(row_id);
	if (page_offset == cur_page->page_offset()) {
		page = cur_page;
	} else if (!page_manager.find_page(page_offset, page)) {
		page = Page::make_page(page_offset, access_desc);
		CHECK(io->read_page(page));
		CHECK(page->open());
		Page_s rm_page;
		if (page_manager.add_page(page, rm_page)) {
			write_page_to_disk(rm_page);
		}
	}
	return ret;
}

u32 CatTableSpace::write_page_to_disk(const Page_s& page)
{
	u32 ret = SUCCESS;
	if (page) {
		if (page->is_page_dirty()) {
			CHECK(io->write_page(page));
			page->write_page_to_disk();
		}
	}
	return ret;
}
