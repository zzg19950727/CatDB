#include "hash_set.h"
#include "expression.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::HashUnion::HashUnion(PhyOperator_s& first_child, PhyOperator_s& second_child)
	:DoubleChildPhyOperator(first_child, second_child)
{
}

CatDB::Sql::HashUnion::~HashUnion()
{
}

PhyOperator_s CatDB::Sql::HashUnion::make_hash_union(PhyOperator_s & first_child, PhyOperator_s & second_child)
{
	return PhyOperator_s(new HashUnion(first_child, second_child));
}

u32 CatDB::Sql::HashUnion::open()
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

u32 CatDB::Sql::HashUnion::close()
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

u32 CatDB::Sql::HashUnion::reset()
{
	u32 ret1 = left_child->reset();
	u32 ret2 = right_child->reset();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashUnion::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	hash_table.clear();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashUnion::get_next_row(Row_s & row)
{
	while (right_child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS)
			continue;
		hash_table.build(row);
		return SUCCESS;
	}
	while (left_child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS)
			continue;
		hash_table.build(row);
		return SUCCESS;
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::HashUnion::type() const
{
	return PhyOperator::HASH_SET;
}

u32 CatDB::Sql::HashUnion::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "hash union", "", output_rows, finished_time);
	left_child->explain_operator(depth + 1, result);
	return right_child->explain_operator(depth + 1, result);
}

CatDB::Sql::HashIntersect::HashIntersect(PhyOperator_s & first_child, PhyOperator_s & second_child)
	:DoubleChildPhyOperator(first_child, second_child)
{
}

CatDB::Sql::HashIntersect::~HashIntersect()
{
}

PhyOperator_s CatDB::Sql::HashIntersect::make_hash_intersect(PhyOperator_s & first_child, PhyOperator_s & second_child)
{
	return PhyOperator_s(new HashIntersect(first_child, second_child));
}

u32 CatDB::Sql::HashIntersect::open()
{
	u32 ret = right_child->open();
	if (ret == SUCCESS){
		build_hash_table();
		ret = left_child->open();
		if (ret == SUCCESS)
			return SUCCESS;
		else
			return ret;
	}else{
		return ret;
	}
}

u32 CatDB::Sql::HashIntersect::close()
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

u32 CatDB::Sql::HashIntersect::reset()
{
	u32 ret1 = left_child->reset();
	u32 ret2 = right_child->reset();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashIntersect::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	build_hash_table();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashIntersect::get_next_row(Row_s & row)
{
	while (left_child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS){
			if (hash_distinct.probe(row) == SUCCESS){
				continue;
			}else{
				hash_distinct.build(row);
				return SUCCESS;
			}
		}
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::HashIntersect::type() const
{
	return PhyOperator::HASH_SET;
}

u32 CatDB::Sql::HashIntersect::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "hash intersect", "", output_rows, finished_time);
	left_child->explain_operator(depth + 1, result);
	return right_child->explain_operator(depth + 1, result);
}

u32 CatDB::Sql::HashIntersect::build_hash_table()
{
	hash_table.clear();
	hash_distinct.clear();
	Row_s row;
	while (right_child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS)
			continue;
		else
			hash_table.build(row);
	}
	return SUCCESS;
}

CatDB::Sql::HashExcept::HashExcept(PhyOperator_s & first_child, PhyOperator_s & second_child)
	:DoubleChildPhyOperator(first_child, second_child)
{
}

CatDB::Sql::HashExcept::~HashExcept()
{
}

PhyOperator_s CatDB::Sql::HashExcept::make_hash_except(PhyOperator_s & first_child, PhyOperator_s & second_child)
{
	return PhyOperator_s(new HashExcept(first_child, second_child));
}

u32 CatDB::Sql::HashExcept::open()
{
	u32 ret = right_child->open();
	if (ret == SUCCESS){
		build_hash_table();
		ret = left_child->open();
		if (ret == SUCCESS)
			return SUCCESS;
		else
			return ret;
	}else{
		return ret;
	}
}

u32 CatDB::Sql::HashExcept::close()
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

u32 CatDB::Sql::HashExcept::reset()
{
	u32 ret1 = left_child->reset();
	u32 ret2 = right_child->reset();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashExcept::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	build_hash_table();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashExcept::get_next_row(Row_s & row)
{
	while (left_child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS){
			continue;
		}else{
			hash_table.build(row);
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::HashExcept::type() const
{
	return PhyOperator::HASH_SET;
}

u32 CatDB::Sql::HashExcept::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "hash except", "", output_rows, finished_time);
	left_child->explain_operator(depth + 1, result);
	return right_child->explain_operator(depth + 1, result);
}

u32 CatDB::Sql::HashExcept::build_hash_table()
{
	hash_table.clear();
	Row_s row;
	while (right_child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS)
			continue;
		else
			hash_table.build(row);
	}
	return SUCCESS;
}

CatDB::Sql::UnionAll::UnionAll(PhyOperator_s & first_child, PhyOperator_s & second_child)
	:DoubleChildPhyOperator(first_child, second_child)
{
}

CatDB::Sql::UnionAll::~UnionAll()
{
}

PhyOperator_s CatDB::Sql::UnionAll::make_union_all(PhyOperator_s & first_child, PhyOperator_s & second_child)
{
	return PhyOperator_s(new UnionAll(first_child, second_child));
}

u32 CatDB::Sql::UnionAll::open()
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

u32 CatDB::Sql::UnionAll::close()
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

u32 CatDB::Sql::UnionAll::reset()
{
	u32 ret1 = left_child->reset();
	u32 ret2 = right_child->reset();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::UnionAll::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 CatDB::Sql::UnionAll::get_next_row(Row_s & row)
{
	while (right_child->get_next_row(row) == SUCCESS){
		return SUCCESS;
	}
	while (left_child->get_next_row(row) == SUCCESS){
		return SUCCESS;
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::UnionAll::type() const
{
	return PhyOperator::HASH_SET;
}

u32 UnionAll::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "union all", "", output_rows, finished_time);
	left_child->explain_operator(depth + 1, result);
	return right_child->explain_operator(depth + 1, result);
}
