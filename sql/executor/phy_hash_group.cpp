#include "phy_hash_group.h"
#include "hash_table.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

PhyHashGroup::PhyHashGroup(PhyOperator_s & child,
						const Vector<Expression_s>& agg_funcs)
	:SingleChildPhyOperator(child),
	AggregateExpCalculator(agg_funcs)
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
	PhyHashGroup* op = new PhyHashGroup(child, agg_funcs);
	op->group_exprs = group_exprs;
	op->hash_table = HashTable::make_hash_table();
	op->hash_table->set_hash_exprs(group_exprs);
	op->hash_table->set_probe_exprs(group_exprs);
	return PhyOperator_s(op);
}

u32 PhyHashGroup::inner_open()
{
	u32 ret = SUCCESS;
	hash_table->set_exec_ctx(exec_ctx);
	CHECK(init_agg_func(exec_ctx));
	aggr_result_row = Row::make_row(agg_funcs.size());
	aggr_result_row->set_op_id(operator_id);
	CHECK(child->open());
	return ret;
}

u32 PhyHashGroup::close()
{
	return child->close();
}

u32 PhyHashGroup::reset()
{
	u32 ret = SUCCESS;
	CHECK(reset_agg_func());
	is_build_hash_table = false;
	group_first_row.reset();
	hash_table->clear();
	CHECK(child->reset());
	return ret;
}

u32 PhyHashGroup::inner_get_next_row()
{
	u32 ret = SUCCESS;
	bool is_valid = false;
	Row_s row;
	if (!is_build_hash_table) {
		CHECK(build_hash_table());
	}
	while (SUCC(hash_table->get_next_row(row))) {
		//是否是同一group
		is_valid = false;
		if (!group_first_row) {
			group_first_row = row;
			is_valid = true;
		} else {
			CHECK(this->euqal(group_first_row, row, is_valid));
		}
		//new group
		if (!is_valid) {
			//calc aggr expr
			group_first_row.swap(row);
			CHECK(calc_aggr_func(exec_ctx, aggr_result_row));
			//prepare next group
			CHECK(reset_agg_func());
			CHECK(add_row_to_agg_func(exec_ctx, group_first_row));
			set_input_rows(row, aggr_result_row);
			return ret;
		} else {
			CHECK(add_row_to_agg_func(exec_ctx, row));
		}
	}
	//last group
	if (ret == NO_MORE_ROWS && group_first_row) {
		group_first_row.swap(row);
		group_first_row.reset();
		CHECK(calc_aggr_func(exec_ctx, aggr_result_row));
		set_input_rows(row, aggr_result_row);
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
		CHECK(hash_table->build(row));
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	CHECK(hash_table->sort_bucket());
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
		set_input_rows(lhs);
		CHECK(group_exprs[i]->get_result(exec_ctx));
		lhs_value = exec_ctx->output_result;
		set_input_rows(rhs);
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