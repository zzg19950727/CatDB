#include "nested_loop_join.h"
#include "expression.h"
#include "filter.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

NestedLoopJoin::NestedLoopJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:JoinPhyOperator(left_child, right_child)
{

}

NestedLoopJoin::~NestedLoopJoin()
{
}

PhyOperator_s NestedLoopJoin::make_nested_loop_join(const PhyOperator_s & left_child,
	const PhyOperator_s & right_child,
	const Expression_s& join_cond)
{
	NestedLoopJoin* op = new NestedLoopJoin(left_child, right_child);
	op->set_join_condition(join_cond);
	return PhyOperator_s(op);
}

u32 NestedLoopJoin::set_join_condition(const Expression_s & join_cond)
{
	join_condition = Filter::make_filter(join_cond);
	return SUCCESS;
}

u32 NestedLoopJoin::open()
{
	u32 ret = left_child->open();
	if (ret == SUCCESS) {
		ret = right_child->open();
		if (ret == SUCCESS) {
			return cache_right_table();
			return SUCCESS;
		}
		else {
			return ret;
		}
	}
	else {
		return ret;
	}
}

u32 NestedLoopJoin::close()
{
	u32 ret1 = left_child->close();
	u32 ret2 = right_child->close();
	if (ret1 == SUCCESS && ret2 == SUCCESS) {
		return SUCCESS;
	}
	else {
		return ret2;
	}
}

u32 NestedLoopJoin::reset()
{
	u32 ret = left_child->reset();
	if (ret == SUCCESS) {
		ret = right_child->reset();
		if (ret == SUCCESS) {
			right_pos = 0;
			return SUCCESS;
		}
		else {
			return ret;
		}
	}
	else {
		return ret;
	}
}

u32 NestedLoopJoin::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return cache_right_table();
}

u32 NestedLoopJoin::get_next_row(Row_s & row)
{
	switch (JoinPhyOperator::type) {
	case JoinPhyOperator::Join:
		return join(row);
	case JoinPhyOperator::SemiJoin:
		return semi_join(row);
	case JoinPhyOperator::AntiJoin:
		return anti_join(row);
	case JoinPhyOperator::LeftOuterJoin:
		return left_outer_join(row);
	case JoinPhyOperator::RightOuterJoin:
		return right_outer_join(row);
	case JoinPhyOperator::FullOuterJoin:
		return full_outer_join(row);
	default:
		return NO_MORE_ROWS;
	}
}

u32 NestedLoopJoin::type() const
{
	return PhyOperator::NESTED_LOOP_JOIN;
}

u32 NestedLoopJoin::cache_right_table()
{
	u32 ret;
	Row_s row;
	right_pos = 0;
	right_tables.clear();
	while (ret=right_child->get_next_row(row) == SUCCESS) {
		right_tables.push_back(row);
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	else {
		return SUCCESS;
	}
}

u32 NestedLoopJoin::join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!left_row) {
		ret = left_child->get_next_row(left_row);
		if (ret != SUCCESS) {
			return ret;
		}
	}
	for (;;) {
		while (right_pos < right_tables.size()) {
			row = right_tables[right_pos++];
			Row_s new_row = RowAgent::make_agent_row(row, left_row);
			if (join_condition) {
				if ((*join_condition)(new_row)) {
					row = Row::join_row(row, left_row);
					return SUCCESS;
				}
				else {
					continue;
				}
			}
			else {
				row = Row::join_row(row, left_row);
				return SUCCESS;
			}
		}
		ret = left_child->get_next_row(left_row);
		if (ret == NO_MORE_ROWS) {
			break;
		}
		else if (ret == SUCCESS) {
			right_pos = 0;
			continue;
		}
		else {
			break;
		}
	}
	return ret;
}

u32 NestedLoopJoin::semi_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!left_row) {
		ret = left_child->get_next_row(left_row);
		if (ret != SUCCESS) {
			return ret;
		}
	}
	for (;;) {
		while (right_pos < right_tables.size()) {
			row = right_tables[right_pos++];
			Row_s new_row = RowAgent::make_agent_row(row, left_row);
			if (join_condition) {
				if ((*join_condition)(new_row)) {
					row = left_row;
					return SUCCESS;
				}
				else {
					continue;
				}
			}
			else {
				row = left_row;
				return SUCCESS;
			}
		}
		ret = left_child->get_next_row(left_row);
		if (ret == NO_MORE_ROWS) {
			break;
		}
		else if (ret == SUCCESS) {
			right_pos = 0;
			continue;
		}
		else {
			break;
		}
	}
	return ret;
}

u32 NestedLoopJoin::anti_join(Row_s & row)
{
	u32 ret = SUCCESS;
	for (;;) {
		right_pos = 0;
		ret = left_child->get_next_row(left_row);
		if (ret == NO_MORE_ROWS) {
			break;
		}
		else if (ret != SUCCESS) {
			break;
		}
		bool is_in = false;
		while (right_pos < right_tables.size()) {
			row = right_tables[right_pos++];
			Row_s new_row = RowAgent::make_agent_row(row, left_row);
			if (join_condition) {
				if ((*join_condition)(new_row)) {
					is_in = true;
					break;
				}
				else {
					continue;
				}
			}
			else {
				return ERR_EXPR_TYPE;
			}
		}
		if (!is_in) {
			row = left_row;
			return SUCCESS;
		}
	}
	return ret;
}

u32 NestedLoopJoin::left_outer_join(Row_s & row)
{
	return u32();
}

u32 NestedLoopJoin::right_outer_join(Row_s & row)
{
	return u32();
}

u32 NestedLoopJoin::full_outer_join(Row_s & row)
{
	return u32();
}
