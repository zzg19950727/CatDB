#include "plan_filter.h"
#include "filter.h"
#include "error.h"
#include "log.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

PlanFilter::PlanFilter(PhyOperator_s& child, const Filter_s& filter)
	:SingleChildPhyOperator(child),
	filter(filter)
{
}

PhyOperator_s PlanFilter::make_plan_filter(PhyOperator_s& child, const Filter_s& filter)
{
	return PhyOperator_s(new PlanFilter(child, filter));
}

u32 PlanFilter::open()
{
	return child->open();
}

u32 PlanFilter::close()
{
	return child->close();
}

u32 PlanFilter::reset()
{
	return child->reset();
}

u32 PlanFilter::reopen(const Row_s & row)
{
	if (filter) {
		filter->reset(row);
	}
	return child->reopen(row);
}

u32 PlanFilter::get_next_row(Row_s & row)
{
	while (child->get_next_row(row) == SUCCESS) {
		if (filter) {
			if ((*filter)(row)) {
				return SUCCESS;
			}
			else {
				continue;
			}
		}
		else {
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 PlanFilter::type() const
{
	return PhyOperator::PLAN_FILTER;
}

u32 PlanFilter::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "subplan filter", "", output_rows, finished_time);
	return child->explain_operator(depth + 1, result);
}
