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
	AggregateExpCalculator(agg_funcs),
	distinct_rows(1)
{
	is_build_hash_table = false;
}

PhyHashGroup::~PhyHashGroup()
{
}

PhyOperator_s PhyHashGroup::make_hash_group(PhyOperator_s & child,
	const Vector<Expression_s>& group_exprs,
	const Vector<Expression_s>& agg_funcs,
	double distinct_rows)
{
	PhyHashGroup* op = new PhyHashGroup(child, agg_funcs);
	op->distinct_rows = distinct_rows;
	op->hash_table = HashTable::make_hash_table(distinct_rows);
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
	hash_table->reset();
	CHECK(child->reset());
	return ret;
}

u32 PhyHashGroup::inner_get_next_row()
{
	u32 ret = SUCCESS;
	if (!is_build_hash_table) {
		CHECK(build_hash_table());
	}
	HashTable::RowIterator iter;
	if (!hash_table->get_next_bucket(iter)) {
		ret = NO_MORE_ROWS;
	} else {
		Row_s row;
		CHECK(reset_agg_func());
		while (iter.get_next_row(row)) {
			CHECK(add_row_to_agg_func(exec_ctx, row));
		}
		CHECK(calc_aggr_func(exec_ctx, aggr_result_row));
		set_input_rows(row, aggr_result_row);
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
	} else {
		ret = SUCCESS;
	}
	is_build_hash_table = true;
	return ret;
}
