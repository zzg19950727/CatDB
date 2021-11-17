#include "tpch.h"
#include "type.h"
#include <fstream>
#include <iostream>
#include "sql_engine.h"
#include "query_ctx.h"
#include "error.h"
using namespace CatDB::Sql;

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
	QueryCtx_s query_ctx = QueryCtx::make_query_ctx();
	ResultSet_s result_set;
	return SqlEngine::handle_inner_sql(query, query_ctx, result_set) == SUCCESS;
}

void create_table()
{
	Vector<String> sqls = {
		R"(CREATE TABLE TPCH.NATION(N_NATIONKEY  NUMERIC,
			N_NAME       VARCHAR,
			N_REGIONKEY  NUMERIC,
			N_COMMENT    VARCHAR);)",

		R"(CREATE TABLE TPCH.REGION(R_REGIONKEY  NUMERIC,
		R_NAME       VARCHAR,
		R_COMMENT    VARCHAR);)",

		R"(CREATE TABLE TPCH.PART(P_PARTKEY     NUMERIC,
		P_NAME        VARCHAR,
		P_MFGR        VARCHAR,
		P_BRAND       VARCHAR,
		P_TYPE        VARCHAR,
		P_SIZE        NUMERIC,
		P_CONTAINER   VARCHAR,
		P_RETAILPRICE NUMERIC,
		P_COMMENT     VARCHAR);)",

		R"(CREATE TABLE TPCH.SUPPLIER(S_SUPPKEY     NUMERIC,
		S_NAME        VARCHAR,
		S_ADDRESS     VARCHAR,
		S_NATIONKEY   NUMERIC,
		S_PHONE       VARCHAR,
		S_ACCTBAL     NUMERIC,
		S_COMMENT     VARCHAR);)",

		R"(CREATE TABLE TPCH.PARTSUPP(PS_PARTKEY     NUMERIC,
		PS_SUPPKEY     NUMERIC,
		PS_AVAILQTY    NUMERIC,
		PS_SUPPLYCOST  NUMERIC ,
		PS_COMMENT     VARCHAR);)",

		R"(CREATE TABLE TPCH.CUSTOMER(C_CUSTKEY     NUMERIC,
		C_NAME        VARCHAR,
		C_ADDRESS     VARCHAR,
		C_NATIONKEY   NUMERIC,
		C_PHONE       VARCHAR,
		C_ACCTBAL     NUMERIC  ,
		C_MKTSEGMENT  VARCHAR,
		C_COMMENT     VARCHAR);)",

		R"(CREATE TABLE TPCH.ORDERS(O_ORDERKEY       NUMERIC,
		O_CUSTKEY        NUMERIC,
		O_ORDERSTATUS    VARCHAR,
		O_TOTALPRICE     NUMERIC,
		O_ORDERDATE      DATETIME,
		O_ORDERPRIORITY  VARCHAR,
		O_CLERK          VARCHAR,
		O_SHIPPRIORITY   NUMERIC,
		O_COMMENT        VARCHAR);)",

		R"(CREATE TABLE TPCH.LINEITEM(L_ORDERKEY    NUMERIC,
		L_PARTKEY     NUMERIC,
		L_SUPPKEY     NUMERIC,
		L_LINENUMBER  NUMERIC,
		L_QUANTITY    NUMERIC,
		L_EXTENDEDPRICE  NUMERIC,
		L_DISCOUNT    NUMERIC,
		L_TAX         NUMERIC,
		L_RETURNFLAG  VARCHAR,
		L_LINESTATUS  VARCHAR,
		L_SHIPDATE    DATETIME,
		L_COMMITDATE  DATETIME,
		L_RECEIPTDATE DATETIME,
		L_SHIPINSTRUCT VARCHAR,
		L_SHIPMODE     VARCHAR,
		L_COMMENT      VARCHAR);)"
	};
	execute_sql("drop database tpch");
	execute_sql("create database tpch");
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
	u32 n = 0, m = 0;
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
				m += n;
				n = 0;
				cache.clear();
				printf("\rinsert into part %d / 200000 rows", m);
				continue;
			}
			else {
				std::cout << "\nload part failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			m += n;
			n = 0;
			cache.clear();
			printf("\rinsert into part %d / 200000 rows", m);
		}
		else {
			std::cout << "\nload part failed" << std::endl;
			return;
		}
	}
	std::cout << "\nload part success" << std::endl;
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
	u32 n = 0, m = 0;
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
				m += n;
				n = 0;
				cache.clear();
				printf("\rinsert into customer %d / 150000 rows", m);
				continue;
			}
			else {
				std::cout << "\nload customer failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			m += n;
			n = 0;
			cache.clear();
			printf("\rinsert into customer %d / 150000 rows", m);
		}
		else {
			std::cout << "\nload customer failed" << std::endl;
			return;
		}
	}
	std::cout << "\nload customer success" << std::endl;
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
	u32 n = 0, m = 0;
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
				m += n;
				n = 0;
				cache.clear();
				printf("\rinsert into partsupp %d / 800000 rows", m);
				continue;
			}
			else {
				std::cout << "\nload partsupp failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			m += n;
			n = 0;
			cache.clear();
			printf("\rinsert into partsupp %d / 800000 rows", m);
		}
		else {
			std::cout << "\nload partsupp failed" << std::endl;
			return;
		}
	}
	std::cout << "\nload partsupp success" << std::endl;
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
	u32 n = 0, m = 0;
	
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
				m += n;
				n = 0;
				cache.clear();
				printf("\rinsert into orders %d / 1500000 rows", m);
				continue;
			}
			else {
				std::cout << "\nload orders failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			m += n;
			n = 0;
			cache.clear();
			printf("\rinsert into orders %d / 1500000 rows", m);
		}
		else {
			std::cout << "\nload orders failed" << std::endl;
			return;
		}
	}
	std::cout << "\nload orders success" << std::endl;
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
	u32 n = 0, m = 0;
	u32 count = 0;
	while (!in.eof()) {
		std::getline(in, line);
		Vector<String> values;
		splite(line, '|', values);
		if (values.size() != 16) {
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
				m += n;
				n = 0;
				cache.clear();
				printf("\rinsert into lineitem %d / 6001215 rows", m);
				continue;
			}
			else {
				std::cout << "\nload lineitem failed" << std::endl;
				return;
			}
		}
	}
	if (n > 0) {
		cache[cache.size() - 1] = ';';
		if (execute_sql(sql + cache)) {
			m += n;
			n = 0;
			cache.clear();
			printf("\rinsert into lineitem %d / 6001215 rows", m);
		}
		else {
			std::cout << "\nload lineitem failed" << std::endl;
			return;
		}
	}
	std::cout << "\nload lineitem success " << std::endl;
}

void init_db()
{
	String sqls[] = {
	"drop database test",
	"drop database system",
	"create database system",
	"create table system.sys_databases(id int, name varchar)",
	"create table system.sys_tables(id int, name varchar, db_id int)",
	"create table system.sys_columns(id int, name varchar, table_id int, type varchar)",
	"create table system.user(name varchar, host varchar, auth_string varchar)",
	"create table system.table_statis(tid int, row_count int, space_size int, analyze_time datetime)",
	"create table system.column_statis(tid int, cid int, ndv int, null_count int, max_value int, min_value int, analyze_time datetime)",
	"create database test"
	};
	for (u32 i = 0; i < 10; ++i) {
		execute_sql(sqls[i]);
	}
}