#include "phy_operator.h"
#include "phy_expression.h"
#include "object.h"
#include "query_ctx.h"
#include "error.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

ExecCtx_s ExecCtx::make_exec_ctx()
{
	return ExecCtx_s(new ExecCtx);
}

u32 ExecCtx::set_input_rows(const Row_s &row)
{
	input_rows.clear();
	Row_s copy = row;
	input_rows.push_back(copy);
	return SUCCESS;
}

u32 ExecCtx::set_input_rows(const Row_s &row1, const Row_s &row2)
{
	input_rows.clear();
	input_rows.push_back(row1);
	input_rows.push_back(row2);
	return SUCCESS;
}

PhyOperator::PhyOperator()
	:query_ctx(NULL)
{
}

PhyOperator::~PhyOperator()
{
}

u32 PhyOperator::open()
{
	u32 ret = SUCCESS;
	cur_row = Row::make_row(output_exprs.size());
	cur_row->set_op_id(operator_id);
	return inner_open();
}

u32 PhyOperator::get_next_row(Row_s &row)
{
	u32 ret = SUCCESS;
	while ((ret=inner_get_next_row()) == SUCCESS) {
		CHECK(check_status());
		CHECK(expr_filter(filters, exec_ctx));
		if (!exec_ctx->bool_result) {
			continue;
		} else {
			return make_row(row);
		}
	}
	return ret;
}

void PhyOperator::set_output_exprs(const Vector<Expression_s> &exprs)
{
	output_exprs = exprs;
}

u32 PhyOperator::make_row(Row_s &row)
{
	u32 ret = SUCCESS;
	if (!cur_row) {
		ret = ERR_UNEXPECTED;
		return ret;
	}
	for (u32 i = 0; i < output_exprs.size(); ++i) {
		ColumnExpression_s col = output_exprs[i];
		CHECK(output_exprs[i]->get_result(exec_ctx));
		cur_row->set_cell(i, exec_ctx->output_result);
	}
	row = cur_row;
	return ret;
}

u32 PhyOperator::make_const_row(Object_s &const_value, Row_s &row)
{
	u32 ret = SUCCESS;
	if (!cur_row) {
		ret = ERR_UNEXPECTED;
		return ret;
	}
	for (u32 i = 0; i < output_exprs.size(); ++i) {
		Object_s cell = Object::make_null_object();
		cur_row->set_cell(i, cell);
	}
	row = cur_row;
	return ret;
}

u32 PhyOperator::set_filter(const Vector<Expression_s>& filter) 
{ 
	filters = filter; 
	return SUCCESS;
}

void PhyOperator::set_operator_id(u32 id)
{
	operator_id = id;
}

void PhyOperator::set_query_ctx(QueryCtx_s &ctx) 
{ 
	query_ctx = ctx; 
}

void PhyOperator::set_exec_ctx(ExecCtx_s &ctx)
{
	exec_ctx = ctx;
}

void PhyOperator::set_input_rows(const Row_s &row)
{
	exec_ctx->input_rows.clear();
	exec_ctx->input_rows.push_back(row);
}

void PhyOperator::set_input_rows(const Row_s &row1, const Row_s &row2)
{
	exec_ctx->input_rows.clear();
	exec_ctx->input_rows.push_back(row1);
	exec_ctx->input_rows.push_back(row2);
}

u32 PhyOperator::check_status()
{
	u32 ret = SUCCESS;
	if (!query_ctx) {
		ret = SUCCESS;
	} else {
		ret = query_ctx->check_query_status(); 
	}
	return ret;
}

void PhyOperator::increase_affected_rows()
{
	if (query_ctx) {
		query_ctx->increase_affected_rows(); 
	}
}

SingleChildPhyOperator::SingleChildPhyOperator(const PhyOperator_s & child)
	:child(child)
{
}

SingleChildPhyOperator::~SingleChildPhyOperator()
{
}

DoubleChildPhyOperator::DoubleChildPhyOperator(const PhyOperator_s & left, const PhyOperator_s & right)
	:left_child(left),
	right_child(right)
{
}

DoubleChildPhyOperator::~DoubleChildPhyOperator()
{
}

JoinPhyOperator::JoinPhyOperator(const PhyOperator_s & left, const PhyOperator_s & right)
	:DoubleChildPhyOperator(left, right),
	type(Inner)
{
}

JoinPhyOperator::~JoinPhyOperator()
{
}

JoinType JoinPhyOperator::join_type() const
{
	return type;
}

void JoinPhyOperator::set_join_type(JoinType type)
{
	this->type = type;
}

void JoinPhyOperator::set_outer_const_value(Object_s &value) 
{ 
	outer_const_value = value; 
}

MultiChildPhyOperator::MultiChildPhyOperator()
	:PhyOperator()
{
}

MultiChildPhyOperator::~MultiChildPhyOperator()
{
}

void MultiChildPhyOperator::add_child(const PhyOperator_s &child) 
{ 
	children.push_back(child); 
}

void MultiChildPhyOperator::add_children(const Vector<PhyOperator_s> &children) 
{ 
	append(this->children, children); 
}