#include "phy_hash_set.h"
#include "phy_expression.h"
#include "hash_table.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::PhyHashSetOp::PhyHashSetOp(PhyOperator_s& first_child,
								 PhyOperator_s& second_child,
								 SetOpType type,
								 double distinct_rows)
	:DoubleChildPhyOperator(first_child, second_child),
	set_type(type),
	distinct_rows(distinct_rows)
{
	init_hash_table = false;
}

CatDB::Sql::PhyHashSetOp::~PhyHashSetOp()
{
}

PhyOperator_s CatDB::Sql::PhyHashSetOp::make_hash_setop(PhyOperator_s & first_child,
													 PhyOperator_s & second_child,
													 SetOpType type,
													 double distinct_rows)
{
	if (EXCEPT == type) {
		return PhyOperator_s(new PhyHashSetOp(second_child, 
											  first_child, 
											  type, 
											  distinct_rows));
	} else {
		return PhyOperator_s(new PhyHashSetOp(first_child, 
											  second_child, 
											  type, 
											  distinct_rows));
	}
}

u32 CatDB::Sql::PhyHashSetOp::inner_open()
{
	u32 ret = SUCCESS;
	CHECK(right_child->open());
	CHECK(left_child->open());
	hash_table = HashTable::make_hash_table(distinct_rows);
	hash_table->set_exec_ctx(exec_ctx);
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::close()
{
	u32 ret1 = left_child->close();
	u32 ret2 = right_child->close();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::PhyHashSetOp::reset()
{
	u32 ret1 = left_child->reset();
	u32 ret2 = right_child->reset();
	hash_table->reset();
	init_hash_table = false;
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::PhyHashSetOp::inner_get_next_row()
{
	u32 ret = SUCCESS;
	Row_s row;
	switch(set_type) {
		case UNION_ALL:
			ret = get_next_row_union_all(row);
			break;
		case UNION:
			ret = get_next_row_union(row);
			break;
		case INTERSECT:
			ret = get_next_row_intersect(row);
			break;
		case EXCEPT:
			ret = get_next_row_except(row);
			break;
	}
	set_input_rows(row);
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_union_all(Row_s &row)
{
	u32 ret = SUCCESS;
	while (SUCC(left_child->get_next_row(row))) {
		return ret;
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	while (SUCC(right_child->get_next_row(row))) {
		return ret;
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_union(Row_s &row)
{
	u32 ret = SUCCESS;
	bool hit = false;
	while (SUCC(left_child->get_next_row(row))) {
		CHECK(hash_table->probe(row, hit));
		if (!hit) {
			row = Row::deep_copy(row);
			CHECK(hash_table->build_without_check(row));
			return ret;
		}
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	while (SUCC(right_child->get_next_row(row))) {
		CHECK(hash_table->probe(row, hit));
		if (!hit) {
			row = Row::deep_copy(row);
			CHECK(hash_table->build_without_check(row));
			return ret;
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_intersect(Row_s &row)
{
	u32 ret = SUCCESS;
	if (!init_hash_table) {
		CHECK(build_hash_table());
	}
	bool hit = false;
	HashTable::RowIterator iter;
	while (SUCC(right_child->get_next_row(row))) {
		CHECK(hash_table->probe_bucket(row, iter, hit));
		if (hit) {
			if (iter.get_next_no_mark_row(row)) {
				iter.mark_last_row();
				break;
			}
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_except(Row_s &row)
{
	u32 ret = SUCCESS;
	if (!init_hash_table) {
		CHECK(build_hash_table());
	}
	bool hit = false;
	while (SUCC(right_child->get_next_row(row))) {
		CHECK(hash_table->probe(row, hit));
		if (!hit) {
			row = Row::deep_copy(row);
			CHECK(hash_table->build_without_check(row));
			return ret;
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::build_hash_table()
{
	u32 ret = SUCCESS;
	Row_s row;
	bool hit = false;
	while (SUCC(left_child->get_next_row(row))) {
		CHECK(hash_table->probe(row, hit));
		if (!hit) {
			row = Row::deep_copy(row);
			CHECK(hash_table->build_without_check(row));
		}
	}
	if (NO_MORE_ROWS == ret) {
		ret = SUCCESS;
	}
	init_hash_table = true;
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::type() const
{
	return PhyOperator::HASH_SET;
}