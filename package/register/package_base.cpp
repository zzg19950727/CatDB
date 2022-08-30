#include "package_base.h"
#include "obj_varchar.h"
#include "sql_engine.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Package;
using namespace CatDB::Common;
using namespace CatDB::Sql;

u32 PackageBase::add_result(const String &value, PackageResType &result)
{
    u32 ret = SUCCESS;
    Vector<Object_s> objs;
    Object_s obj = Varchar::make_object(value);
    objs.push_back(obj);
    result.push_back(objs);
    return ret;
}

u32 PackageBase::add_result(const String &value1, const String &value2, PackageResType &result)
{
    u32 ret = SUCCESS;
    Vector<Object_s> objs;
    Object_s obj = Varchar::make_object(value1);
    objs.push_back(obj);
    obj = Varchar::make_object(value2);
    objs.push_back(obj);
    result.push_back(objs);
    return ret;
}

u32 PackageBase::add_result_from_inner_sql(const String &sql, PackageResType &result)
{
    u32 ret = SUCCESS;
    ResultSet_s result_set;
    CHECK(SqlEngine::handle_inner_sql(sql, result_set));
    Row_s row;
    MY_ASSERT(result_set);
	CHECK(result_set->open());
	while (SUCC(result_set->get_next_row(row))) {
		result.push_back(row->get_all_cells());
	}
	int temp_ret = ret;
	if (NO_MORE_ROWS == ret) {
		temp_ret = SUCCESS;
	}
	CHECK(result_set->close());
	ret = temp_ret;
    return ret;
}