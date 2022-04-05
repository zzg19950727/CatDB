#include "select_stmt.h"
#include "obj_cast_util.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Parser;
using namespace CatDB::Common;

OrderStmt::OrderStmt()
	:asc(false)
{
}

OrderStmt::~OrderStmt()
{
}

OrderStmt_s OrderStmt::make_order_stmt(const ExprStmt_s& order_expr, bool asc)
{
	OrderStmt* stmt = new OrderStmt;
	stmt->order_expr = order_expr;
	stmt->asc = asc;
	return OrderStmt_s(stmt);
}

u32 OrderStmt::deep_copy(OrderStmt_s &order, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	ExprStmt_s copy_expr;
	CHECK(ExprUtils::deep_copy_expr(order_expr, copy_expr, ctx, flag));
	order = make_order_stmt(copy_expr, asc);
	return ret;
}

u32 OrderStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(order_expr->formalize());
	return ret;
}

LimitStmt::LimitStmt()
	:limit_offset(0),
	limit_size(0)
{
}

LimitStmt::~LimitStmt()
{
}

LimitStmt_s LimitStmt::make_limit_stmt(u32 size, u32 offset)
{
	LimitStmt* stmt = new LimitStmt;
	stmt->limit_offset = offset;
	stmt->limit_size = size;
	return LimitStmt_s(stmt);
}

u32 LimitStmt::formalize()
{
	return SUCCESS;
}

SelectStmt::SelectStmt()
	:DMLStmt(),
	is_distinct(false)
{
}

SelectStmt::~SelectStmt()
{
}

StmtType SelectStmt::stmt_type() const
{
	return Select;
}

Stmt_s SelectStmt::make_select_stmt()
{
	return Stmt_s(new SelectStmt());
}

u32 SelectStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::formalize());
	for (u32 i = 0; i < select_expr_list.size(); ++i) {
		CHECK(select_expr_list[i]->formalize());
	}
	for (u32 i = 0; i < group_exprs.size(); ++i) {
		CHECK(group_exprs[i]->formalize());
	}
	for (u32 i = 0; i < having_stmt.size(); ++i) {
		CHECK(having_stmt[i]->formalize());
	}
	for (u32 i = 0; i < order_exprs.size(); ++i) {
		CHECK(order_exprs[i]->formalize());
	}
	if (limit_stmt) {
		CHECK(limit_stmt->formalize());
	}
	return ret;
}

u32 SelectStmt::deep_copy(SelectStmt_s &stmt, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	stmt = make_select_stmt();
	CHECK(inner_deep_copy(stmt, ctx, flag));
	stmt->is_distinct = is_distinct;
	stmt->limit_stmt = limit_stmt;
	CHECK(ExprUtils::deep_copy_exprs(select_expr_list, stmt->select_expr_list, ctx, flag));
	CHECK(ExprUtils::deep_copy_exprs(group_exprs, stmt->group_exprs, ctx, flag));
	CHECK(ExprUtils::deep_copy_exprs(having_stmt, stmt->having_stmt, ctx, flag));
	OrderStmt_s copy_order;
	for (u32 i = 0; i < order_exprs.size(); ++i) {
		CHECK(order_exprs[i]->deep_copy(copy_order, ctx, flag));
		stmt->order_exprs.push_back(copy_order);
	}
	return ret;
}

bool SelectStmt::is_scalar_group_by() const
{
	if (group_exprs.size()) {
		return false;
	}
	for (u32 i = 0; i < select_expr_list.size(); ++i) {
		if (select_expr_list[i]->has_flag(HAS_AGG)) {
			return true;
		}
	}
	return false;
}

bool SelectStmt::has_group_by() const
{
	return group_exprs.size() || is_scalar_group_by();
}

u32 SelectStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_get_stmt_exprs(exprs));
	append(exprs, select_expr_list);
	append(exprs, group_exprs);
	append(exprs, having_stmt);
	for (u32 i = 0; i < order_exprs.size(); ++i) {
		exprs.push_back(order_exprs[i]->order_expr);
	}
	return ret;
}

u32 SelectStmt::inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   		 const Vector<ExprStmt_s> &new_exprs)
{
	u32 ret = SUCCESS;
	CHECK(DMLStmt::inner_replace_stmt_exprs(old_exprs, new_exprs));
	CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, select_expr_list));
	CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, group_exprs));
	CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, having_stmt));
	for (u32 i = 0; i < order_exprs.size(); ++i) {
		CHECK(ExprUtils::replace_expr(old_exprs, new_exprs, order_exprs[i]->order_expr));
	}
	return ret;
}

SetStmt::SetStmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op)
	:left_query(left_query),
	right_query(right_query),
	set_op(set_op)
{
}

SetStmt::~SetStmt()
{
}

StmtType SetStmt::stmt_type() const
{
	return SetOperation;
}

Stmt_s SetStmt::make_set_stmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op)
{
	SetStmt* stmt = new SetStmt(left_query, right_query, set_op);
	return Stmt_s(stmt);
}

u32 SetStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(left_query->formalize());
	CHECK(right_query->formalize());
	CHECK(SelectStmt::formalize());
	CHECK(deduce_type());
	return ret;
}

u32 SetStmt::deduce_type()
{
	u32 ret = SUCCESS;
	bool l_need_cast = false;
	bool r_need_cast = false;
	DataType dst_type;
	for (u32 i = 0; i < select_expr_list.size(); ++i) {
		CHECK(ObjCastUtil::get_result_type(left_query->select_expr_list[i]->res_type, 
										   l_need_cast,
										   right_query->select_expr_list[i]->res_type, 
										   r_need_cast,
										   dst_type));
		if (l_need_cast) {
			CHECK(ObjCastUtil::add_cast(left_query->select_expr_list[i], 
										dst_type,
										left_query->select_expr_list[i]));
		}
		if (r_need_cast) {
			CHECK(ObjCastUtil::add_cast(right_query->select_expr_list[i], 
										dst_type,
										right_query->select_expr_list[i]));
		}
		select_expr_list[i]->res_type = dst_type;
	}
	return ret;
}

u32 SetStmt::deep_copy(SelectStmt_s &stmt, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	SelectStmt_s copy_left_query;
	SelectStmt_s copy_right_query;
	CHECK(left_query->deep_copy(copy_left_query, ctx, flag));
	CHECK(right_query->deep_copy(copy_right_query, ctx, flag));
	stmt = make_set_stmt(copy_left_query, copy_right_query, set_op);
	CHECK(inner_deep_copy(stmt, ctx, flag));
	stmt->limit_stmt = limit_stmt;
	CHECK(ExprUtils::deep_copy_exprs(select_expr_list, stmt->select_expr_list, ctx, flag));
	OrderStmt_s copy_order;
	for (u32 i = 0; i < order_exprs.size(); ++i) {
		CHECK(order_exprs[i]->deep_copy(copy_order, ctx, flag));
		stmt->order_exprs.push_back(copy_order);
	}
	return ret;
}

u32 SetStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(SelectStmt::inner_get_stmt_exprs(exprs));
	CHECK(left_query->get_stmt_exprs(exprs));
	CHECK(right_query->get_stmt_exprs(exprs));
	return ret;
}

u32 SetStmt::inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   	  const Vector<ExprStmt_s> &new_exprs)
{
	u32 ret = SUCCESS;
	CHECK(SelectStmt::inner_replace_stmt_exprs(old_exprs, new_exprs));
	CHECK(left_query->replace_stmt_exprs(old_exprs, new_exprs));
	CHECK(right_query->replace_stmt_exprs(old_exprs, new_exprs));
	return ret;
}