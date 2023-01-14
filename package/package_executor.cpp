#include "package_executor.h"
#include "package_manager.h"
#include "obj_cast_util.h"
#include "dml_resolver.h"
#include "expr_stmt.h"
#include "obj_varchar.h"
#include "obj_number.h"
#include "sql_engine.h"
#include "cmd_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Package;
using namespace CatDB::Sql;

u32 PackageExecutor::execute_function(ExecFunctionParam_s param, ResultSet_s &query_result)
{
    u32 ret = SUCCESS;
    Vector<Object_s> param_list;
    PackageBase::PackageResType result_objs;
    PackageManager_s &manager = PackageManager::get_package_manager();
    PackageManager::PackageFuncInfo info;
    CHECK(manager->get_func_info(param->package_name, 
                                 param->function_name, 
                                 info));
    MY_ASSERT(info.func_ptr);
    CHECK(get_func_params(param, 
                          info.param_type_list,
                          param_list));
    CHECK(info.func_ptr(param_list, result_objs));
    CHECK(init_result_head(info.return_columns, query_result));
    for (u32 i = 0; i < result_objs.size(); ++i) {
        MY_ASSERT(info.return_columns.size() == result_objs[i].size());
        CHECK(add_objects_to_result_set(result_objs[i], query_result));
    }
    return ret;
}

u32 PackageExecutor::get_func_params(ExecFunctionParam_s &param, 
                                     Vector<DataType> &param_type,
                                     Vector<Object_s> &param_list)
{
    u32 ret = SUCCESS;
    bool need_cast = false;
    bool bool_result = false;
    Object_s obj_result;
    MY_ASSERT(param->param_list.size() == param_type.size());
    for (u32 i = 0; i < param_type.size(); ++i) {
        ExprStmt_s &expr = param->param_list[i];
        obj_result.reset();
        CHECK(expr->formalize());
        if (!expr->has_flag(IS_CONST)) {
            ret = ERR_UNEXPECTED;
        }
        CHECK(ObjCastUtil::check_need_cast(expr->res_type, 
                                           param_type[i], 
                                           need_cast,
                                           true));
        if (need_cast) {
            CHECK(ObjCastUtil::add_cast(expr, 
                                        param_type[i], 
                                        expr));
        }
        CHECK(SqlEngine::handle_const_expr(expr, 
                                           obj_result, 
                                           bool_result));
        if (expr->res_type.is_bool()) {
            obj_result = Number::make_int_object(bool_result);
        }
        MY_ASSERT(obj_result);
        param_list.push_back(obj_result);
    }
    return ret;
}

u32 PackageExecutor::init_result_head(const Vector<String> &title, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
    for (u32 i = 0; i < title.size(); ++i) {
		query_result->add_result_title(title[i], T_VARCHAR);
	}
    return ret;
}

u32 PackageExecutor::add_objects_to_result_set(Vector<Object_s> &objs, ResultSet_s &query_result)
{
	u32 ret = SUCCESS;
	Row_s row = Row::make_row(objs.size());
	for (u32 i = 0; i < objs.size(); ++i) {
		row->set_cell(i, objs[i]);
	}
	query_result->add_row(row);
	return ret;
}