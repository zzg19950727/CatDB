#ifndef OBJ_CAST_UTIL_H
#define OBJ_CAST_UTIL_H
#include "print_helper.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(SetStmt);
    }
	namespace Common {
        using Parser::ExprStmt_s;
        using Parser::SetStmt_s;
		DECLARE(ObjCastUtil);
		class ObjCastUtil
		{
		public:
            static u32 check_need_cast(const DataType &from, 
                                       const DataType &to,
                                       bool &need_cast,
                                       const bool is_restrict = false);

            static u32 get_result_type(const DataType& lhs, 
                                        bool &l_need_cast,
                                        const DataType& rhs, 
                                        bool &r_need_cast,
                                        DataType &dst_type);

			static u32 get_result_type(const Vector<DataType>& src_types,
                                       Vector<bool>& need_cast, 
                                       DataType& dst_type);
		
            static u32 check_compare_type(const DataType& lhs, 
                                          bool &l_need_cast,
                                          DataType &l_dst_type,
                                          const DataType& rhs, 
                                          bool &r_need_cast,
                                          DataType &r_dst_type,
                                          DataType &dst_type);

            static u32 check_calc_type(const DataType& lhs, 
                                        bool &l_need_cast,
                                        DataType &l_dst_type,
                                        const DataType& rhs, 
                                        bool &r_need_cast,
                                        DataType &r_dst_type,
                                        DataType &dst_type);

            static u32 check_logical_type(const DataType& lhs,
                                        DataType &dst_type);

            static u32 get_result_type(const DataType& lhs, 
                                       bool &l_need_cast,
                                       DataType &l_dst_type,
                                       const DataType& rhs, 
                                       bool &r_need_cast,
                                       DataType &r_dst_type,
                                       OperationType op_type,
                                       DataType &dst_type);

            static u32 add_cast(ExprStmt_s& from_expr, 
                                const DataType& to_type, 
                                ExprStmt_s &to_expr);

            static u32 inner_add_cast(ExprStmt_s& from_expr, 
                                    const DataType& to_type, 
                                    ExprStmt_s &to_expr);
        
            static u32 deduce_set_expr_type(SetStmt_s &stmt);
        };
	}
}
#endif	//OBJ_CAST_UTIL_H
