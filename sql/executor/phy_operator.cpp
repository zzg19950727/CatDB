#include "phy_operator.h"
#include "phy_expression.h"
#include "phy_filter.h"
#include "query_ctx.h"
#include "error.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

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
	cur_row = Row::make_row(output_desc);
	return inner_open();
}

u32 PhyOperator::get_next_row(Row_s &row)
{
	u32 ret = SUCCESS;
	while ((ret=inner_get_next_row(row)) == SUCCESS) {
		CHECK(check_status());
		if (filter) {
			if ((*filter)(row)) {
				return make_row(row);
			} else {
				continue;
			}
		} else {
			return make_row(row);
		}
	}
	return ret;
}

void PhyOperator::set_output_exprs(u32 operator_id, const Vector<Expression_s> &exprs)
{
	output_exprs = exprs;
	for (u32 i = 0; i < exprs.size(); ++i) {
		ColumnDesc col_desc;
		col_desc.set_tid_cid(operator_id, i);
		output_desc.add_column_desc(col_desc);
	}
}

u32 PhyOperator::make_row(Row_s &row)
{
	u32 ret = SUCCESS;
	if (!cur_row || output_desc.get_column_num() != output_exprs.size()) {
		ret = ERR_UNEXPECTED;
		return ret;
	}
	for (u32 i = 0; i < output_exprs.size(); ++i) {
		Object_s cell = output_exprs[i]->get_result(row);
		cur_row->set_cell(i, cell);
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
	for (u32 i = 0; i < output_desc.get_column_num(); ++i) {
		Object_s cell = Object::make_null_object();
		cur_row->set_cell(i, cell);
	}
	row = cur_row;
	return ret;
}

u32 PhyOperator::set_filter(const PhyFilter_s& filter) 
{ 
	this->filter = filter; 
	return SUCCESS;
}

PhyFilter_s PhyOperator::get_filter()const 
{ 
	return filter; 
}

void PhyOperator::set_query_ctx(QueryCtx_s &ctx) 
{ 
	query_ctx = ctx; 
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

u32 JoinPhyOperator::make_join_row(const Row_s &left_row, const Row_s &right_row, Row_s &row)
{
	row = RowAgent::make_agent_row(left_row, right_row);
	return SUCCESS;
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