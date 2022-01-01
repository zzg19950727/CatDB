#include "cat_table_space.h"
#include "cat_io_service.h"
#include "error.h"
#include "page.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Storage;
using namespace CatDB::Common;

LRUManger::LRUManger()
{
	head.pre = &tail;
	head.next = &tail;
	tail.pre = &head;
	tail.next = &head;
}

bool LRUManger::find_page(u32 offset, Page_s &page)
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

bool LRUManger::add_page(Page_s &new_page, Page_s &old_page)
{
	if (head.count >= MAX_SIZE) {
		Node *node = tail.pre;
		old_page = node->page;
		del_node(node);
		delete node;
		node = new Node;
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

void LRUManger::visit_node(Node* node)
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

void LRUManger::del_node(Node* node)
{
	if (node == &head || node == &tail) {
		return;
	}
	Node *parent = node->pre;
	Node *child = node->next;
	parent->next = child;
	child->pre = parent;
}

void LRUManger::add_node_after(Node *head, Node *node)
{
	Node *child = head->next;
	node->pre = head;
	node->next = child;
	head->next = node;
	child->pre = node;
}

void LRUManger::add_node_before(Node *tail, Node *node)
{
	Node *parent = tail->pre;
	node->pre = parent;
	node->next = tail;
	tail->pre = node;
	parent->next = node;
}

CatTableSpace::CatTableSpace()
	:page_skip_size(0),
	is_empty_table_space(false),
	read_only(true)
{
	io = CatIoService::make_cat_io_service();
	cur_page = Page::make_page(0);
}

CatTableSpace::~CatTableSpace()
{
}

TableSpace_s CatTableSpace::make_table_space(const String& table_name, 
											 const String & database, 
											 const Vector<String> &args,
											 double sample_size,
											 bool read_only)
{
	CatTableSpace* table_space = new CatTableSpace;
	table_space->database = database;
	table_space->table_name = table_name;
	table_space->read_only = read_only;
	if (sample_size > 0.99) {
		table_space->page_skip_size = 0;
	} else {
		table_space->page_skip_size = 1.0 / sample_size;
	}
	return TableSpace_s(table_space);
}

u32 CatTableSpace::open()
{
	LOG_TRACE("open table space", K(database), K(table_name));
	String path = table_path(database, table_name);
	u32 ret = SUCCESS;
	CHECK(io->open(path));
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
	while (!cur_page->have_row()) {
		LOG_TRACE("page have read end, load next page", K(cur_page));
		//读取下一页
		if (0 == page_skip_size) {
			ret = io->read_next_page(cur_page);
		} else {
			u32 next_offset = cur_page->next_page_offset() + page_skip_size;
			cur_page->clear_page(next_offset);
			ret = io->read_page(cur_page);
		}
		if (ret == END_OF_TABLE_SPACE) {
			return ret;
		} else if (ret != SUCCESS) {
			LOG_ERR("get page failed", err_string(ret));
			return ret;
		} else {
			CHECK(cur_page->open());
		}
	}
	//TODO filter 放这还是下压到page层面？
	return cur_page->get_next_row(row);
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
	//当前页能够存放下行
	if (cur_page->have_free_space_insert(row)) {
		CHECK(cur_page->insert_row(row));
	} else {//创建新的页存放
		LOG_TRACE("page have no free space to insert row", K(cur_page));
		CHECK(write_page_to_disk(cur_page));
		cur_page->clear_page(cur_page->next_page_offset());
		CHECK(cur_page->open());
		CHECK(cur_page->insert_row(row));
	}
	return ret;
}

u32 CatTableSpace::update_row(const Row_s & row)
{
	u32 ret = SUCCESS;
	u32 row_id = row->get_row_id();
	Page_s page;
	CHECK(get_page_from_row_id(row_id, page));
	Row_s old_row = row;
	ret = page->update_row(row_id, old_row);
	if (ret == ROW_DATA_TOO_LONG) {
		CHECK(page->delete_row(row_id));
		CHECK(insert_row(old_row));
	}
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

u32 CatTableSpace::get_page_from_row_id(u32 row_id, Page_s& page)
{
	u32 ret = SUCCESS;
	u32 page_offset = Page::get_page_offset_from_row_id(row_id);
	if (page_offset == cur_page->page_offset()) {
		page = cur_page;
	} else if (!page_manager.find_page(page_offset, page)) {
		page = Page::make_page(page_offset);
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
