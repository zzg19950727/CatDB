#include "phy_hash_join.h"
#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyHashJoin::PhyHashJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:JoinPhyOperator(left_child, right_child),
	is_build_hash_table(false),
	has_null_in_hash_table(false),
	prepare_output_hash_table(true)
{

}

PhyHashJoin::~PhyHashJoin()
{
}

PhyOperator_s PhyHashJoin::make_hash_join(const PhyOperator_s & left_child, 
	const PhyOperator_s & right_child,
	const Vector<Expression_s>& other_join_cond,
	const Vector<Expression_s> &hash_exprs,
	const Vector<Expression_s> &prob_exprs)
{
	PhyHashJoin* op = new PhyHashJoin(left_child, right_child);
	op->hash_exprs = hash_exprs;
	op->prob_exprs = prob_exprs;
	op->hash_table.set_hash_exprs(hash_exprs);
	op->hash_table.set_probe_exprs(prob_exprs);
	op->hash_table.set_other_condition(other_join_cond);
	return PhyOperator_s(op);
}

u32 PhyHashJoin::inner_open()
{
	u32 ret = SUCCESS;
	CHECK(left_child->open());
	CHECK(right_child->open());
	hash_table.set_exec_ctx(exec_ctx);
	return ret;
}

u32 PhyHashJoin::close()
{
	u32 ret1 = left_child->close();
	u32 ret2 = right_child->close();
	if (ret1 == SUCCESS && ret2 == SUCCESS){
		return SUCCESS;
	}else{
		return ret2;
	}
}

u32 PhyHashJoin::reset()
{
	u32 ret = SUCCESS;
	CHECK(left_child->reset());
	CHECK(right_child->reset());
	last_probe_row.reset();
	hash_table.clear();
	is_build_hash_table = false;
	has_null_in_hash_table = false;
	prepare_output_hash_table = true;
	return ret;
}

u32 PhyHashJoin::inner_get_next_row()
{
	u32 ret = SUCCESS;
	if (!is_build_hash_table) {
		CHECK(build_hash_table());
	}
	switch (JoinPhyOperator::type) {
	case Inner:
		return join();
	case LeftSemi:
		return left_semi_join();
	case RightSemi:
		return right_semi_join();
	case LeftAnti:
		return left_anti_join();
	case RightAnti:
		return right_anti_join();
	case LeftOuter:
		return left_outer_join();
	case RightOuter:
		return right_outer_join();
	case FullOuter:
		return full_outer_join();
	default:
		return NO_MORE_ROWS;
	}
}

u32 PhyHashJoin::type() const
{
	return PhyOperator::HASH_JOIN;
}

u32 PhyHashJoin::build_hash_table()
{
	u32 ret = SUCCESS;
	Row_s row;
	while (SUCC(left_child->get_next_row(row))) {
		row = Row::deep_copy(row);
		hash_table.build(row);
	}
	if (ret == NO_MORE_ROWS) {
		ret = SUCCESS;
	}
	is_build_hash_table = true;
	return ret;
}

u32 PhyHashJoin::build_hash_table_for_left_na_anti()
{
	u32 ret = SUCCESS;
	if (hash_exprs.size() != 1 || prob_exprs.size() != 1) {
		LOG_ERR("hash anti join only support one join condition", K(ret));
		return OPERATION_NOT_SUPPORT;
	}
	Expression_s hash_expr = hash_exprs[0];
	Expression_s prob_expr = prob_exprs[0];
	Row_s row;
	bool right_is_empty = true;
	while (SUCC(right_child->get_next_row(row))) {
		right_is_empty = false;
		exec_ctx->set_input_rows(row);
		CHECK(prob_expr->get_result(exec_ctx));
		if (exec_ctx->output_result->is_null()) {
			has_null_in_hash_table = true;
			break;
		}
	}
	if (ret == NO_MORE_ROWS) {
		ret = SUCCESS;
		right_child->reset();
	}
	if (!has_null_in_hash_table) {
		while (SUCC(ret) && SUCC(left_child->get_next_row(row))) {
			exec_ctx->set_input_rows(row);
			CHECK(hash_expr->get_result(exec_ctx));
			if (!exec_ctx->output_result->is_null() || right_is_empty) {
				row = Row::deep_copy(row);
				CHECK(hash_table.build(row));
			}
		}
		if (ret == NO_MORE_ROWS) {
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 PhyHashJoin::build_hash_table_for_right_na_anti()
{
	if (hash_exprs.size() != 1) {
		LOG_ERR("hash anti join only support one join condition");
		return OPERATION_NOT_SUPPORT;
	}
	u32 ret  = SUCCESS;
	Expression_s hash_expr = hash_exprs[0];
	Row_s row;
	while (SUCC(left_child->get_next_row(row))) {
		exec_ctx->set_input_rows(row);
		CHECK(hash_expr->get_result(exec_ctx));
		if (exec_ctx->output_result->is_null()) {
			has_null_in_hash_table = true;
			break;
		}
		row = Row::deep_copy(row);
		CHECK(hash_table.build(row));
	}
	if (NO_MORE_ROWS == ret) {
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyHashJoin::join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		set_input_rows(left_row, last_probe_row);
		return ret;
	}
	while (SUCC(right_child->get_next_row(row))) {
		if (SUCC(hash_table.probe_all_rows(row, probe_result))) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			set_input_rows(left_row, last_probe_row);
			return ret;
		} else if (ROW_NOT_FOUND != ret) {
			return ret;
		} else {
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 PhyHashJoin::left_semi_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (!probe_result.empty()) {
		row = probe_result.front();
		probe_result.pop();
		return SUCCESS;
	}
	while (SUCC(right_child->get_next_row(row))) {
		if (SUCC(hash_table.probe_all_rows(row, probe_result, true))) {
			row = probe_result.front();
			probe_result.pop();
			set_input_rows(row);
			return ret;
		} else if (ROW_NOT_FOUND != ret) {
			return ret;
		} else {
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 PhyHashJoin::right_semi_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	while (SUCC(right_child->get_next_row(row))) {
		if (SUCC(hash_table.probe(row))) {
			set_input_rows(row);
			return ret;
		} else if (ROW_NOT_FOUND != ret) {
			return ret;
		} else {
			ret = SUCCESS;
		}
	}
	return ret;
}

u32 PhyHashJoin::left_anti_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (has_null_in_hash_table) {
		return NO_MORE_ROWS;
	} else if (prepare_output_hash_table) {
		while (SUCC(right_child->get_next_row(row))) {
			if (SUCC(hash_table.probe_all_rows(row))) {

			} else if (ROW_NOT_FOUND != ret) {
				return ret;
			} else {
				ret = SUCCESS;
			}
		}
		if (ret == NO_MORE_ROWS) {
			prepare_output_hash_table = false;
		} else {
			return ret;
		}
	}
	CHECK(hash_table.get_next_row(row, true));
	set_input_rows(row);
	return ret;
}

u32 PhyHashJoin::right_anti_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (has_null_in_hash_table) {
		return NO_MORE_ROWS;
	}
	Expression_s prob_expr = prob_exprs[0];
	while (SUCC(right_child->get_next_row(row))) {
		if (hash_table.empty()) {
			set_input_rows(row);
			return SUCCESS;
		}
		exec_ctx->set_input_rows(row);
		CHECK(prob_expr->get_result(exec_ctx));
		if (exec_ctx->output_result->is_null()) {
			continue;
		} else {
			ret = hash_table.probe(row);
			if (ROW_NOT_FOUND == ret) {
				set_input_rows(row);
				ret = SUCCESS;
			}
			return ret;
		}
	}
	return ret;
}

u32 PhyHashJoin::left_outer_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		set_input_rows(left_row, last_probe_row);
		return ret;
	}
	while (SUCC(right_child->get_next_row(row))) {
		if (SUCC(hash_table.probe_all_rows(row, probe_result))) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			set_input_rows(left_row, last_probe_row);
			return ret;
		} else if (ROW_NOT_FOUND != ret) {
			return ret;
		} else {
			ret = SUCCESS;
		}
	}
	if (NO_MORE_ROWS == ret) {
		CHECK(hash_table.get_next_row(row, true));
		Row_s right_row;
		CHECK(right_child->make_const_row(outer_const_value, right_row));
		set_input_rows(row, right_row);
	}
	return ret;
}

u32 PhyHashJoin::right_outer_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		set_input_rows(left_row, last_probe_row);
		return ret;
	}
	while (SUCC(right_child->get_next_row(row))) {
		ret = hash_table.probe_all_rows(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			set_input_rows(left_row, last_probe_row);
		} else if (ROW_NOT_FOUND == ret) {
			Row_s left_row;
			CHECK(left_child->make_const_row(outer_const_value, left_row));
			set_input_rows(left_row, row);
		}
		return ret;
	}
	return ret;
}

u32 PhyHashJoin::full_outer_join()
{
	u32 ret = SUCCESS;
	Row_s row;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		set_input_rows(left_row, last_probe_row);
		return ret;
	}
	while (SUCC(right_child->get_next_row(row))) {
		ret = hash_table.probe_all_rows(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			set_input_rows(left_row, last_probe_row);
		} else if (ROW_NOT_FOUND == ret) {
			Row_s left_row;
			CHECK(left_child->make_const_row(outer_const_value, left_row));
			set_input_rows(left_row, row);
		}
		return ret;
	}
	if (NO_MORE_ROWS == ret) {
		CHECK(hash_table.get_next_row(row, true));
		Row_s right_row;
		CHECK(right_child->make_const_row(outer_const_value, right_row));
		set_input_rows(row, right_row);
	}
	return ret;
}