#ifndef PACKAGE_BASE_H
#define PACKAGE_BASE_H
#include "type.h"

namespace CatDB {    
    namespace Common {
        DECLARE(Object);
    }
    namespace Package {
        using Common::Object_s;

        class PackageBase {
        public:
            typedef Vector<Vector<Object_s>> PackageResType;
            typedef u32 (*PackageFuncType)(const Vector<Object_s>&, PackageResType&);
            
        protected:
            PackageBase() {}
            virtual ~PackageBase() {}

            static u32 add_result(const String &value, PackageResType &result);
            static u32 add_result(const String &value1, const String &value2, PackageResType &result);
        };
    }
}

#endif  //PACKAGE_BASE_H