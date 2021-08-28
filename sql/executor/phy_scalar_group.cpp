#include "phy_scalar_group.h"
#include "phy_expression.h"
#include "phy_filter.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::PhyScalarGroup::PhyScalarGroup(PhyOperator_s & child,
										Vector<Expression_s> &aggr_items)
	:SingleChildPhyOperator(child),
	agg_funcs(aggr_items),
	is_start(false)
{
}

CatDB::Sql::PhyScalarGroup::~PhyScalarGroup()
{
}

PhyOperator_s CatDB::Sql::PhyScalarGroup::make_scalar_group(PhyOperator_s & child,
															Vector<Expression_s> &aggr_items)
{
	PhyScalarGroup* op = new PhyScalarGroup(child, aggr_items);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::PhyScalarGroup::inner_open()
{
	return child->open();
}

u32 CatDB::Sql::PhyScalarGroup::close()
{
	return child->close();
}

u32 CatDB::Sql::PhyScalarGroup::reset()
{
	reset_agg_func();
	is_start = false;
	return child->reset();
}

u32 CatDB::Sql::PhyScalarGroup::inner_get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	if (is_start) {
		return NO_MORE_ROWS;
	}
	while ((ret=child->get_next_row(row)) == SUCCESS){
		row = Row::deep_copy(row);
		add_row_to_agg_func(row);
	}
	if (NO_MORE_ROWS == ret) {
		return SUCCESS;
	} else {
		return ret;
	}
}

u32 CatDB::Sql::PhyScalarGroup::type() const
{
	return PhyOperator::SCALAR_GROUP;
}

void CatDB::Sql::PhyScalarGroup::reset_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		agg_func->reset();
	}
}

u32 CatDB::Sql::PhyScalarGroup::add_row_to_agg_func(const Row_s & row)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		ret = agg_func->add_row(row);
		if (ret != SUCCESS) {
			break;
		}
	}
	return ret;
}
