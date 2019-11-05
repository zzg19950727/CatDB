#include "tpch.h"
#include "type.h"
#include <fstream>
#include <iostream>
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



void splite(const String& line, char c, Vector<String>& list)
{
	String tmp;
	for (u32 i = 0; i < line.size(); ++i) {
		if (line[i] == c) {
			list.push_back(tmp);
			tmp.clear();
		}
		else {
			tmp += line[i];
		}
	}
}

bool execute_sql(const String& query)
{
	Plan_s plan;
	SqlDriver parser;
	int ret = parser.parse_sql(query);

	if (parser.is_sys_error()) {
		//std::cout << parser.sys_error() << std::endl;
		return false;
	}
	else if (parser.is_syntax_error()) {
		//std::cout << parser.syntax_error() << std::endl;
		return false;
	}
	else {
		plan = Plan::make_plan(parser.parse_result());
		u32 ret = plan->build_plan();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result)
				std::cout << "build plan failed:" << result->to_string() << std::endl;
			return false;
		}
		ret = plan->execute();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result)
				std::cout << "execute plan failed:" << result->to_string() << std::endl;
			return false;
		}
		return true;
	}
}

void create_table()
{
	Vector<String> sqls = {
		R"(CREATE TABLE NATION(N_NATIONKEY  NUMBER,
			N_NAME       VARCHAR,
			N_REGIONKEY  NUMBER,
			N_COMMENT    VARCHAR);)",

		R"(CREATE TABLE REGION(R_REGIONKEY  NUMBER,
		R_NAME       VARCHAR,
		R_COMMENT    VARCHAR);)",

		R"(CREATE TABLE PART(P_PARTKEY     NUMBER,
		P_NAME        VARCHAR,
		P_MFGR        VARCHAR,
		P_BRAND       VARCHAR,
		P_TYPE        VARCHAR,
		P_SIZE        NUMBER,
		P_CONTAINER   VARCHAR,
		P_RETAILPRICE NUMBER,
		P_COMMENT     VARCHAR);)",

		R"(CREATE TABLE SUPPLIER(S_SUPPKEY     NUMBER,
		S_NAME        VARCHAR,
		S_ADDRESS     VARCHAR,
		S_NATIONKEY   NUMBER,
		S_PHONE       VARCHAR,
		S_ACCTBAL     NUMBER,
		S_COMMENT     VARCHAR);)",

		R"(CREATE TABLE PARTSUPP(PS_PARTKEY     NUMBER,
		PS_SUPPKEY     NUMBER,
		PS_AVAILQTY    NUMBER,
		PS_SUPPLYCOST  NUMBER ,
		PS_COMMENT     VARCHAR);)",

		R"(CREATE TABLE CUSTOMER(C_CUSTKEY     NUMBER,
		C_NAME        VARCHAR,
		C_ADDRESS     VARCHAR,
		C_NATIONKEY   NUMBER,
		C_PHONE       VARCHAR,
		C_ACCTBAL     NUMBER  ,
		C_MKTSEGMENT  VARCHAR,
		C_COMMENT     VARCHAR);)",

		R"(CREATE TABLE ORDERS(O_ORDERKEY       NUMBER,
		O_CUSTKEY        NUMBER,
		O_ORDERSTATUS    VARCHAR,
		O_TOTALPRICE     NUMBER,
		O_ORDERDATE      DATETIME,
		O_ORDERPRIORITY  VARCHAR,
		O_CLERK          VARCHAR,
		O_SHIPPRIORITY   NUMBER,
		O_COMMENT        VARCHAR);)",

		R"(CREATE TABLE LINEITEM(L_ORDERKEY    NUMBER,
		L_PARTKEY     NUMBER,
		L_SUPPKEY     NUMBER,
		L_LINENUMBER  NUMBER,
		L_QUANTITY    NUMBER,
		L_EXTENDEDPRICE  NUMBER,
		L_DISCOUNT    NUMBER,
		L_TAX         NUMBER,
		L_RETURNFLAG  VARCHAR,
		L_LINESTATUS  VARCHAR,
		L_SHIPDATE    DATETIME,
		L_COMMITDATE  DATETIME,
		L_RECEIPTDATE DATETIME,
		L_SHIPINSTRUCT VARCHAR,
		L_SHIPMODE     VARCHAR,
		L_COMMENT      VARCHAR);)"
	};
	execute_sql("create database tpch");
	execute_sql("use tpch");
	for (u32 i = 0; i < sqls.size(); ++i) {
		execute_sql(sqls[i]);
	}
}

void load_nation_data()
{
	std::ifstream in;
	in.open("tpch-data//nation.tbl");
	if (!in.is_open()) {
		return ;
	}
	String line;

	String sql = "insert into tpch.nation values";
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 4) {
			continue;
		}
		values[1] = "\"" + values[1] + "\"";
		values[3] = "\"" + values[3] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		sql += tmp + ",";
	}
	sql[sql.size() - 1] = ';';
	if (execute_sql(sql)) {
		std::cout << "load nation success" << std::endl;
	}
	else {
		std::cout << "load nation failed" << std::endl;
	}
}

void load_region_data()
{
	std::ifstream in;
	in.open("tpch-data//region.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.region values";
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 3) {
			continue;
		}
		values[1] = "\"" + values[1] + "\"";
		values[2] = "\"" + values[2] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		sql += tmp + ",";
	}
	sql[sql.size() - 1] = ';';
	if (execute_sql(sql)) {
		std::cout << "load region success" << std::endl;
	}
	else {
		std::cout << "load region failed" << std::endl;
	}
}

void load_supplier_data()
{
	std::ifstream in;
	in.open("tpch-data//supplier.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.supplier values";
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 7) {
			continue;
		}
		values[1] = "\"" + values[1] + "\"";
		values[2] = "\"" + values[2] + "\"";
		values[4] = "\"" + values[4] + "\"";
		values[6] = "\"" + values[6] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		sql += tmp + ",";
	}
	sql[sql.size() - 1] = ';';
	if (execute_sql(sql)) {
		std::cout << "load supplier success" << std::endl;
	}
	else {
		std::cout << "load supplier failed" << std::endl;
	}
}

void load_part_data()
{
	std::ifstream in;
	in.open("tpch-data//part.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.part values";
	String cache;
	u32 n = 0;
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 9) {
			continue;
		}
		values[1] = "\"" + values[1] + "\"";
		values[2] = "\"" + values[2] + "\"";
		values[3] = "\"" + values[3] + "\"";
		values[4] = "\"" + values[4] + "\"";
		values[6] = "\"" + values[6] + "\"";
		values[8] = "\"" + values[8] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		cache += tmp + ",";
		++n;
		if (n >= 6000) {
			cache[cache.size() - 1] = ';';
			if (execute_sql(sql+cache)) {
				n = 0;
				cache.clear();
				continue;
			}
			else {
				std::cout << "load part failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			n = 0;
			cache.clear();
		}
		else {
			std::cout << "load part failed" << std::endl;
			return;
		}
	}
	std::cout << "load part success" << std::endl;
}

void load_customer_data()
{
	std::ifstream in;
	in.open("tpch-data//customer.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.customer values";
	String cache;
	u32 n = 0;
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 8) {
			continue;
		}
		values[1] = "\"" + values[1] + "\"";
		values[2] = "\"" + values[2] + "\"";
		values[4] = "\"" + values[4] + "\"";
		values[6] = "\"" + values[6] + "\"";
		values[7] = "\"" + values[7] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		cache += tmp + ",";
		++n;
		if (n >= 6000) {
			cache[cache.size() - 1] = ';';
			if (execute_sql(sql + cache)) {
				n = 0;
				cache.clear();
				continue;
			}
			else {
				std::cout << "load customer failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			n = 0;
			cache.clear();
		}
		else {
			std::cout << "load customer failed" << std::endl;
			return;
		}
	}
	std::cout << "load customer success" << std::endl;
}

void load_partsupp_data()
{
	std::ifstream in;
	in.open("tpch-data//partsupp.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.partsupp values";
	String cache;
	u32 n = 0;
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 5) {
			continue;
		}
		values[4] = "\"" + values[4] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		cache += tmp + ",";
		++n;
		if (n >= 6000) {
			cache[cache.size() - 1] = ';';
			if (execute_sql(sql + cache)) {
				n = 0;
				cache.clear();
				continue;
			}
			else {
				std::cout << "load partsupp failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			n = 0;
			cache.clear();
		}
		else {
			std::cout << "load partsupp failed" << std::endl;
			return;
		}
	}
	std::cout << "load partsupp success" << std::endl;
}

void load_orders_data()
{
	std::ifstream in;
	in.open("tpch-data//orders.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.orders values";
	String cache;
	u32 n = 0;
	
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 9) {
			continue;
		}
		values[2] = "\"" + values[2] + "\"";
		values[4] = "'" + values[4] + "'";
		values[5] = "\"" + values[5] + "\"";
		values[6] = "\"" + values[6] + "\"";
		values[8] = "\"" + values[8] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		cache += tmp + ",";
		++n;
		if (n >= 6000) {
			cache[cache.size() - 1] = ';';
			if (execute_sql(sql + cache)) {
				n = 0;
				cache.clear();
				continue;
			}
			else {
				std::cout << "load orders failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			n = 0;
			cache.clear();
		}
		else {
			std::cout << "load orders failed" << std::endl;
			return;
		}
	}
	std::cout << "load orders success" << std::endl;
}

void load_lineitem_data()
{
	std::ifstream in;
	in.open("tpch-data//lineitem.tbl");
	if (!in.is_open()) {
		return;
	}
	String line;

	String sql = "insert into tpch.lineitem values";
	String cache;
	u32 n = 0;
	u32 count = 0;
	u32 err = 0;
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 16) {
			++err;
			continue;
		}
		values[8] = "\"" + values[8] + "\"";
		values[9] = "\"" + values[9] + "\"";
		values[10] = "'" + values[10] + "'";
		values[11] = "'" + values[11] + "'";
		values[12] = "'" + values[12] + "'";
		values[13] = "\"" + values[13] + "\"";
		values[14] = "\"" + values[14] + "\"";
		values[15] = "\"" + values[15] + "\"";
		String tmp = "(";
		for (u32 i = 0; i < values.size(); ++i) {
			tmp += values[i] + ",";
		}
		tmp[tmp.size() - 1] = ')';
		cache += tmp + ",";
		++n;
		++count;
		if (n >= 6000) {
			cache[cache.size() - 1] = ';';
			if (execute_sql(sql + cache)) {
				n = 0;
				cache.clear();
				continue;
			}
			else {
				std::cout << "load lineitem failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			n = 0;
			cache.clear();
		}
		else {
			std::cout << "load lineitem failed" << std::endl;
			return;
		}
	}
	std::cout << "load lineitem success " << count << " ,err:" << err << std::endl;
}

void load_tpch_data()
{/*
	create_table();
	load_nation_data();
	load_region_data();
	load_supplier_data();
	load_part_data();
	load_customer_data();
	load_partsupp_data();
	load_orders_data();
	load_lineitem_data();*/
}