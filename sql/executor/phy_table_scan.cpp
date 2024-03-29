#include "table_space.h"
#include "phy_table_scan.h"
#include "phy_expression.h"
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
	double sample_value)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, 
															database, 
															engine_args, 
															sample_value, 
															true);
	PhyTableScan* scan = new PhyTableScan(table_space);
	return PhyOperator_s(scan);
}

u32 PhyTableScan::inner_open()
{
	u32 ret = SUCCESS;
	store_row = Row::make_row(access_desc.get_column_num());
	store_row->set_op_id(operator_id);
	for (u32 i = 0; i < store_row->get_cell_num(); ++i)
	{
		ColumnDesc col_desc;
		CHECK(access_desc.get_column_desc(i, col_desc));
		Object_s value = Object::make_empty_object(col_desc.get_data_type());
		store_row->set_cell(i, value);
	}
	CHECK(table_space->open());
	return ret;
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
	access_desc = desc;
	table_space->set_access_desc(desc);
}

u32 PhyTableScan::inner_get_next_row()
{
	u32 ret;
	for (;;) {
		ret = table_space->get_next_row(store_row);
		if (ret == END_OF_TABLE_SPACE) {
			ret = NO_MORE_ROWS;
		} else {
			set_input_rows(store_row);
		}
		return ret;
	} 
}

u32 PhyTableScan::type() const
{
	return PhyOperator::TABLE_SCAN;
}