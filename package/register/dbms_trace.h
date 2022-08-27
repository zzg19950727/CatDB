#ifndef DBMS_TRACE_H
#define DBMS_TRACE_H
#include "package_base.h"

namespace CatDB {
    namespace Package {
        class DBMS_TRACE : public PackageBase
        {
        public:
            static u32 set_log_level(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_log_level(const Vector<Object_s> &params, PackageResType &result);
            static u32 set_log_module(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_log_module(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_last_trace_id(const Vector<Object_s> &params, PackageResType &result);
        };
    }
}

#endif  //DBMS_TRACE_H