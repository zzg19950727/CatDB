#include <iostream>
#include <cassert>
#include <chrono>
#include "table_scan.h"
#include "hash_distinct.h"
#include "hash_group.h"
#include "hash_set.h"
#include "scalar_group.h"
#include "table_space.h"
#include "expression.h"
#include "IoService.h"
#include "hash_join.h"
#include "filter.h"
#include "buffer.h"
#include "object.h"
#include "limit.h"
#include "error.h"
#include "sort.h"
#include "page.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Storage;
using namespace CatDB::Common;
using namespace CatDB::Sql;

class Timer
{
public:
	Timer()
	{
		auto now = std::chrono::steady_clock::now();
		start = now.time_since_epoch().count();
	}
	~Timer()
	{
		auto now = std::chrono::steady_clock::now();
		long long end = now.time_since_epoch().count();
		std::cout << "cost " << (end - start) / 1000000000 << "s" << std::endl;
	}
	void when()
	{
		auto now = std::chrono::steady_clock::now();
		long long end = now.time_since_epoch().count();
		std::cout << "cost " << (end - start) / 1000000000 << "s" << std::endl;
	}
private:
	long long start;
};

void object_test()
{
	u8 buf[18];
	for (int i = 0; i < 10; ++i)
		buf[i] = 'a';
	RawData& data = *RawData::make_row_data(buf);
	data.type = T_VARCHAR;
	data.length = 10;
	auto object = Object::make_object(data);
	assert(object->get_type() == T_VARCHAR);
	assert(object->is_fixed_length() == false);
	assert(object->is_null() == false);
	assert(object->width() == 10);

	Buffer_s buffer = Buffer::make_buffer(object->width() + sizeof(RawData));
	u8* tmp = buffer->buf;
	object->serialization(tmp);

	RawData* out = RawData::make_row_data(buffer->buf);
	assert(out->type == data.type);
	assert(out->length == data.length);
	for (u32 i = 0; i < object->width(); ++i)
	{
		assert(out->data[i] == data.data[i]);
	}
	std::cout << "object test passed" << std::endl;
}

void page_test()
{
	u8 buf[12];
	for (int i = 0; i < 4; ++i)
		buf[i] = 'a';
	RawData& data = *RawData::make_row_data(buf);
	data.type = T_VARCHAR;
	data.length = 4;
	
	u32 ret;
	IoService_s io = IoService::make_io_service();
	Page_s page = Page::make_page(io, 1, 1024 * 16, 0, 1024 * 32, 20);
	RowDesc desc(3);
	ColumnDesc col_desc;
	col_desc.set_tid_cid(1, 0);
	desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(1, 1);
	desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(1, 2);
	desc.set_column_desc(2, col_desc);
	for (int i = 0; i < 10; ++i)
	{
		Row_s row = Row::make_row(desc);
		for (int j = 0; j < 3; ++j)
		{
			auto object = Object::make_object(data);
			ret = row->set_cell(j, object);
			assert(ret == SUCCESS);
		}
		u32 row_id;
		ret = page->insert_row(row_id, row);
		assert(ret == SUCCESS);
	}
	
	Page_s page_copy = page;
	page_copy->open();
	Row_s row;
	while (page_copy->get_next_row(row) == SUCCESS)
	{
		for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i)
		{
			Object_s object;
			row->get_cell(i, object);
			assert(object->get_type() == T_VARCHAR);
			assert(object->is_fixed_length() == false);
			assert(object->is_null() == false);
			assert(object->width() == 4);
		}
	}
	page_copy->close();
	std::cout << "page test passed" << std::endl;
}

void table_space_test(const String& table_name)
{
	u32 ret;

	RowDesc desc(3);
	ColumnDesc col_desc;
	col_desc.set_tid_cid(1, 0);
	desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(1, 1);
	desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(1, 2);
	desc.set_column_desc(2, col_desc);

	TableSpace_s table = TableSpace::make_table_space(table_name);
	table->open();
	for (int i = 0; i < 1000; ++i)
	{
		Row_s row = Row::make_row(desc);
		for (int j = 0; j < 3; ++j)
		{
			auto object = Number::make_object(i+j+500);
			ret = row->set_cell(j, object);
			assert(ret == SUCCESS);
		}
		ret = table->insert_row(row);
		row = Row::make_row(desc);
		for (int j = 0; j < 3; ++j)
		{
			auto object = Number::make_object(i + j + 500);
			ret = row->set_cell(j, object);
			assert(ret == SUCCESS);
		}
		ret = table->insert_row(row);
		assert(ret == SUCCESS);
	}
	Row_s row;
	while (table->get_next_row(row) == SUCCESS)
	{
		for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i)
		{
			Object_s object;
			row->get_cell(i, object);
			assert(object->get_type() == T_NUMBER);
			assert(object->is_fixed_length() == true);
			assert(object->is_null() == false);
			assert(object->width() == 8);
		}
	}
	table->close();
	std::cout << "table space test passed" << std::endl;
}

void expression_test()
{
	TableSpace_s table = TableSpace::make_table_space("test");

	Object_s value = Number::make_object(5000);
	Expression_s e_value = ConstExpression::make_const_expression(value);
	ColumnDesc desc;
	desc.set_tid_cid(1, 0);
	Expression_s c_value = ColumnExpression::make_column_expression(desc);
	Expression_s e_filter = BinaryExpression::make_binary_expression(c_value, e_value, Operation::OP_GREATER);

	desc.set_tid_cid(1, 1);
	c_value = ColumnExpression::make_column_expression(desc);
	Expression_s e_filter2 = BinaryExpression::make_binary_expression(c_value, e_value, Operation::OP_GREATER);
	e_filter = BinaryExpression::make_binary_expression(e_filter, e_filter2, Operation::OP_AND);
	
	Filter_s filter = Filter::make_filter(e_filter);

	RowDesc row_desc(3);
	ColumnDesc col_desc;
	col_desc.set_tid_cid(1, 0);
	row_desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(1, 1);
	row_desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(1, 2);
	row_desc.set_column_desc(2, col_desc);
	Timer timer;
	PhyOperator_s scan = TableScan::make_table_scan(table, row_desc, filter);
	scan->open();

	Row_s row;
	int n = 0;
	while (scan->get_next_row(row) == SUCCESS)
	{
		++n;
	}
	scan->close();
	std::cout << "output " << n << " rows" << std::endl;
}

String fix_length(const String& str)
{
	u32 size = 6 - str.size();
	return str + String(size, ' ');
}

void hash_join_test()
{
	RowDesc row_desc(3);
	ColumnDesc col_desc;
	
	Object_s value = Number::make_object(600);
	Expression_s e_value = ConstExpression::make_const_expression(value);
	col_desc.set_tid_cid(1, 0);
	Expression_s c_value = ColumnExpression::make_column_expression(col_desc);
	Expression_s e_filter = BinaryExpression::make_binary_expression(c_value, e_value, Operation::OP_GREATER);
	Filter_s filter = Filter::make_filter(e_filter);
	TableSpace_s left_table = TableSpace::make_table_space("test1");
	col_desc.set_tid_cid(1, 0);
	row_desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(1, 1);
	row_desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(1, 2);
	row_desc.set_column_desc(2, col_desc);
	PhyOperator_s left_scan = TableScan::make_table_scan(left_table, row_desc, filter);


	TableSpace_s right_table = TableSpace::make_table_space("test2");
	col_desc.set_tid_cid(2, 0);
	row_desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(2, 1);
	row_desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(2, 2);
	row_desc.set_column_desc(2, col_desc);
	PhyOperator_s right_scan = TableScan::make_table_scan(right_table, row_desc);

	col_desc.set_tid_cid(1, 0);
	Expression_s left_value = ColumnExpression::make_column_expression(col_desc);
	col_desc.set_tid_cid(2, 0);
	Expression_s right_value = ColumnExpression::make_column_expression(col_desc);
	Expression_s join_condition = BinaryExpression::make_binary_expression(left_value, right_value, Operation::OP_EQUAL);

	PhyOperator_s join = HashJoin::make_hash_join(left_scan, right_scan, join_condition, join_condition, 1);

	col_desc.set_tid_cid(2, 1);
	Expression_s sort_col1 = ColumnExpression::make_column_expression(col_desc);
	col_desc.set_tid_cid(2, 2);
	Expression_s sort_col2 = ColumnExpression::make_column_expression(col_desc);
	Vector<Expression_s> sort_cols;
	sort_cols.push_back(sort_col1);
	sort_cols.push_back(sort_col2);
	PhyOperator_s sort = TopNSort::make_topn_sort(join, sort_cols, 20, false);
	
	col_desc.set_tid_cid(1, 0);
	Expression_s distinct_col = ColumnExpression::make_column_expression(col_desc);
	Vector<Expression_s> distinct_cols;
	distinct_cols.push_back(distinct_col);
	PhyOperator_s distinct = HashDistinct::make_hash_distinct(sort, distinct_cols);

	PhyOperator_s limit = Limit::make_limit(distinct, 10);

	col_desc.set_tid_cid(1, 0);
	Expression_s group_col = ColumnExpression::make_column_expression(col_desc);
	Expression_s agg_func = AggregateExpression::make_aggregate_expression(group_col, Operation::OP_COUNT);
	PhyOperator_s group = ScalarGroup::make_scalar_group(limit, agg_func);

	PhyOperator_s plan = distinct;
	Timer timer;
	Row_s row;
	int n = 0;
	plan->open();
	while (plan->get_next_row(row) == SUCCESS)
	{
		++n;
		std::cout << "| ";
		for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i)
		{
			Object_s cell;
			row->get_cell(i, cell);
			std::cout << fix_length(cell->to_string()) << " | ";
		}
		std::cout << std::endl;
		for (u32 i = 0; i < 55; ++i)
			std::cout << "-";
		std::cout << std::endl;
	}
	plan->close();
	std::cout << "affect " << n << " rows" << std::endl;
}

void set_test()
{
	RowDesc row_desc(3);
	ColumnDesc col_desc;

	Object_s value = Number::make_object(950);
	Expression_s e_value = ConstExpression::make_const_expression(value);
	col_desc.set_tid_cid(1, 0);
	Expression_s c_value = ColumnExpression::make_column_expression(col_desc);
	Expression_s e_filter = BinaryExpression::make_binary_expression(c_value, e_value, Operation::OP_GREATER);
	Filter_s filter = Filter::make_filter(e_filter);
	TableSpace_s left_table = TableSpace::make_table_space("test1");
	col_desc.set_tid_cid(1, 0);
	row_desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(1, 1);
	row_desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(1, 2);
	row_desc.set_column_desc(2, col_desc);
	PhyOperator_s left_scan = TableScan::make_table_scan(left_table, row_desc, filter);


	TableSpace_s right_table = TableSpace::make_table_space("test2");
	col_desc.set_tid_cid(2, 0);
	row_desc.set_column_desc(0, col_desc);
	col_desc.set_tid_cid(2, 1);
	row_desc.set_column_desc(1, col_desc);
	col_desc.set_tid_cid(2, 2);
	row_desc.set_column_desc(2, col_desc);
	PhyOperator_s right_scan = TableScan::make_table_scan(right_table, row_desc);

	PhyOperator_s left_limit = Limit::make_limit(left_scan, 10, 1083);
	PhyOperator_s right_limit = Limit::make_limit(right_scan, 8, 1799);

	PhyOperator_s set = UnionAll::make_union_all(left_limit, right_limit);

	PhyOperator_s plan = set;
	Timer timer;
	Row_s row;
	int n = 0;
	plan->open();
	while (plan->get_next_row(row) == SUCCESS)
	{
		++n;
		std::cout << "| ";
		for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i)
		{
			Object_s cell;
			row->get_cell(i, cell);
			std::cout << fix_length(cell->to_string()) << " | ";
		}
		std::cout << std::endl;
		for (u32 i = 0; i < 55; ++i)
			std::cout << "-";
		std::cout << std::endl;
	}
	plan->close();
	std::cout << "affect " << n << " rows" << std::endl;
}

#include "sql_driver.h"
#include <fstream>
using namespace CatDB;
void parser_test()
{
	std::ofstream log;
	log.open("log.txt", std::fstream::out);
	String query = R"(create table t1 (c1 number, c2 varchar, c3 datetime);)";
	SqlDriver parser;
	parser.set_yacc_debug(true);
	parser.set_yacc_debug_ostream(log);
	int ret = parser.parse_sql(query);
	if (ret)
	{
		std::cout << "parse error!" << std::endl;
		if (parser.is_sys_error())
			std::cout << parser.sys_error() << std::endl;
		if (parser.is_syntax_error())
			std::cout << parser.syntax_error() << std::endl;
	}
	else
	{
		std::cout << "parse finished" << std::endl;
	}
	log.close();
}

int main()
{
	//object_test();
	//page_test();
	//table_space_test("test1");
	//table_space_test("test2");
	//hash_join_test();
	//set_test();
	parser_test();
	return 0;
}
