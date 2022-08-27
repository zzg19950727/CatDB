#ifndef PACKAGE_EXECUTOR_H
#define PACKAGE_EXECUTOR_H
#include "register/package_base.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExecFunctionParam);
    }
	namespace Sql {
		DECLARE(ResultSet);
	}
    namespace Package {
        using Common::DataType;
        using Sql::ResultSet_s;
        using Parser::ExecFunctionParam_s;

        class PackageExecutor {
        public:
            static u32 execute_function(ExecFunctionParam_s param, ResultSet_s &query_result);
        private:
            static u32 get_func_params(ExecFunctionParam_s &param, 
                                       Vector<DataType> &param_type,
                                       Vector<Object_s> &param_list);

            static u32 init_result_head(const Vector<String> &title, ResultSet_s &query_result);

            static u32 add_objects_to_result_set(Vector<Object_s> &objs, ResultSet_s &query_result);
        };
    }
}

#endif  //PACKAGE_EXECUTOR_H