#include "package_base.h"
#include "obj_varchar.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Package;
using namespace CatDB::Common;

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