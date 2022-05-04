#include "phy_scalar_group.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

PhyScalarGroup::PhyScalarGroup(PhyOperator_s & child,
								Vector<Expression_s> &aggr_items)
	:SingleChildPhyOperator(child),
	AggregateExpCalculator(aggr_items),
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
	u32 ret = SUCCESS;
	CHECK(init_agg_func(exec_ctx));
	aggr_result_row = Row::make_row(agg_funcs.size());
	aggr_result_row->set_op_id(operator_id);
	CHECK(child->open());
	return ret;
}

u32 PhyScalarGroup::close()
{
	return child->close();
}

u32 PhyScalarGroup::reset()
{
	u32 ret = SUCCESS;
	CHECK(reset_agg_func());
	is_start = false;
	CHECK(child->reset());
	return ret;
}

u32 PhyScalarGroup::inner_get_next_row()
{
	u32 ret = SUCCESS;
	if (is_start) {
		return NO_MORE_ROWS;
	}
	Row_s row;
	while (SUCC(child->get_next_row(row))){
		CHECK(add_row_to_agg_func(exec_ctx, row));
	}
	is_start = true;
	if (NO_MORE_ROWS == ret) {
		CHECK(calc_aggr_func(exec_ctx, aggr_result_row));
		set_input_rows(aggr_result_row);
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyScalarGroup::type() const
{
	return PhyOperator::SCALAR_GROUP;
}