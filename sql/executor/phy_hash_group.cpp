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
	reset();
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

u32 PhyHashGroup::inner_get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!is_build_hash_table) {
		ret = build_hash_table();
		if (FAIL(ret)) {
			return ret;
		}
	}
	reset_agg_func();
	if (start_new_group) {
		if (!first_group_row) {
			return ERR_UNEXPECTED;
		} else {
			add_row_to_agg_func(first_group_row);
			start_new_group = false;
		}
	}
	while ((ret = hash_table.get_next_row(row)) == SUCCESS) {
		//是否是同一group
		if (!first_group_row) {
			first_group_row = row;
		} else if (!this->euqal(first_group_row, row)) {
			first_group_row.swap(row);
			start_new_group = true;
			return SUCCESS;
		}
		add_row_to_agg_func(row);
	}
	if (ret == NO_MORE_ROWS && first_group_row) {
		first_group_row.swap(row);
		first_group_row.reset();
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
	while ((ret=child->get_next_row(row)) == SUCCESS){
		row = Row::deep_copy(row);
		hash_table.build(row);
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	ret = hash_table.sort_bucket();
	if (FAIL(ret)) {
		return ret;
	}
	is_build_hash_table = true;
	return ret;
}

bool PhyHashGroup::euqal(const Row_s & lhs, const Row_s & rhs)
{
	for (u32 i = 0; i < group_exprs.size(); ++i){
		Object_s lhs_value = group_exprs[i]->get_result(lhs);
		Object_s rhs_value = group_exprs[i]->get_result(rhs);
		if (lhs_value->is_null() && rhs_value->is_null()) {
			return true;
		}
		Object_s result = lhs_value->operator==(rhs_value);
		if (!result->bool_value()){
			return false;
		}
	}
	return true;
}

void PhyHashGroup::reset_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression_s agg_func = agg_funcs[i];
		agg_func->reset();
	}
}

u32 PhyHashGroup::add_row_to_agg_func(const Row_s & row)
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
