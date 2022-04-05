#include "phy_hash_group.h"
#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

PhyHashGroup::PhyHashGroup(PhyOperator_s & child)
	:SingleChildPhyOperator(child),
	start_new_group(false)
{
	is_build_hash_table = false;
}

PhyHashGroup::~PhyHashGroup()
{
}

PhyOperator_s PhyHashGroup::make_hash_group(PhyOperator_s & child,
	const Vector<Expression_s>& group_exprs,
	const Vector<Expression_s>& agg_funcs)
{
	PhyHashGroup* op = new PhyHashGroup(child);
	op->group_exprs = group_exprs;
	op->agg_funcs = agg_funcs;
	op->hash_table.set_hash_exprs(group_exprs);
	op->hash_table.set_probe_exprs(group_exprs);
	return PhyOperator_s(op);
}

u32 PhyHashGroup::inner_open()
{
	hash_table.set_exec_ctx(exec_ctx);
	init_agg_func();
	return child->open();
}

u32 PhyHashGroup::close()
{
	return child->close();
}

u32 PhyHashGroup::reset()
{
	reset_agg_func();
	is_build_hash_table = false;
	start_new_group = false;
	first_group_row.reset();
	hash_table.clear();
	return child->reset();
}

u32 PhyHashGroup::inner_get_next_row()
{
	u32 ret = SUCCESS;
	bool is_valid = false;
	Row_s row;
	if (!is_build_hash_table) {
		CHECK(build_hash_table());
	}
	reset_agg_func();
	if (start_new_group) {
		if (!first_group_row) {
			return ERR_UNEXPECTED;
		} else {
			CHECK(add_row_to_agg_func(first_group_row));
			start_new_group = false;
		}
	}
	while (SUCC(hash_table.get_next_row(row))) {
		//是否是同一group
		if (!first_group_row) {
			first_group_row = row;
		}
		CHECK(this->euqal(first_group_row, row, is_valid));
		if (!is_valid) {
			first_group_row.swap(row);
			start_new_group = true;
			set_input_rows(row);
			return SUCCESS;
		}
		CHECK(add_row_to_agg_func(row));
	}
	if (ret == NO_MORE_ROWS && first_group_row) {
		first_group_row.swap(row);
		first_group_row.reset();
		set_input_rows(row);
		ret = SUCCESS;
	}
	return ret;
}

u32 PhyHashGroup::type() const
{
	return PhyOperator::HASH_GROUP;
}

u32 PhyHashGroup::build_hash_table()
{
	Row_s row;
	u32 ret = SUCCESS;
	while (SUCC(child->get_next_row(row))){
		row = Row::deep_copy(row);
		CHECK(hash_table.build(row));
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	CHECK(hash_table.sort_bucket());
	is_build_hash_table = true;
	return ret;
}

u32 PhyHashGroup::euqal(const Row_s & lhs, const Row_s & rhs, bool &is_valid)
{
	u32 ret = SUCCESS;
	Object_s lhs_value;
	Object_s rhs_value;
	int res = 0;
	is_valid = true;
	for (u32 i = 0; i < group_exprs.size(); ++i) {
		exec_ctx->set_input_rows(lhs);
		CHECK(group_exprs[i]->get_result(exec_ctx));
		lhs_value = exec_ctx->output_result;
		exec_ctx->set_input_rows(rhs);
		CHECK(group_exprs[i]->get_result(exec_ctx));
		rhs_value = exec_ctx->output_result;
		if (lhs_value->is_null() && rhs_value->is_null()) {
			is_valid = true;
			break;
		}
		CHECK(lhs_value->compare(rhs_value, res));
		if (0 != res) {
			is_valid = false;
			break;
		}
	}
	return ret;
}

void PhyHashGroup::reset_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		agg_func->reset();
	}
}

void PhyHashGroup::init_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		agg_func->set_exec_ctx(exec_ctx);
	}
}

u32 PhyHashGroup::add_row_to_agg_func(Row_s & row)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		exec_ctx->set_input_rows(row);
		CHECK(agg_func->add_row(exec_ctx));
	}
	return ret;
}
