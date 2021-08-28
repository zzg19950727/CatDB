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
	op->hash_table.set_hash_exprs(hash_exprs);
	op->hash_table.set_probe_exprs(prob_exprs);
	op->hash_table.set_other_condition(other_join_cond);
	return PhyOperator_s(op);
}

u32 PhyHashJoin::inner_open()
{
	u32 ret = left_child->open();
	if (ret == SUCCESS){
		return right_child->open();
	}else{
		return ret;
	}
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
	u32 ret = left_child->reset();
	if (ret == SUCCESS){
		ret = right_child->reset();
		if (ret == SUCCESS){
			last_probe_row.reset();
			hash_table.clear();
			is_build_hash_table = false;
			has_null_in_hash_table = false;
			prepare_output_hash_table = true;
			return SUCCESS;
		}else{
			return ret;
		}
	}else{
		return ret;
	}
}

u32 PhyHashJoin::inner_get_next_row(Row_s & row)
{
	if (!is_build_hash_table) {
		u32 ret = build_hash_table();
		if (FAIL(ret)) {
			return ret;
		}
	}
	switch (JoinPhyOperator::type) {
	case Inner:
		return join(row);
	case LeftSemi:
		return left_semi_join(row);
	case RightSemi:
		return right_semi_join(row);
	case LeftAnti:
		return left_anti_join(row);
	case RightAnti:
		return right_anti_join(row);
	case LeftOuter:
		return left_outer_join(row);
	case RightOuter:
		return right_outer_join(row);
	case FullOuter:
		return full_outer_join(row);
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
	if (LeftAnti == JoinPhyOperator::type) {
		return build_hash_table_for_left_anti();
	} else if (RightSemi == JoinPhyOperator::type) {
		return build_hash_table_for_right_anti();
	} else {
		Row_s row;
		while ((ret=left_child->get_next_row(row)) == SUCCESS) {
			row = Row::deep_copy(row);
			hash_table.build(row);
		}
		if (ret == NO_MORE_ROWS) {
			ret = SUCCESS;
		}
	}
	is_build_hash_table = true;
	return ret;
}

u32 PhyHashJoin::build_hash_table_for_left_anti()
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
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		right_is_empty = false;
		if (prob_expr->get_result(row)->is_null()) {
			has_null_in_hash_table = true;
			break;
		}
	}
	if (ret == NO_MORE_ROWS) {
		ret = SUCCESS;
		right_child->reset();
	}
	if (!has_null_in_hash_table) {
		while (SUCC(ret) && (ret=left_child->get_next_row(row)) == SUCCESS) {
			if (!hash_expr->get_result(row)->is_null() || right_is_empty) {
				row = Row::deep_copy(row);
				ret = hash_table.build(row);
			}
		}
	}
	return ret;
}

u32 PhyHashJoin::build_hash_table_for_right_anti()
{
	if (hash_exprs.size() != 1) {
		LOG_ERR("hash anti join only support one join condition");
		return OPERATION_NOT_SUPPORT;
	}
	u32 ret  = SUCCESS;
	Expression_s hash_expr = hash_exprs[0];
	Row_s row;
	while ((ret=left_child->get_next_row(row)) == SUCCESS) {
		if (hash_expr->get_result(row)->is_null()) {
			has_null_in_hash_table = true;
			break;
		}
		row = Row::deep_copy(row);
		hash_table.build(row);
	}
	if (NO_MORE_ROWS == ret) {
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyHashJoin::join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		return make_join_row(left_row, last_probe_row, row);
	}
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		u32 ret = hash_table.probe_all_rows(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			return make_join_row(left_row, last_probe_row, row);
		}
	}
	return ret;
}

u32 PhyHashJoin::left_semi_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!probe_result.empty()) {
		row = probe_result.front();
		probe_result.pop();
		return SUCCESS;
	}
	while ((ret = right_child->get_next_row(row)) == SUCCESS) {
		ret = hash_table.probe_all_rows(row, probe_result, true);
		if (ret == SUCCESS) {
			row = probe_result.front();
			probe_result.pop();
			return SUCCESS;
		}
	}
	return ret;
}

u32 PhyHashJoin::right_semi_join(Row_s & row)
{
	u32 ret = SUCCESS;
	while ((ret = right_child->get_next_row(row)) == SUCCESS) {
		u32 ret = hash_table.probe(row);
		if (ret == SUCCESS) {
			return SUCCESS;
		}
	}
	return ret;
}

u32 PhyHashJoin::left_anti_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (has_null_in_hash_table) {
		return NO_MORE_ROWS;
	} else if (prepare_output_hash_table) {
		while ((ret = right_child->get_next_row(row)) == SUCCESS) {
			hash_table.probe_all_rows(row);
		}
		if (ret == NO_MORE_ROWS) {
			prepare_output_hash_table = false;
		} else {
			return ret;
		}
	}
	return hash_table.get_next_row(row, true);
}

u32 PhyHashJoin::right_anti_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (has_null_in_hash_table) {
		return NO_MORE_ROWS;
	}
	Expression_s prob_expr = prob_exprs[0];
	while ((ret = right_child->get_next_row(row)) == SUCCESS) {
		if (hash_table.empty()) {
			return SUCCESS;
		} else if (prob_expr->get_result(row)->is_null()) {
			continue;
		} else {
			u32 ret = hash_table.probe(row);
			if (ret != SUCCESS) {
				return SUCCESS;
			}
		}
	}
	return ret;
}

u32 PhyHashJoin::left_outer_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		return make_join_row(left_row, last_probe_row, row);
	}
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		u32 ret = hash_table.probe_all_rows(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			return make_join_row(left_row, last_probe_row, row);
		}
	}
	if (NO_MORE_ROWS == ret) {
		ret = hash_table.get_next_row(row, true);
		if (NO_MORE_ROWS == ret) {
			return ret;
		}
		Row_s right_row;
		ret = right_child->make_const_row(outer_const_value, right_row);
		if (SUCCESS != ret) {
			return ret;
		}
		return make_join_row(row, right_row, row);
	}
	return ret;
}

u32 PhyHashJoin::right_outer_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		return make_join_row(left_row, last_probe_row, row);
	}
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		u32 ret = hash_table.probe_all_rows(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			return make_join_row(left_row, last_probe_row, row);
		} else {
			Row_s left_row;
			ret = left_child->make_const_row(outer_const_value, left_row);
			if (SUCCESS != ret) {
				return ret;
			}
			return make_join_row(left_row, row, row);
		}
	}
	return ret;
}

u32 PhyHashJoin::full_outer_join(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		return make_join_row(left_row, last_probe_row, row);
	}
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		u32 ret = hash_table.probe_all_rows(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			return make_join_row(left_row, last_probe_row, row);
		} else {
			Row_s left_row;
			ret = left_child->make_const_row(outer_const_value, left_row);
			if (SUCCESS != ret) {
				return ret;
			}
			return make_join_row(left_row, row, row);
		}
	}
	if (NO_MORE_ROWS == ret) {
		ret = hash_table.get_next_row(row, true);
		if (NO_MORE_ROWS == ret) {
			return ret;
		}
		Row_s right_row;
		ret = right_child->make_const_row(outer_const_value, right_row);
		if (SUCCESS != ret) {
			return ret;
		}
		return make_join_row(row, right_row, row);
	}
	return ret;
}