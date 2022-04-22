#include "phy_nested_loop_join.h"
#include "phy_expression.h"
#include "object.h"
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
	op->join_condition = join_cond;
	return PhyOperator_s(op);
}

u32 PhyNestedLoopJoin::inner_open()
{
	u32 ret = SUCCESS;
	CHECK(left_child->open());
	CHECK(right_child->open());
	return ret;
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
	u32 ret = SUCCESS;
	CHECK(left_child->reset());
	CHECK(right_child->reset());
	right_pos = 0;
	is_start = false;
	right_cache.clear();
	return ret;
}

u32 PhyNestedLoopJoin::inner_get_next_row()
{
	u32 ret = SUCCESS;
	if (!is_start) {
		CHECK(cache_right_table());
		is_start = true;
	}
	switch (JoinPhyOperator::type) {
	case Inner:
		ret = inner_join();
		break;
	case LeftSemi:
		ret = left_semi_join();
		break;
	case LeftAnti:
		ret = left_anti_join();
		break;
	case LeftOuter:
		ret = left_outer_join();
		break;
	default:
		ret = OPERATION_NOT_SUPPORT;
	}
	return ret;
}

u32 PhyNestedLoopJoin::type() const
{
	return PhyOperator::NESTED_LOOP_JOIN;
}

u32 PhyNestedLoopJoin::cache_right_table()
{
	u32 ret = SUCCESS;
	Row_s row;
	right_pos = 0;
	right_cache.clear();
	while (SUCC(right_child->get_next_row(row))) {
		row = Row::deep_copy(row);
		right_cache.push_back(row);
		//如果是semi、anti笛卡尔积，只需要取一行数据
		if ((LeftSemi == JoinPhyOperator::type ||
			LeftAnti == JoinPhyOperator::type) &&
			join_condition.empty()) {
			break;
		}
	}
	if (ret == NO_MORE_ROWS) {
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyNestedLoopJoin::inner_join()
{
	u32 ret = SUCCESS;
	if (!left_row) {
		CHECK(left_child->get_next_row(left_row));
	}
	for (;;) {
		bool is_valid = false;
		while (right_pos < right_cache.size()) {
			Row_s row = right_cache[right_pos++];
			set_input_rows(left_row, row);
			CHECK(expr_filter(join_condition, exec_ctx));
			if (exec_ctx->bool_result) {
				return ret;
			}
		}
		CHECK(left_child->get_next_row(left_row));
		right_pos = 0;
	}
	return ret;
}

u32 PhyNestedLoopJoin::left_semi_join()
{
	u32 ret = SUCCESS;
	while (SUCC(left_child->get_next_row(left_row))) {
		right_pos = 0;
		while (right_pos < right_cache.size()) {
			Row_s row = right_cache[right_pos++];
			set_input_rows(left_row, row);
			CHECK(expr_filter(join_condition, exec_ctx));
			if (exec_ctx->bool_result) {
				return ret;
			}
		}
	}
	return ret;
}

u32 PhyNestedLoopJoin::left_anti_join()
{
	u32 ret = SUCCESS;
	while(SUCC(left_child->get_next_row(left_row))) {
		right_pos = 0;
		bool is_in = false;
		while (right_pos < right_cache.size()) {
			Row_s row = right_cache[right_pos++];
			set_input_rows(left_row, row);
			CHECK(expr_filter(join_condition, exec_ctx));
			if (exec_ctx->bool_result) {
				is_in = true;
				break;
			}
		}
		if (!is_in || right_cache.empty()) {
			set_input_rows(left_row);
			return ret;
		}
	}
	return ret;
}

u32 PhyNestedLoopJoin::left_outer_join()
{
	u32 ret = SUCCESS;
	if (!left_row) {
		CHECK(left_child->get_next_row(left_row));
	}
	for (;;) {
		bool not_match = right_pos == 0;
		while (right_pos < right_cache.size()) {
			Row_s row = right_cache[right_pos++];
			set_input_rows(left_row, row);
			CHECK(expr_filter(join_condition, exec_ctx));
			if (exec_ctx->bool_result) {
				return ret;
			}
		}
		if (not_match) {
			Row_s right_row;
			CHECK(right_child->make_const_row(outer_const_value, right_row));
			set_input_rows(left_row, right_row);
			left_row.reset();
			return ret;
		}
		CHECK(left_child->get_next_row(left_row));
		right_pos = 0;
	}
	return ret;
}