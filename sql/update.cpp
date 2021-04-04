#include "table_space.h"
#include "update.h"
#include "expression.h"
#include "filter.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Storage;

Update::Update(const TableSpace_s& table_space)
    :table_space(table_space)
{
    assert(table_space);
}

Update::~Update()
{

}

PhyOperator_s Update::make_update(const String&database, 
									const String& table,
                                    const Row_s& new_row)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
    Update* op = new Update(table_space);
    op->new_row = new_row;
    return PhyOperator_s(op);
}

PhyOperator_s Update::make_update(const String&database,
								const String& table,
								const String& alias_table_name,
                                const Row_s& new_row,
                                const Filter_s& filter)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
    Update* op = new Update(table_space);
    op->new_row = new_row;
    op->set_filter(filter);
	table_space->set_alias_table_name(alias_table_name);
    return PhyOperator_s(op);
}

u32 Update::set_filter(const Filter_s& filter)
{
    this->filter = filter;
    return SUCCESS;
}

Filter_s Update::get_filter()const
{
    return filter;
}

u32 Update::open()
{
    return table_space->open();
}

u32 Update::close()
{
    return table_space->close();
}

u32 Update::reset()
{
    return table_space->reset();
}

u32 Update::reopen(const Row_s& row)
{
    return SUCCESS;
}

u32 Update::get_next_row(Row_s &row)
{
    u32 ret;
    for(;;){
		row = Row::make_row(access_desc);
	    ret = table_space->get_next_row(row);
	    if (ret == END_OF_TABLE_SPACE){
		    return NO_MORE_ROWS;
		}
		else if (ret == NO_MORE_ROWS) {
			return NO_MORE_ROWS;
		}
		else if (ret != SUCCESS){
			LOG_ERR("get next row error", err_string(ret));
			return ret;
		}else{
			if (filter){
				if ((*filter)(row)){
                    new_row->set_row_id( row->get_row_id() );
                    return table_space->update_row(new_row);
				}else{
					continue;
				}
			}else{
                new_row->set_row_id( row->get_row_id() );
                return table_space->update_row(new_row);
			}
        }
    }
}

u32 Update::type()const
{
    return PhyOperator::UPDATE;
}

u32 CatDB::Sql::Update::explain_operator(u32 depth, QueryResult * result)
{
	return result->add_operation_info(depth, "update", table_space->get_alias_table_name(), output_rows, finished_time);
}
