#include "obj_cast_util.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Parser;

/**
 * number   --> varchar     invalid
 * number   --> datetime    invalid
 * varchar  --> number
 * varchar  --> datetime
 * datetime --> number      invalid
 * datetime --> varchar     invalid
 */
u32 ObjCastUtil::check_need_cast(const DataType &from, 
                                const DataType &to,
                                bool &need_cast,
                                const bool is_restrict)
{
    u32 ret = SUCCESS;
    need_cast = false;
    if (is_restrict && from.is_same(to)) {

    } else if (from.res_type == to.res_type) {
        need_cast = is_restrict;
    } else if (from.res_type != to.res_type) {
        if (from.is_varchar()) {
            need_cast = true;
        } else {
            ret = INVALID_CAST;
            LOG_ERR("can not cast:", K(from), K(to), K(ret));
        }
    }
    return ret;
}

u32 ObjCastUtil::get_result_type(const DataType& lhs, 
                                bool &l_need_cast,
                                const DataType& rhs, 
                                bool &r_need_cast,
                                DataType &dst_type)
{
    u32 ret = SUCCESS;
    Vector<DataType> src_types;
    Vector<bool> need_cast;
    src_types.push_back(lhs);
    src_types.push_back(rhs);
    CHECK(get_result_type(src_types, need_cast, dst_type));
    MY_ASSERT(2 == need_cast.size());
    l_need_cast = need_cast[0];
    r_need_cast = need_cast[1];
    return ret;
}

u32 ObjCastUtil::get_result_type(const Vector<DataType>& src_types,
                                Vector<bool>& need_cast, 
                                DataType& dst_type)
{
    u32 ret = SUCCESS;
    bool has_number = false;
    bool has_varchar = false;
    bool has_datetime = false;
    TimeType time_type;
    for (u32 i = 0; i < src_types.size(); ++i) {
        if (src_types[i].is_number()) {
            has_number = true;
        }
        if (src_types[i].is_varchar()) {
            has_varchar = true;
        }
        if (src_types[i].is_datetime()) {
            if (!has_datetime) {
                time_type = src_types[i].time_type;
            } else if (time_type < src_types[i].time_type) {
                time_type = src_types[i].time_type;
            }
            has_datetime = true;
        }
        if (src_types[i].is_bool()) {
            ret = INVALID_CAST;
            return ret;
        }
    }
    if (has_number && has_datetime) {
        ret = INVALID_CAST;
        return ret;
    }
    for (u32 i = 0; i < src_types.size(); ++i) {
        need_cast.push_back(false);
        if (src_types[i].is_varchar()) {
            if (has_number) {
                need_cast[i] = true;
            } else if (has_datetime) {
                need_cast[i] = true;
            }
        }
    }
    if (has_number) {
        dst_type = DataType::default_number_type();
    } else if (has_datetime) {
        dst_type = DataType::default_datetime_type(time_type);
    } else {
        dst_type = DataType::default_varchar_type();
    }
    if (FAIL(ret)) {
        LOG_ERR("can not get result type", K(src_types), K(ret));
    }
    return ret;
}

/**
 * cmp type     number      datetime    varchar
 * number       number      invalid     invalid
 * datetime     invalid     datetime    invalid 
 * varchar      invalid     invalid     varchar
 */
u32 ObjCastUtil::check_compare_type(const DataType& lhs, 
                                    bool &l_need_cast,
                                    DataType &l_dst_type,
                                    const DataType& rhs, 
                                    bool &r_need_cast,
                                    DataType &r_dst_type,
                                    DataType &dst_type)
{
    u32 ret = SUCCESS;
    l_need_cast = false;
    r_need_cast = false;
    if (lhs.is_bool() || rhs.is_bool()) {
        ret = INVALID_CAST;
    } else if (lhs.res_type != rhs.res_type) {
        if (lhs.is_varchar()) {
            l_need_cast = true;
            l_dst_type = rhs;
        } else if (rhs.is_varchar()) {
            r_need_cast = true;
            r_dst_type = lhs;
        } else {
            ret = INVALID_CAST;
        }
    }
    dst_type = DataType::default_bool_type();
    if (FAIL(ret)) {
        LOG_ERR("can not compare", K(lhs), K(rhs), K(ret));
    }
    return ret;
}

u32 ObjCastUtil::check_calc_type(const DataType& lhs, 
                                bool &l_need_cast,
                                DataType &l_dst_type,
                                const DataType& rhs, 
                                bool &r_need_cast,
                                DataType &r_dst_type,
                                DataType &dst_type)
{
    u32 ret = SUCCESS;
    l_need_cast = false;
    r_need_cast = false;
    if (lhs.is_number() && rhs.is_number()) {

    } else if (lhs.is_varchar() && rhs.is_number()) {
        l_need_cast = true;
        l_dst_type = DataType::default_number_type();
    } else if (lhs.is_number() && rhs.is_varchar()) {
        r_need_cast = true;
        r_dst_type = DataType::default_number_type();
    } else if (lhs.is_varchar() && rhs.is_varchar()) {
        l_need_cast = true;
        l_dst_type = DataType::default_number_type();
        r_need_cast = true;
        r_dst_type = DataType::default_number_type();
    } else {
        ret = INVALID_CAST;
    }
    dst_type = DataType::default_number_type();
    if (FAIL(ret)) {
        LOG_ERR("can not calc", K(lhs), K(rhs), K(ret));
    }
    return ret;
}

u32 ObjCastUtil::check_logical_type(const DataType& lhs,
                                    DataType &dst_type)
{
    u32 ret = SUCCESS;
    if (!lhs.is_bool()) {
        ret = INVALID_CAST;
    }
    dst_type = DataType::default_bool_type();
    if (FAIL(ret)) {
        LOG_ERR("can not calc", K(lhs), K(ret));
    }
    return ret;
}

u32 ObjCastUtil::get_result_type(const DataType& lhs, 
                                bool &l_need_cast,
                                DataType &l_dst_type,
                                const DataType& rhs, 
                                bool &r_need_cast,
                                DataType &r_dst_type,
                                OperationType op_type,
                                DataType &dst_type)
{
    u32 ret = SUCCESS;
    switch (op_type) {
    case OP_MINUS:
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV:
        CHECK(check_calc_type(lhs, 
                            l_need_cast, 
                            l_dst_type, 
                            rhs, 
                            r_need_cast, 
                            r_dst_type, 
                            dst_type));
        break;
    case OP_EQ:
    case OP_NE:
    case OP_GE:
    case OP_GT:
    case OP_LE:
    case OP_LT:
    case OP_EQ_ANY:
    case OP_NE_ANY:
    case OP_GE_ANY:
    case OP_GT_ANY:
    case OP_LE_ANY:
    case OP_LT_ANY:
    case OP_EQ_ALL:
    case OP_NE_ALL:
    case OP_GE_ALL:
    case OP_GT_ALL:
    case OP_LE_ALL:
    case OP_LT_ALL:
    case OP_BETWEEN:
    case OP_NOT_BETWEEN:
    case OP_IN:
    case OP_NOT_IN:
    case OP_IN_LIST:
    case OP_NOT_IN_LIST:
        CHECK(check_compare_type(lhs, 
                                 l_need_cast, 
                                 l_dst_type, 
                                 rhs, 
                                 r_need_cast, 
                                 r_dst_type, 
                                 dst_type));
        break;
    case OP_IS_NULL:
    case OP_IS_NOT_NULL:
    case OP_EXISTS:
    case OP_NOT_EXISTS:
        dst_type = DataType::default_bool_type();
        break;
    case OP_NOT:
        CHECK(check_logical_type(lhs, dst_type));
        break;
    default:
        ret = INVALID_CAST;
    }
    if (FAIL(ret)) {
        LOG_ERR("can not get result type", K(lhs), K(rhs), K(ret));
    }
    return ret;
}

u32 ObjCastUtil::add_cast(ExprStmt_s& from_expr, 
                          const DataType& to_type, 
                          ExprStmt_s &to_expr)
{
    u32 ret = SUCCESS;
    if (from_expr->has_flag(IS_LIST)) {
        for (u32 i = 0; i < from_expr->params.size(); ++i) {
            CHECK(add_cast(from_expr->params[i], to_type, from_expr->params[i]));
        }
        to_expr = from_expr;
    } else if (from_expr->has_flag(IS_SUBQUERY)) {
        SubQueryStmt_s expr = from_expr;
        for (u32 i = 0; i < expr->query_stmt->select_expr_list.size(); ++i) {
            CHECK(add_cast(expr->query_stmt->select_expr_list[i], 
                           to_type, 
                           expr->query_stmt->select_expr_list[i]));
        }
        to_expr = from_expr;
    } else {
        CHECK(inner_add_cast(from_expr, to_type, to_expr));
    }
    return ret;
}

u32 ObjCastUtil::inner_add_cast(ExprStmt_s& from_expr, 
                                const DataType& to_type, 
                                ExprStmt_s &to_expr)
{
    u32 ret = SUCCESS;
    OpExprStmt_s op_expr = OpExprStmt::make_op_expr_stmt(OP_CAST);
    Object_s value = Object::make_null_object(to_type);
    ExprStmt_s const_expr = ConstStmt::make_const_stmt(value);
    const_expr->res_type = to_type;
    op_expr->add_param(from_expr);
    op_expr->add_param(const_expr);
    CHECK(op_expr->formalize());
    to_expr = op_expr;
    return ret;
}

u32 ObjCastUtil::deduce_set_expr_type(SetStmt_s &stmt)
{
    u32 ret = SUCCESS;
	bool l_need_cast = false;
	bool r_need_cast = false;
	DataType dst_type;
    Vector<ExprStmt_s> &select_expr_list = stmt->select_expr_list;
    Vector<ExprStmt_s> &l_select_expr_list = stmt->left_query->select_expr_list;
    Vector<ExprStmt_s> &r_select_expr_list = stmt->right_query->select_expr_list;
	for (u32 i = 0; i < select_expr_list.size(); ++i) {
		CHECK(get_result_type(l_select_expr_list[i]->res_type, 
                            l_need_cast,
                            r_select_expr_list[i]->res_type, 
                            r_need_cast,
                            dst_type));
		if (l_need_cast) {
			CHECK(add_cast(l_select_expr_list[i], 
                            dst_type,
                            l_select_expr_list[i]));
		}
		if (r_need_cast) {
			CHECK(add_cast(r_select_expr_list[i], 
                            dst_type,
                            r_select_expr_list[i]));
		}
		select_expr_list[i]->res_type = dst_type;
	}
	return ret;
}