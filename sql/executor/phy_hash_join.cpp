#include "phy_hash_join.h"
#include "phy_expression.h"
#include "hash_table.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyHashJoin::PhyHashJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:JoinPhyOperator(left_child, right_child),
	state(BUILD_HASH_TABLE),
	left_distinct_rows(1)
{
	hash_join_func[Inner][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[Inner][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table;
	hash_join_func[Inner][VISIT_BUCKET] = &PhyHashJoin::visit_bucket_inner;
	hash_join_func[Inner][VISIT_HASH_TABLE] = &PhyHashJoin::end_join;
	hash_join_func[Inner][OUTPUT_ROWS] = &PhyHashJoin::end_join;
	hash_join_func[Inner][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[LeftOuter][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[LeftOuter][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table;
	hash_join_func[LeftOuter][VISIT_BUCKET] = &PhyHashJoin::visit_bucket_left_outer;
	hash_join_func[LeftOuter][VISIT_HASH_TABLE] = &PhyHashJoin::visti_hash_table;
	hash_join_func[LeftOuter][OUTPUT_ROWS] = &PhyHashJoin::output_rows_left_outer;
	hash_join_func[LeftOuter][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[RightOuter][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[RightOuter][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table_right_outer;
	hash_join_func[RightOuter][VISIT_BUCKET] = &PhyHashJoin::visit_bucket_right_outer;
	hash_join_func[RightOuter][VISIT_HASH_TABLE] = &PhyHashJoin::end_join;
	hash_join_func[RightOuter][OUTPUT_ROWS] = &PhyHashJoin::end_join;
	hash_join_func[RightOuter][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[FullOuter][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[FullOuter][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table_right_outer;
	hash_join_func[FullOuter][VISIT_BUCKET] = &PhyHashJoin::visit_bucket_right_outer;
	hash_join_func[FullOuter][VISIT_HASH_TABLE] = &PhyHashJoin::visti_hash_table;
	hash_join_func[FullOuter][OUTPUT_ROWS] = &PhyHashJoin::output_rows_left_outer;
	hash_join_func[FullOuter][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[LeftSemi][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[LeftSemi][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table;
	hash_join_func[LeftSemi][VISIT_BUCKET] = &PhyHashJoin::visit_bucket_left_semi;
	hash_join_func[LeftSemi][VISIT_HASH_TABLE] = &PhyHashJoin::visti_hash_table;
	hash_join_func[LeftSemi][OUTPUT_ROWS] = &PhyHashJoin::output_rows_left_semi;
	hash_join_func[LeftSemi][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[RightSemi][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[RightSemi][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table_right_semi;
	hash_join_func[RightSemi][VISIT_BUCKET] = &PhyHashJoin::end_join;
	hash_join_func[RightSemi][VISIT_HASH_TABLE] = &PhyHashJoin::end_join;
	hash_join_func[RightSemi][OUTPUT_ROWS] = &PhyHashJoin::end_join;
	hash_join_func[RightSemi][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[LeftAnti][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[LeftAnti][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table;
	hash_join_func[LeftAnti][VISIT_BUCKET] = &PhyHashJoin::visit_bucket_left_anti;
	hash_join_func[LeftAnti][VISIT_HASH_TABLE] = &PhyHashJoin::visti_hash_table;
	hash_join_func[LeftAnti][OUTPUT_ROWS] = &PhyHashJoin::output_rows_left_anti;
	hash_join_func[LeftAnti][END_JOIN] = &PhyHashJoin::end_join;

	hash_join_func[RightAnti][BUILD_HASH_TABLE] = &PhyHashJoin::build_hash_table;
	hash_join_func[RightAnti][PROBE_HASH_TABLE] = &PhyHashJoin::probe_hash_table_right_anti;
	hash_join_func[RightAnti][VISIT_BUCKET] = &PhyHashJoin::end_join;
	hash_join_func[RightAnti][VISIT_HASH_TABLE] = &PhyHashJoin::end_join;
	hash_join_func[RightAnti][OUTPUT_ROWS] = &PhyHashJoin::end_join;
	hash_join_func[RightAnti][END_JOIN] = &PhyHashJoin::end_join;
}

PhyHashJoin::~PhyHashJoin()
{
}

PhyOperator_s PhyHashJoin::make_hash_join(const PhyOperator_s & left_child, 
	const PhyOperator_s & right_child,
	const Vector<Expression_s>& other_join_cond,
	const Vector<Expression_s> &hash_exprs,
	const Vector<Expression_s> &prob_exprs,
	double left_distinct_rows)
{
	PhyHashJoin* op = new PhyHashJoin(left_child, right_child);
	op->left_distinct_rows = left_distinct_rows;
	op->hash_exprs = hash_exprs;
	op->prob_exprs = prob_exprs;
	op->other_conditions = other_join_cond;
	op->hash_table = HashTable::make_hash_table(left_distinct_rows);
	op->hash_table->set_hash_exprs(hash_exprs);
	op->hash_table->set_probe_exprs(prob_exprs);
	op->hash_table->set_null_safe(false);
	return PhyOperator_s(op);
}

u32 PhyHashJoin::inner_open()
{
	u32 ret = SUCCESS;
	CHECK(left_child->open());
	CHECK(right_child->open());
	hash_table->set_exec_ctx(exec_ctx);
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
	hash_table->reset();
	state = BUILD_HASH_TABLE;
	return ret;
}

u32 PhyHashJoin::inner_get_next_row()
{
	u32 ret = SUCCESS;
	bool goto_next_state = true;
	while(goto_next_state) {
		CHECK((this->*hash_join_func[JoinPhyOperator::type][state])(goto_next_state));
	}
	return ret;
}

u32 PhyHashJoin::type() const
{
	return PhyOperator::HASH_JOIN;
}

// u32 PhyHashJoin::build_hash_table_for_right_na_anti()
// {
// 	u32 ret  = SUCCESS;
// 	Expression_s hash_expr = hash_exprs[0];
// 	Row_s row;
// 	while (SUCC(left_child->get_next_row(row))) {
// 		set_input_rows(row);
// 		CHECK(hash_expr->get_result(exec_ctx));
// 		if (exec_ctx->output_result->is_null()) {
// 			has_null_in_hash_table = true;
// 			break;
// 		}
// 		row = Row::deep_copy(row);
// 		CHECK(CHECK(hash_table->build(row)));
// 	}
// 	if (NO_MORE_ROWS == ret) {
// 		ret = SUCCESS;
// 	}
// 	return ret;
//}

u32 PhyHashJoin::build_hash_table(bool &goto_next_state)
{
	u32 ret = SUCCESS;
	Row_s row;
	while (SUCC(left_child->get_next_row(row))) {
		row = Row::deep_copy(row);
		CHECK(hash_table->build(row));
	}
	if (ret == NO_MORE_ROWS) {
		state = PROBE_HASH_TABLE;
		ret = SUCCESS;
	}
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::probe_hash_table(bool &goto_next_state)
{
	u32 ret = SUCCESS;
	bool hit = false;
	goto_next_state = false;
	while (SUCC(right_child->get_next_row(last_probe_row))) {
		CHECK(hash_table->probe_bucket(last_probe_row, iter, hit));
		if (hit) {
			state = VISIT_BUCKET;
			goto_next_state = true;
			return ret;
		}
	}
	if (NO_MORE_ROWS == ret) {
		state = VISIT_HASH_TABLE;
		goto_next_state = true;
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyHashJoin::probe_hash_table_right_outer(bool &goto_next_state)
{
	u32 ret = SUCCESS;
	bool hit = false;
	goto_next_state = false;
	while (SUCC(right_child->get_next_row(last_probe_row))) {
		CHECK(hash_table->probe_bucket(last_probe_row, iter, hit));
		if (hit) {
			state = VISIT_BUCKET;
			goto_next_state = true;
			return ret;
		} else {
			Row_s left_null_row;
			CHECK(left_child->make_const_row(left_null_row));
			set_input_rows(left_null_row, last_probe_row);
			return ret;
		}
	}
	if (NO_MORE_ROWS == ret) {
		state = VISIT_HASH_TABLE;
		ret= SUCCESS;
		goto_next_state = true;
	}
	return ret;
}

u32 PhyHashJoin::probe_hash_table_right_semi(bool &goto_next_state)
{
	u32 ret = SUCCESS;
	bool hit = false;
	Row_s row;
	while (SUCC(right_child->get_next_row(last_probe_row))) {
		CHECK(hash_table->probe_bucket(last_probe_row, iter, hit));
		if (hit) {
			while (iter.get_next_row(row)) {
				set_input_rows(row, last_probe_row);
				CHECK(expr_filter(other_conditions, exec_ctx));
				if (exec_ctx->bool_result) {
					set_input_rows(last_probe_row);
					goto_next_state = false;
					return ret;
				}
			}
		}
	}
	state = END_JOIN;
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::probe_hash_table_right_anti(bool &goto_next_state)
{
	u32 ret = SUCCESS;
	bool hit = false;
	Row_s row;
	while (SUCC(right_child->get_next_row(last_probe_row))) {
		CHECK(hash_table->probe_bucket(last_probe_row, iter, hit));
		if (hit) {
			if (other_conditions.empty()) {
				continue;
			}
			hit = false;
			while (iter.get_next_row(row)) {
				set_input_rows(row, last_probe_row);
				CHECK(expr_filter(other_conditions, exec_ctx));
				if (exec_ctx->bool_result) {
					hit = true;
					break;
				}
			}
			if (!hit) {
				set_input_rows(last_probe_row);
				goto_next_state = false;
				return ret;
			}
		} else {
			set_input_rows(last_probe_row);
			goto_next_state = false;
			return ret;
		}
	}
	state = END_JOIN;
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::visti_hash_table(bool &goto_next_state)
{
	int ret = SUCCESS;
	if (hash_table->get_next_bucket(iter)) {
		state = OUTPUT_ROWS;
		goto_next_state = true;
	} else {
		state = END_JOIN;
		goto_next_state = true;
	}
	return ret;
}

u32 PhyHashJoin::visit_bucket_inner(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	goto_next_state = false;
	while (iter.get_next_row(row)) {
		set_input_rows(row, last_probe_row);
		CHECK(expr_filter(other_conditions, exec_ctx));
		if (exec_ctx->bool_result) {
			return ret;
		}
	}
	state = PROBE_HASH_TABLE;
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::visit_bucket_left_outer(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	while (iter.get_next_row(row)) {
		set_input_rows(row, last_probe_row);
		CHECK(expr_filter(other_conditions, exec_ctx));
		if (exec_ctx->bool_result) {
			iter.mark_last_row();
			goto_next_state = false;
			return ret;
		}
	}
	state = PROBE_HASH_TABLE;
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::visit_bucket_right_outer(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	if (iter.get_next_row(row)) {
		set_input_rows(row, last_probe_row);
		CHECK(expr_filter(other_conditions, exec_ctx));
		if (exec_ctx->bool_result) {
			iter.mark_last_row();
		} else {
			Row_s left_null_row;
			CHECK(left_child->make_const_row(left_null_row));
			set_input_rows(left_null_row, last_probe_row);
		}
		goto_next_state = false;
	} else {
		state = PROBE_HASH_TABLE;
		goto_next_state = true;
	}
	return ret;
}

u32 PhyHashJoin::visit_bucket_left_semi(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	while (iter.get_next_row(row)) {
		set_input_rows(row, last_probe_row);
		CHECK(expr_filter(other_conditions, exec_ctx));
		if (exec_ctx->bool_result) {
			iter.mark_last_row();
		}
	}
	state = PROBE_HASH_TABLE;
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::visit_bucket_left_anti(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	while (iter.get_next_row(row)) {
		set_input_rows(row, last_probe_row);
		CHECK(expr_filter(other_conditions, exec_ctx));
		if (exec_ctx->bool_result) {
			iter.mark_last_row();
		}
	}
	state = PROBE_HASH_TABLE;
	goto_next_state = true;
	return ret;
}

u32 PhyHashJoin::output_rows_left_outer(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	if (iter.get_next_no_mark_row(row)) {
		Row_s right_null_row;
		CHECK(right_child->make_const_row(right_null_row));
		set_input_rows(row, right_null_row);
		goto_next_state = false;
	} else {
		state = VISIT_HASH_TABLE;
		goto_next_state = true;
	}
	return ret;
}

u32 PhyHashJoin::output_rows_left_semi(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	if (iter.get_next_mark_row(row)) {
		set_input_rows(row);
		goto_next_state = false;
	} else {
		state = VISIT_HASH_TABLE;
		goto_next_state = true;
	}
	return ret;
}

u32 PhyHashJoin::output_rows_left_anti(bool &goto_next_state)
{
	int ret = SUCCESS;
	Row_s row;
	if (iter.get_next_no_mark_row(row)) {
		set_input_rows(row);
		goto_next_state = false;
	} else {
		state = VISIT_HASH_TABLE;
		goto_next_state = true;
	}
	return ret;
}

u32 PhyHashJoin::end_join(bool &goto_next_state)
{
	goto_next_state = false;
	return NO_MORE_ROWS;
}