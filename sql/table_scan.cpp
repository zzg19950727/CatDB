#include "table_space.h"
#include "table_scan.h"
#include "expression.h"
#include "filter.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Storage;

CatDB::Sql::TableScan::TableScan(const TableSpace_s & table_space
	, const RowDesc& desc)
	:table_space(table_space),
	desc(desc)
{
}

CatDB::Sql::TableScan::~TableScan()
{
	
}

PhyOperator_s CatDB::Sql::TableScan::make_table_scan(const TableSpace_s & table_space,
	const RowDesc& desc)
{
	return PhyOperator_s(new TableScan(table_space, desc));
}

PhyOperator_s CatDB::Sql::TableScan::make_table_scan(const TableSpace_s & table_space,
	const RowDesc& desc,
	const Filter_s & filter)
{
	TableScan* scan = new TableScan(table_space, desc);
	scan->set_filter(filter);
	return PhyOperator_s(scan);
}

u32 CatDB::Sql::TableScan::set_filter(const Filter_s & filter)
{
	this->filter = filter;
	return SUCCESS;
}

Filter_s CatDB::Sql::TableScan::get_filter() const
{
	return filter;
}

u32 CatDB::Sql::TableScan::open()
{
	return table_space->open();
}

u32 CatDB::Sql::TableScan::close()
{
	return table_space->close();
}

u32 CatDB::Sql::TableScan::reset()
{
	return table_space->reset();
}

u32 CatDB::Sql::TableScan::reopen(const Row_s & row)
{
	filter->reset(row);
	table_space->reset();
	return SUCCESS;
}

u32 CatDB::Sql::TableScan::get_next_row(Row_s & row)
{
	u32 ret;
	row = Row::make_row(desc);
	
	for(;;){
		ret = table_space->get_next_row(row);
		if (ret == END_OF_TABLE_SPACE){
			return ret;
		}else if (ret != SUCCESS){
			Log(LOG_ERR, "TableScan", "get next row error:%s", err_string(ret));
			return ret;
		}else{
			if (filter){
				if ((*filter)(row)){
					return SUCCESS;
				}else{
					continue;
				}
			}else{
				return SUCCESS;
			}
		}
	} 
}

u32 CatDB::Sql::TableScan::type() const
{
	return PhyOperator::TABLE_SCAN;
}
