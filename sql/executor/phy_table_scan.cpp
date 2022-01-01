#include "table_space.h"
#include "phy_table_scan.h"
#include "phy_expression.h"
#include "phy_filter.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Storage;

PhyTableScan::PhyTableScan(const TableSpace_s & table_space)
	:table_space(table_space)
{
}

PhyTableScan::~PhyTableScan()
{
	
}

PhyOperator_s PhyTableScan::make_table_scan(const String&database,
	const String& table,
	const Vector<String> &engine_args,
	double sample_size)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, 
															database, 
															engine_args, 
															sample_size, 
															true);
	PhyTableScan* scan = new PhyTableScan(table_space);
	return PhyOperator_s(scan);
}

u32 PhyTableScan::inner_open()
{
	return table_space->open();
}

u32 PhyTableScan::close()
{
	return table_space->close();
}

u32 PhyTableScan::reset()
{
	return table_space->reset();
}

void PhyTableScan::set_access_desc(const RowDesc &desc)
{
	access_row = Row::make_row(desc);
}

u32 PhyTableScan::inner_get_next_row(Row_s & row)
{
	u32 ret;
	row = access_row;
	for(;;){
		ret = table_space->get_next_row(row);
		if (ret == END_OF_TABLE_SPACE){
			ret = NO_MORE_ROWS;
		}
		return ret;
	} 
}

u32 PhyTableScan::type() const
{
	return PhyOperator::TABLE_SCAN;
}