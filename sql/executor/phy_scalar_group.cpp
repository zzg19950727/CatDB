#include "phy_scalar_group.h"
#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

PhyScalarGroup::PhyScalarGroup(PhyOperator_s & child,
								Vector<Expression_s> &aggr_items)
	:SingleChildPhyOperator(child),
	agg_funcs(aggr_items),
	is_start(false)
{
}

PhyScalarGroup::~PhyScalarGroup()
{
}

PhyOperator_s PhyScalarGroup::make_scalar_group(PhyOperator_s & child,
												Vector<Expression_s> &aggr_items)
{
	PhyScalarGroup* op = new PhyScalarGroup(child, aggr_items);
	return PhyOperator_s(op);
}

u32 PhyScalarGroup::inner_open()
{
	init_agg_func();
	return child->open();
}

u32 PhyScalarGroup::close()
{
	return child->close();
}

u32 PhyScalarGroup::reset()
{
	reset_agg_func();
	is_start = false;
	return child->reset();
}

u32 PhyScalarGroup::inner_get_next_row()
{
	u32 ret = SUCCESS;
	if (is_start) {
		return NO_MORE_ROWS;
	}
	Row_s row;
	while (SUCC(child->get_next_row(row))){
		row = Row::deep_copy(row);
		CHECK(add_row_to_agg_func(row));
	}
	is_start = true;
	if (NO_MORE_ROWS == ret) {
		set_input_rows(row);
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyScalarGroup::type() const
{
	return PhyOperator::SCALAR_GROUP;
}

void PhyScalarGroup::reset_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		agg_func->reset();
	}
}

void PhyScalarGroup::init_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		agg_func->set_exec_ctx(exec_ctx);
	}
}

u32 PhyScalarGroup::add_row_to_agg_func(Row_s & row)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		exec_ctx->set_input_rows(row);
		CHECK(agg_func->add_row(exec_ctx));
	}
	return ret;
}
