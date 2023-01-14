#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H
#include "register/package_base.h"
#include "print_helper.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(CreatePackageParam);
    }
    namespace Package {
        using Common::DataType;
        using Parser::CreatePackageParam_s;

        DECLARE(PackageManager);
        class PackageManager {
        public:
            struct PackageFuncInfo {
                PackageFuncInfo()
                    :func_ptr(NULL)
                {}
                
                String name;
                PackageBase::PackageFuncType func_ptr;
                Vector<String> param_names;
                Vector<DataType> param_type_list;
                Vector<String> return_columns;
                Vector<DataType> return_type_list;

                KV_STRING(
                    K(name),
                    K(param_type_list),
                    K(return_columns),
                    K(return_type_list)
                );
            };
            typedef UnorderedHashMap<String, PackageFuncInfo> PackageFuncMap;
            struct PackageInfo {
                PackageFuncMap funcs;
                String name;

                KV_STRING(
                    K(name),
                    K(funcs)
                );
            };
            typedef UnorderedHashMap<String, PackageInfo> PackageMap;
        public:
            static PackageManager_s &get_package_manager();
            u32 init();
            u32 get_all_package_name(Vector<String> &packages);
            u32 add_package(CreatePackageParam_s param);
            u32 get_func_info(const String& package_name, 
                              const String& func_name, 
                              PackageFuncInfo& info);
            u32 get_package_info(const String& package_name, 
                                 PackageInfo& info);
            KV_STRING(
                K(all_packages)
            );
        private:
            u32 add_package(const String& name);
            u32 set_function(const String &package_name, 
                             const String& func_name, 
                             PackageBase::PackageFuncType func_ptr);
            u32 parse_package_definition(const String &sql);
        private:
            PackageMap all_packages;
        };
    }
}

#endif  //PACKAGE_MANAGER_H