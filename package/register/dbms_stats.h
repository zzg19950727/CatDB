#ifndef DBMS_STATS_H
#define DBMS_STATS_H
#include "package_base.h"

namespace CatDB {
    namespace Package {
        class DBMS_STATS : public PackageBase
        {
        public:
            static u32 gather_database_stats(const Vector<Object_s> &params, PackageResType &result);
            static u32 gather_table_stats(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_table_stats(const Vector<Object_s> &params, PackageResType &result);
            static u32 get_column_stats(const Vector<Object_s> &params, PackageResType &result);
        };
    }
}

#endif  //DBMS_STATS_H