#include "dbms_stats.h"
#include "schema_checker.h"
#include "statis_manager.h"
#include "session_info.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Package;
using namespace CatDB::Parser;
using namespace CatDB::Optimizer;

u32 DBMS_STATS::gather_database_stats(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
	String database = params[0]->to_string();
	String table = "*";
	double sample_size = params[1]->value();
    
	StatisManager_s &manager = StatisManager::get_statis_manager();
	MY_ASSERT(manager);
	QUERY_CTX->set_sample_size(sample_size);
	CHECK(manager->analyze_table(database, table, sample_size));
	return ret;
}

u32 DBMS_STATS::gather_table_stats(const Vector<Object_s> &params, PackageResType &result)
{
    u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 3);
	String database = params[0]->to_string();
	String table = params[1]->to_string();
	double sample_size = params[2]->value();
    
	StatisManager_s &manager = StatisManager::get_statis_manager();
	MY_ASSERT(manager);
	QUERY_CTX->set_sample_size(sample_size);
	CHECK(manager->analyze_table(database, table, sample_size));
	return ret;
}

u32 DBMS_STATS::get_table_stats(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
	String database = params[0]->to_string();
	String table = params[1]->to_string();
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	u32 tid = INVALID_ID;
	CHECK(checker->get_table_id(database, table, tid));
	String query = R"(select row_count,space_size
				from "system"."table_statis"  
				where "tid" = )" + std::to_string(tid) +
		R"( order by "analyze_time" desc limit 1;)";
	CHECK(add_result_from_inner_sql(query, result));
	return ret;
}

u32 DBMS_STATS::get_column_stats(const Vector<Object_s> &params, PackageResType &result)
{
	u32 ret = SUCCESS;
    MY_ASSERT(params.size() == 2);
	String database = params[0]->to_string();
	String table = params[1]->to_string();
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	u32 tid = INVALID_ID;
	CHECK(checker->get_table_id(database, table, tid));
	String query = R"(select col.name as COLUMN_NAME, cs.ndv as NDV,cs.null_count as NULL_COUNT,
				cs.max_value as MAX_VALUE, cs.min_value as MIN_VALUE
				from system.sys_databases as db, system.sys_tables as tb, 
				system.sys_columns as col, "system"."column_statis" as cs  
				where db.id=tb.db_id and tb.id = col.table_id and col.id=cs.cid and 
				db.name=")" + database + R"(" and tb.name=")" + table + R"(" and cs."tid" = )" + std::to_string(tid) +
				R"(and cs.analyze_time = (select max(cs.analyze_time) from system.sys_databases as db, system.sys_tables as tb, 
				system.sys_columns as col, "system"."column_statis" as cs  
				where db.id=tb.db_id and tb.id = col.table_id and col.id=cs.cid and 
				db.name=")" + database + R"(" and tb.name=")" + table + R"(" and cs."tid" = )" + std::to_string(tid) + ")" +
				R"( order by col.id asc;)";
	CHECK(add_result_from_inner_sql(query, result));
	return ret;
}