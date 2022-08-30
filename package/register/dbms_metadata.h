#ifndef DBMS_METADATA_H
#define DBMS_METADATA_H
#include "package_base.h"
#include "../package_manager.h"

namespace CatDB {
    namespace Package {
        class DBMS_METADATA : public PackageBase
        {
        public:
            static u32 get_table_ddl(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_view_ddl(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_package_ddl(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_function_ddl(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_procedure_ddl(const Vector<Object_s> &params, PackageResType &result);

        private:
            static u32 print_ident(const String& ident, String &ddl);
            static u32 print_function(const PackageManager::PackageFuncInfo &info, 
                                      bool is_func, 
                                      bool print_package, 
                                      String &ddl);
        };
    }
}

#endif  //DBMS_METADATA_H