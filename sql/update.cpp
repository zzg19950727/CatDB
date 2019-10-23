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

PhyOperator_s Update::make_update(const TableSpace_s& table_space, 
                                    const Row_s& new_row)
{
    Update* op = new Update(table_space);
    op->new_row = new_row;
    return PhyOperator_s(op);
}

PhyOperator_s Update::make_update(const TableSpace_s& table_space, 
                                const Row_s& new_row,
                                const Filter_s& filter)
{
    Update* op = new Update(table_space);
    op->new_row = new_row;
    op->set_filter(filter);
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
	    ret = table_space->get_next_row(row);
	    if (ret == END_OF_TABLE_SPACE){
		    return NO_MORE_ROWS;
		}else if (ret != SUCCESS){
			Log(LOG_ERR, "Update", "get next row error:%s", err_string(ret));
			return ret;
		}else{
			if (filter){
				if ((*filter)(row)){
                    new_row->set_row_id( row->get_row_id() );
                    table_space->update_row(new_row);
					return SUCCESS;
				}else{
					continue;
				}
			}else{
                new_row->set_row_id( row->get_row_id() );
                table_space->update_row(new_row);
				return SUCCESS;
			}
        }
    }
}

u32 Update::type()const
{
    return PhyOperator::UPDATE;
}