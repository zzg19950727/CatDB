#include "phy_hash_set.h"
#include "phy_expression.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::PhyHashSetOp::PhyHashSetOp(PhyOperator_s& first_child,
								 PhyOperator_s& second_child,
								 SetOpType type)
	:DoubleChildPhyOperator(first_child, second_child),
	set_type(type)
{
	init_hash_table = false;
}

CatDB::Sql::PhyHashSetOp::~PhyHashSetOp()
{
}

PhyOperator_s CatDB::Sql::PhyHashSetOp::make_hash_setop(PhyOperator_s & first_child,
													 PhyOperator_s & second_child,
													 SetOpType type)
{
	return PhyOperator_s(new PhyHashSetOp(first_child, second_child, type));
}

u32 CatDB::Sql::PhyHashSetOp::inner_open()
{
	u32 ret = right_child->open();
	if (ret == SUCCESS){
		ret = left_child->open();
		if (ret == SUCCESS)
			return SUCCESS;
		else
			return ret;
	}else{
		return ret;
	}
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
	hash_table.clear();
	init_hash_table = false;
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::PhyHashSetOp::inner_get_next_row(Row_s & row)
{
	switch(set_type) {
		case UNION_ALL:
			return get_next_row_union_all(row);
		case UNION:
			return get_next_row_union(row);
		case INTERSECT:
			return get_next_row_intersect(row);
		case EXCEPT:
			return get_next_row_except(row);
	}
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_union_all(Row_s &row)
{
	u32 ret = SUCCESS;
	while ((ret=left_child->get_next_row(row)) == SUCCESS) {
		return ret;
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		return ret;
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_union(Row_s &row)
{
	u32 ret = SUCCESS;
	while ((ret=left_child->get_next_row(row)) == SUCCESS) {
		if (hash_table.probe(row) == SUCCESS) {
			continue;
		} else {
			row = Row::deep_copy(row);
			return hash_table.build(row);
		}
	}
	if (ret != NO_MORE_ROWS) {
		return ret;
	}
	while ((ret=right_child->get_next_row(row)) == SUCCESS) {
		if (hash_table.probe(row) == SUCCESS) {
			continue;
		} else {
			row = Row::deep_copy(row);
			return hash_table.build(row);
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_intersect(Row_s &row)
{
	u32 ret = SUCCESS;
	if (!init_hash_table) {
		ret = build_hash_table();
		if (FAIL(ret)) {
			return ret;
		}
	}
	while ((ret=left_child->get_next_row(row)) == SUCCESS) {
		if (hash_table.probe_all_rows(row, true) == SUCCESS) {
			return SUCCESS;
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::get_next_row_except(Row_s &row)
{
	u32 ret = SUCCESS;
	if (!init_hash_table) {
		ret = build_hash_table();
		if (FAIL(ret)) {
			return ret;
		}
	}
	while ((ret=left_child->get_next_row(row)) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS) {
			continue;
		} else {
			row = Row::deep_copy(row);
			return hash_table.build(row);
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::build_hash_table()
{
	u32 ret = SUCCESS;
	Row_s row;
	while ((ret=right_child->get_next_row(row)) == SUCCESS){
		row = Row::deep_copy(row);
		hash_table.build(row);
	}
	init_hash_table = true;
	return ret;
}

u32 CatDB::Sql::PhyHashSetOp::type() const
{
	return PhyOperator::HASH_SET;
}