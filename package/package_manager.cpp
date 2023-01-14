#include "package_manager.h"
#include "register/package_register.h"
#include "session_info.h"
#include "sql_engine.h"
#include "cmd_stmt.h"
#include "error.h"
#include "log.h"

#define str_to_lower(str) 					\
{											\
	for(u32 i = 0; i<str.size(); ++i){		\
		if(str[i] >= 'A' && str[i] <= 'Z'){	\
			str[i] -= 'A';					\
			str[i] += 'a';					\
		}									\
	}										\
}

//编译期检查是否注册了同名包
#undef REGISTE_PACKAGE
#undef REGISTE_PACKAGE_INTERFACE
#define REGISTE_PACKAGE(name, define_sql) name,
#define REGISTE_PACKAGE_INTERFACE(package_name, pro_or_func, func_ptr)
enum PACKAGE_UNIQUE_CHECK {
#include "register/package_register.def"
};  

//编译期检查是否存在同名函数
#undef REGISTE_PACKAGE
#undef REGISTE_PACKAGE_INTERFACE
#define REGISTE_PACKAGE(name, define_sql)
#define REGISTE_PACKAGE_INTERFACE(package_name, pro_or_func, func_ptr) \
        package_name##_##pro_or_func,
enum PACKAGE_FUNC_UNIQUE_CHECK {
#include "register/package_register.def"
};

//编译期检查是否所有函数定义是否包含在定义的包内
#undef REGISTE_PACKAGE
#undef REGISTE_PACKAGE_INTERFACE
#define REGISTE_PACKAGE(name, define_sql) 
#define PACKAGE_NAME_CHECK(name, pro_or_func) \
        PACKAGE_UNIQUE_CHECK name##pro_or_func##_test = name;
#define REGISTE_PACKAGE_INTERFACE(package_name, pro_or_func, func_ptr) \
        PACKAGE_NAME_CHECK(package_name, pro_or_func)
#include "register/package_register.def"


using namespace CatDB::Common;
using namespace CatDB::Parser;
using namespace CatDB::Package;

PackageManager_s& PackageManager::get_package_manager()
{
    static PackageManager_s manager(new PackageManager());
	return manager;
}

u32 PackageManager::init()
{
    u32 ret = SUCCESS;
    //注册所有的包名
    #undef REGISTE_PACKAGE
    #undef REGISTE_PACKAGE_INTERFACE
    #define REGISTE_PACKAGE_INTERFACE(package_name, pro_or_func, func_ptr)
    #define REGISTE_PACKAGE(name, define_sql) CHECK(add_package(#name)); 
    #include "register/package_register.def"
    //注册所有的包定义
    #undef REGISTE_PACKAGE
    #undef REGISTE_PACKAGE_INTERFACE
    #define REGISTE_PACKAGE(name, define_sql) CHECK(parse_package_definition(define_sql));
    #define REGISTE_PACKAGE_INTERFACE(package_name, pro_or_func, func_ptr)
    #include "register/package_register.def"
    //注册所有的函数接口
    #undef REGISTE_PACKAGE
    #undef REGISTE_PACKAGE_INTERFACE
    #define REGISTE_PACKAGE(name, define_sql)
    #define REGISTE_PACKAGE_INTERFACE(package_name, pro_or_func, func_ptr) \
            CHECK(set_function(#package_name, #pro_or_func, func_ptr));
    #define T(a,b) a##::##b
    #include "register/package_register.def"
    LOG_TRACE("init package manager", K(all_packages));
    return ret;
}

u32 PackageManager::get_all_package_name(Vector<String> &packages)
{
    u32 ret = SUCCESS;
    for (auto iter = all_packages.cbegin(); iter != all_packages.cend(); ++iter) {
        packages.push_back(iter->first);
    }
    return ret;
}

u32 PackageManager::add_package(const String& name)
{
    u32 ret = SUCCESS;
    String lower_name = name;
    str_to_lower(lower_name);
    if (all_packages.find(lower_name) != all_packages.cend()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("package exists", K(lower_name), K(ret));
    } else {
        PackageInfo info;
        info.name = lower_name;
        all_packages[lower_name] = info;
        LOG_TRACE("add package", K(lower_name));
    }
    return ret;
}

u32 PackageManager::add_package(CreatePackageParam_s param)
{
    u32 ret = SUCCESS;
    if (!param->is_replace && 
        all_packages.find(param->name) != all_packages.cend()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("package exists", K(param), K(ret));
        QUERY_CTX->set_error_msg("package "+param->name+" exists!");
    } else {
        PackageInfo info;
        info.name = param->name;
        info.funcs.clear();
        for (u32 i = 0; i < param->functions.size(); ++i) {
            FunctionDefinition_s &func = param->functions[i];
            PackageFuncInfo func_info;
            func_info.name = func->name;
            for (u32 j = 0; j < func->param_list.size(); ++j) {
                func_info.param_names.push_back(func->param_list[j]->column_name);
                func_info.param_type_list.push_back(func->param_list[j]->data_type);
            }
            for (u32 j = 0; j < func->return_type_list.size(); ++j) {
                func_info.return_columns.push_back(func->return_type_list[j]->column_name);
                func_info.return_type_list.push_back(func->return_type_list[j]->data_type);
            }
            info.funcs[func->name] = func_info;
            LOG_TRACE("add function:", K(func->name));
        }
        all_packages[param->name] = info;
    }
    return ret;
}

u32 PackageManager::get_func_info(const String& package_name, 
                                const String& func_name, 
                                PackageFuncInfo& info)
{
    u32 ret = SUCCESS;
    String lower_package_name = package_name;
    String lower_func_name = func_name;
    str_to_lower(lower_package_name);
    str_to_lower(lower_func_name);
    if (all_packages.find(lower_package_name) == all_packages.cend()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("package not exists", K(lower_package_name), K(ret));
    } else {
        PackageInfo &package_info = all_packages[lower_package_name];
        if (package_info.funcs.find(lower_func_name) == package_info.funcs.cend()) {
            ret = ERR_UNEXPECTED;
            LOG_ERR("package function not exists", K(lower_func_name), K(ret));
        } else {
            info = package_info.funcs[lower_func_name];
        }
    }
    return ret;
}

u32 PackageManager::get_package_info(const String& package_name, 
                                     PackageInfo& info)
{
    u32 ret = SUCCESS;
    String lower_package_name = package_name;
    str_to_lower(lower_package_name);
    if (all_packages.find(lower_package_name) == all_packages.cend()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("package not exists", K(lower_package_name), K(ret));
    } else {
        info = all_packages[lower_package_name];
    }
    return ret;
}

u32 PackageManager::set_function(const String &package_name, 
                                 const String& func_name, 
                                 PackageBase::PackageFuncType func_ptr)
{
    u32 ret = SUCCESS;
    String lower_package_name = package_name;
    String lower_func_name = func_name;
    str_to_lower(lower_package_name);
    str_to_lower(lower_func_name);
    if (all_packages.find(lower_package_name) == all_packages.cend()) {
        ret = ERR_UNEXPECTED;
        LOG_ERR("package not exists", K(lower_package_name), K(ret));
    } else {
        PackageInfo &info = all_packages[lower_package_name];
        if (info.funcs.find(lower_func_name) == info.funcs.cend()) {
            ret = ERR_UNEXPECTED;
            LOG_ERR("package function not exists", K(lower_func_name), K(ret));
        } else {
            PackageFuncInfo &func_info = info.funcs[lower_func_name];
            func_info.func_ptr = func_ptr;
        }
    }
    return ret;
}

u32 PackageManager::parse_package_definition(const String &sql)
{
    u32 ret = SUCCESS;
	CHECK(Sql::SqlEngine::handle_inner_sql(sql));
    return ret;
}