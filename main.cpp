#include <iostream>
#include <cassert>
#include <chrono>
#include "tpch.h"
#include "schema_checker.h"
#include "query_result.h"
#include "sql_driver.h"
#include "expr_stmt.h"
#include "object.h"
#include "error.h"
#include "plan.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;
using CatDB::Parser::ExprStmt;
using CatDB::Parser::AggrStmt;
using CatDB::SqlDriver;

extern String g_database;

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
		long long msec = (end - start) / 1000000;
		String time;
		time = std::to_string(msec % 1000) + "ms" + time;
		msec /= 1000;
		if (msec > 0)
		{
			time = std::to_string(msec % 60) + "s" + time;
			msec /= 60;
			if (msec > 0)
			{
				time = std::to_string(msec % 60) + "min" + time;
				msec /= 60;
			}
		}
		std::cout << "cost " << time << std::endl;
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

Vector<u32> length;
String fix_length(u32 i, const String& str)
{
	u32 size = length[i] - str.length();
	return str + String(size, ' ');
}

void print_line(u32 column_count)
{
	std::cout << "+";
	for (u32 i = 0; i < column_count; ++i) {
		for (u32 j = 0; j < length[i]+2; ++j)
			std::cout << "-";
		std::cout << "+";
	}
	std::cout << std::endl;
}

void print_row(const Row_s& row)
{
	std::cout << "| ";
	for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i)
	{
		Object_s cell;
		row->get_cell(i, cell);
		std::cout << fix_length(i, cell->to_string()) << " | ";
	}
	std::cout << std::endl;
}

void print_query_result(QueryResult* result, const Row_s& title)
{
	Row_s row;
	u32 col_count = 0;
	length = Vector<u32>(40, 0);
	for (u32 i = 0; i < result->size(); ++i) {
		result->get_row(i, row);
		col_count = row->get_row_desc().get_column_num();
		for (u32 i = 0; i < col_count; ++i)
		{
			Object_s cell;
			row->get_cell(i, cell);
			if (length[i] < cell->to_string().length())
				length[i] = cell->to_string().length();
			title->get_cell(i, cell);
			if (length[i] < cell->to_string().length())
				length[i] = cell->to_string().length();
		}
	}
	if (result->size() == 0)
		return;
	print_line(col_count);
	print_row(title);
	print_line(col_count);
	for (u32 i = 0; i < result->size(); ++i) {
		result->get_row(i, row);
		print_row(row);
	}
	print_line(col_count);
	std::cout << "affect " << result->size() << " rows" << std::endl;
}

void parser_test()
{
	bool create_data = false;
	Plan_s plan;
	
	u32 n = 1000;
	while (n--) {
		plan.reset();
		SqlDriver parser;
		String query;
		if (!create_data) {
			std::cout << "CatSQL(" << g_database << ") > ";
			std::getline(std::cin, query);
		}
		else {
			query = "insert into t2 values";
			for (u32 i = 0; i < 600; ++i) {
				query += "(" + std::to_string(rand() % 100000) + "," + std::to_string(rand() % 100000) + ","
					 + std::to_string(rand() % 100000) + "),";
			}
			query[query.size() - 1] = ';';
		}
		if (query == "quit") {
			std::cout << "bye" << std::endl;
			break;
		}
		
		Timer timer;
		int ret = parser.parse_sql(query);
		
		if (parser.is_sys_error())
			std::cout << parser.sys_error() << std::endl;
		else if (parser.is_syntax_error())
			std::cout << parser.syntax_error() << std::endl;
		else {
			plan = Plan::make_plan(parser.parse_result());
			u32 ret = plan->build_plan();
			if (ret != SUCCESS) {
				Object_s result = plan->get_result();
				if (result)
					std::cout << "build plan failed:" << result->to_string() << std::endl;
				continue;
			}
			ret = plan->execute();
			if (ret != SUCCESS) {
				Object_s result = plan->get_result();
				if (result)
					std::cout << "execute plan failed:" << result->to_string() << std::endl;
				continue;
			}
			else {
				Object_s result = plan->get_result();
				QueryResult* query_result = dynamic_cast<QueryResult*>(result.get());
				print_query_result(query_result, plan->get_result_title());
			}
		}
	}
}

void schema_checker_test()
{
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	checker->add_database("test");
	Vector<String> databases;
	checker->show_database(databases);
	std::cout << "show databases" << std::endl;
	for (u32 i = 0; i < databases.size(); ++i) {
		std::cout << databases[i] << std::endl;
	}
	Vector<Pair<String, String>> columns;
	columns.push_back(Pair<String, String>("c1", "number"));
	columns.push_back(Pair<String, String>("c2", "number"));
	checker->add_table("test", "t1", columns);

	columns.push_back(Pair<String, String>("c3", "number"));
	checker->add_table("test", "t2", columns);

	columns.push_back(Pair<String, String>("c4", "number"));
	checker->add_table("test", "t3", columns);

	Vector<String> tables;
	checker->show_tables("test", tables);
	std::cout << "show tables" << std::endl;
	for (u32 i = 0; i < tables.size(); ++i) {
		std::cout << tables[i] << std::endl;
	}

	Vector<Pair<String, String>> desc;
	checker->desc_table("test", "t2", desc);
	std::cout << "desc t2" << std::endl;
	for (u32 i = 0; i < desc.size(); ++i) {
		std::cout << desc[i].first << "," << desc[i].second << std::endl;
	}
}

int main()
{
	//schema_checker_test();
	parser_test();
	//load_tpch_data();
	Timer timer;
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	//checker->analyze_table("tpch", "*");
	return 0;
}

