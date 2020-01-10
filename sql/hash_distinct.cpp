#include "hash_distinct.h"
#include "expression.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::HashDistinct::HashDistinct(PhyOperator_s & child)
	:SingleChildPhyOperator(child)
{
}

CatDB::Sql::HashDistinct::~HashDistinct()
{
}

u32 CatDB::Sql::HashDistinct::set_distinct_columns(const Vector<Expression_s>& distinct_cols)
{
	hash_table.clear_hash_columns();
	hash_table.clear_probe_columns();
	for (u32 i = 0; i < distinct_cols.size(); ++i) {
		hash_table.add_hash_column(distinct_cols[i]);
		hash_table.add_probe_column(distinct_cols[i]);
	}
	return SUCCESS;
}

PhyOperator_s CatDB::Sql::HashDistinct::make_hash_distinct(PhyOperator_s & child)
{
	HashDistinct* op = new HashDistinct(child);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::HashDistinct::open()
{
	return child->open();
}

u32 CatDB::Sql::HashDistinct::close()
{
	return child->close();
}

u32 CatDB::Sql::HashDistinct::reset()
{
	hash_table.clear();
	return child->reset();
}

u32 CatDB::Sql::HashDistinct::reopen(const Row_s & row)
{
	hash_table.clear();
	return child->reopen(row);
}

u32 CatDB::Sql::HashDistinct::get_next_row(Row_s & row)
{
	while (child->get_next_row(row) == SUCCESS){
		if (hash_table.probe(row) == SUCCESS){
			continue;
		}else{
			hash_table.build(row);
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::HashDistinct::type() const
{
	return PhyOperator::HASH_DISTINCT;
}

u32 HashDistinct::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "hash distinct", "", output_rows, finished_time);
	return child->explain_operator(depth + 1, result);
}
