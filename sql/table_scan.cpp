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

CatDB::Sql::TableScan::TableScan(const TableSpace_s & table_space)
	:table_space(table_space)
{
}

CatDB::Sql::TableScan::~TableScan()
{
	
}

PhyOperator_s CatDB::Sql::TableScan::make_table_scan(const String&database,
	const String& table,
	const RowDesc& desc)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
	TableScan* scan = new TableScan(table_space);
	scan->set_access_desc(desc);
	scan->set_output_desc(desc);
	return PhyOperator_s(scan);
}

PhyOperator_s CatDB::Sql::TableScan::make_table_scan(const String&database,
	const String& table,
	const RowDesc& desc,
	const Filter_s & filter)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
	TableScan* scan = new TableScan(table_space);
	scan->set_filter(filter);
	scan->set_access_desc(desc);
	scan->set_output_desc(desc);
	return PhyOperator_s(scan);
}

void CatDB::Sql::TableScan::set_alias_table_name(const String & alias_name)
{
	table_space->set_alias_table_name(alias_name);
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
	if (filter) {
		filter->reset(row);
	}
	return table_space->open();
}

u32 CatDB::Sql::TableScan::get_next_row(Row_s & row)
{
	u32 ret;
	row = Row::make_row(access_desc);
	
	for(;;){
		ret = table_space->get_next_row(row);
		if (ret == END_OF_TABLE_SPACE){
			return NO_MORE_ROWS;
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

u32 CatDB::Sql::TableScan::explain_operator(u32 depth, QueryResult * result)
{
	return result->add_operation_info(depth, "table scan", table_space->get_alias_table_name(), output_rows, finished_time);
}
