#ifndef DBMS_SYS_H
#define DBMS_SYS_H
#include "package_base.h"
#include "type.h"

namespace CatDB {
    namespace Package {
        class DBMS_SYS : public PackageBase
        {
        public:
            static u32 set_sys_parameter(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_sys_parameter(const Vector<Object_s> &params, PackageResType &result);
            static u32 list_sys_parameters(const Vector<Object_s> &params, PackageResType &result);
            static u32 list_sys_config(const Vector<Object_s> &params, PackageResType &result);
            static u32 set_session_parameter(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_session_parameter(const Vector<Object_s> &params, PackageResType &result);
            static u32 list_session_parameters(const Vector<Object_s> &params, PackageResType &result);
        };
    }
}

#endif  //DBMS_SYS_H