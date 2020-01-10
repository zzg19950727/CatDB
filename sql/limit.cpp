#include "limit.h"
#include "error.h"
#include "log.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::Limit::Limit(PhyOperator_s& child, u32 size, u32 offset)
	:SingleChildPhyOperator(child),
	size(size),
	offset(offset),
	cur_offset(0)
{
}

PhyOperator_s CatDB::Sql::Limit::make_limit(PhyOperator_s& child, u32 size, u32 offset)
{
	return PhyOperator_s(new Limit(child, size, offset));
}

u32 CatDB::Sql::Limit::open()
{
	return child->open();
}

u32 CatDB::Sql::Limit::close()
{
	return child->close();
}

u32 CatDB::Sql::Limit::reset()
{
	cur_offset = 0;
	return child->reset();
}

u32 CatDB::Sql::Limit::reopen(const Row_s & row)
{
	cur_offset = 0;
	return child->reopen(row);
}

u32 CatDB::Sql::Limit::get_next_row(Row_s & row)
{
	if ( cur_offset > offset && cur_offset - offset >= size){
		return NO_MORE_ROWS;
	}
	while (child->get_next_row(row) == SUCCESS){
		if (cur_offset < offset){
			++cur_offset;
			continue;
		}else{
			++cur_offset;
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::Limit::type() const
{
	return PhyOperator::LIMIT;
}

u32 CatDB::Sql::Limit::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "limit", "", output_rows, finished_time);
	return child->explain_operator(depth + 1, result);
}
