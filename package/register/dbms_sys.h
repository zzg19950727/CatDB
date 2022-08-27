#ifndef DBMS_SYS_H
#define DBMS_SYS_H
#include "package_base.h"
#include "type.h"

namespace CatDB {
    namespace Package {
        class DBMS_SYS : public PackageBase
        {
        public:
            static u32 set_sys_var(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_sys_var(const Vector<Object_s> &params, PackageResType &result);
            static u32 list_sys_var(const Vector<Object_s> &params, PackageResType &result);
            static u32 set_session_var(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_session_var(const Vector<Object_s> &params, PackageResType &result);
            static u32 list_session_var(const Vector<Object_s> &params, PackageResType &result);
        };
    }
}

#endif  //DBMS_SYS_H