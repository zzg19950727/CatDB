#include "phy_nested_loop_join.h"
#include "phy_expression.h"
#include "object.h"
#include "phy_filter.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyNestedLoopJoin::PhyNestedLoopJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:JoinPhyOperator(left_child, right_child),
	is_start(false)
{

}

PhyNestedLoopJoin::~PhyNestedLoopJoin()
{
}

PhyOperator_s PhyNestedLoopJoin::make_nested_loop_join(const PhyOperator_s & left_child,
	const PhyOperator_s & right_child,
	const Vector<Expression_s>& join_cond)
{
	PhyNestedLoopJoin* op = new PhyNestedLoopJoin(left_child, right_child);
	op->set_join_condition(join_cond);
	return PhyOperator_s(op);
}

u32 PhyNestedLoopJoin::set_join_condition(const Vector<Expression_s> & join_cond)
{
	join_condition = PhyFilter::make_filter(join_cond);
	return SUCCESS;
}

u32 PhyNestedLoopJoin::inner_open()
{
	u32 ret = left_child->open();
	if (ret == SUCCESS) {
		return right_child->open();
	}
	else {
		return ret;
	}
}

u32 PhyNestedLoopJoin::close()
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

u32 PhyNestedLoopJoin::reset()
{
	u32 ret = left_child->reset();
	if (ret == SUCCESS) {
		ret = right_child->reset();
		if (ret == SUCCESS) {
			right_pos = 0;
			is_start = false;
			right_cache.clear();
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

u32 PhyNestedLoopJoin::inner_get_next_row(Row_s & row)
{
	if (!is_start) {
		u32 ret = cache_right_table();
		if (SUCC(ret)) {
			is_start = true;
		} else {
			return ret;
		}
	}
	switch (JoinPhyOperator::type) {
	case Inner:
		return inner_join(row);
	case LeftSemi:
		return left_semi_join(row);
	case LeftAnti:
		return left_anti_join(row);
	case LeftOuter:
		return left_outer_join(row);
	default:
		return OPERATION_NOT_SUPPORT;
	}
}

u32 PhyNestedLoopJoin::type() const
{
	return PhyOperator::NESTED_LOOP_JOIN;
}

u32 PhyNestedLoopJoin::cache_right_table()
{
	u32 ret;
	Row_s row;
	right_pos = 0;
	right_cache.clear();
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		row = Row::deep_copy(row);
		right_cache.push_back(row);
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	else {
		return SUCCESS;
	}
}

u32 PhyNestedLoopJoin::inner_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!left_row) {
		ret = left_child->get_next_row(left_row);
		if (ret != SUCCESS) {
			return ret;
		}
	}
	for (;;) {
		while (right_pos < right_cache.size()) {
			row = right_cache[right_pos++];
			Row_s new_row = RowAgent::make_agent_row(row, left_row);
			if (join_condition) {
				if ((*join_condition)(new_row)) {
					return make_join_row(left_row, row, row);
				}
				else {
					continue;
				}
			}
			else {
				return make_join_row(left_row, row, row);
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

u32 PhyNestedLoopJoin::left_semi_join(Row_s & row)
{
	u32 ret = SUCCESS;
	while (ret=left_child->get_next_row(left_row) == SUCCESS) {
		right_pos = 0;
		while (right_pos < right_cache.size()) {
			row = right_cache[right_pos++];
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
	}
	return ret;
}

u32 PhyNestedLoopJoin::left_anti_join(Row_s & row)
{
	u32 ret = SUCCESS;
	while((ret = left_child->get_next_row(left_row)) == SUCCESS) {
		right_pos = 0;
		bool is_in = false;
		while (right_pos < right_cache.size()) {
			row = right_cache[right_pos++];
			Row_s new_row = RowAgent::make_agent_row(row, left_row);
			if (join_condition) {
				if ((*join_condition)(new_row)) {
					is_in = true;
					break;
				} else {
					continue;
				}
			} else {
				is_in = true;
				break;
			}
		}
		if (!is_in || right_cache.empty()) {
			row = left_row;
			return SUCCESS;
		}
	}
	return ret;
}

u32 PhyNestedLoopJoin::left_outer_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!left_row) {
		ret = left_child->get_next_row(left_row);
		if (ret != SUCCESS) {
			return ret;
		}
	}
	for (;;) {
		bool not_match = right_pos == 0;
		while (right_pos < right_cache.size()) {
			row = right_cache[right_pos++];
			Row_s new_row = RowAgent::make_agent_row(row, left_row);
			if (join_condition) {
				if ((*join_condition)(new_row)) {
					return make_join_row(left_row, row, row);
				}
				else {
					continue;
				}
			}
			else {
				return make_join_row(left_row, row, row);
			}
		}
		if (not_match) {
			Row_s right_row;
			ret = right_child->make_const_row(outer_const_value, right_row);
			if (FAIL(ret)) {
				return ret;
			}
			ret = make_join_row(left_row, right_row, row);
			left_row.reset();
			return ret;
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