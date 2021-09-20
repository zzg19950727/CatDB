#include "phy_limit.h"
#include "error.h"
#include "log.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::PhyLimit::PhyLimit(PhyOperator_s& child, u32 size, u32 offset)
	:SingleChildPhyOperator(child),
	size(size),
	offset(offset),
	cur_offset(0)
{
}

PhyOperator_s CatDB::Sql::PhyLimit::make_limit(PhyOperator_s& child, u32 size, u32 offset)
{
	return PhyOperator_s(new PhyLimit(child, size, offset));
}

u32 CatDB::Sql::PhyLimit::inner_open()
{
	return child->open();
}

u32 CatDB::Sql::PhyLimit::close()
{
	return child->close();
}

u32 CatDB::Sql::PhyLimit::reset()
{
	cur_offset = 0;
	return child->reset();
}

u32 CatDB::Sql::PhyLimit::inner_get_next_row(Row_s & row)
{
	if ( cur_offset >= offset && cur_offset - offset >= size){
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

u32 CatDB::Sql::PhyLimit::type() const
{
	return PhyOperator::LIMIT;
}
