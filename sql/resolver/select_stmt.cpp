#include "select_stmt.h"
#include "expr_stmt.h"
using namespace::CatDB::Parser;

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

Stmt::StmtType SelectStmt::stmt_type() const
{
	return Stmt::Select;
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

SetStmt::SetStmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op)
	:left_query(left_query),
	right_query(right_query),
	set_op(set_op)
{
}

SetStmt::~SetStmt()
{
}

Stmt::StmtType SetStmt::stmt_type() const
{
	return Stmt::SetOperation;
}

Stmt_s SetStmt::make_set_stmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op)
{
	SetStmt* stmt = new SetStmt(left_query, right_query, set_op);
	return Stmt_s(stmt);
}

u32 SetStmt::formalize()
{
	u32 ret = SUCCESS;
	CHECK(SelectStmt::formalize());
	CHECK(left_query->formalize());
	CHECK(right_query->formalize());
	return ret;
}

u32 SetStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(SelectStmt::inner_get_stmt_exprs(exprs));
	return ret;
}