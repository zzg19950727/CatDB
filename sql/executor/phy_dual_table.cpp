#include "phy_dual_table.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyDualTable::PhyDualTable()
    :is_start(false)
{
}

PhyDualTable::~PhyDualTable()
{
	
}

PhyOperator_s PhyDualTable::make_dual_table()
{
	PhyDualTable* dual_table = new PhyDualTable();
	return PhyOperator_s(dual_table);
}

u32 PhyDualTable::inner_open()
{
	return SUCCESS;
}

u32 PhyDualTable::close()
{
	return SUCCESS;
}

u32 PhyDualTable::reset()
{
    is_start = false;
	return SUCCESS;
}

u32 PhyDualTable::inner_get_next_row(Row_s & row)
{
    if (!is_start) {
        row = cur_row;
        is_start = true;
        return SUCCESS;
    } else {
        return NO_MORE_ROWS;
    }
}

u32 PhyDualTable::type() const
{
	return PhyOperator::DUAL_TABLE;
}