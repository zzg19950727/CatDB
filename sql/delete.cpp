#include "table_space.h"
#include "delete.h"
#include "expression.h"
#include "filter.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Storage;

Delete::Delete(const TableSpace_s& table_space)
    :table_space(table_space)
{
    assert(table_space);
}

Delete::~Delete()
{

}


PhyOperator_s Delete::make_delete(const String&database,
								const String& table,
								const String& alias_table_name,
                                const Filter_s& filter)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
    Delete* op = new Delete(table_space);
    op->set_filter(filter);
	table_space->set_alias_table_name(alias_table_name);
    return PhyOperator_s(op);
}

u32 Delete::set_filter(const Filter_s& filter)
{
    this->filter = filter;
    return SUCCESS;
}

Filter_s Delete::get_filter()const
{
    return filter;
}

u32 Delete::open()
{
    return table_space->open();
}

u32 Delete::close()
{
    return table_space->close();
}

u32 Delete::reset()
{
    return table_space->reset();
}

u32 Delete::reopen(const Row_s& row)
{
    return SUCCESS;
}

u32 Delete::get_next_row(Row_s &row)
{
    u32 ret;
	//delete all rows
	if (!filter) {
		ret = table_space->delete_all_row();
		if (ret != SUCCESS) {
			LOG_ERR("delete all rows error", err_string(ret));
			return ret;
		}else {
			return NO_MORE_ROWS;
		}
	}else
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
					table_space->delete_row(row->get_row_id());
					return SUCCESS;
				}else{
					continue;
				}
			}else{
				table_space->delete_row(row->get_row_id());
				return SUCCESS;
			}
        }
    }
}

u32 Delete::type()const
{
    return PhyOperator::DELETE;
}

u32 Delete::explain_operator(u32 depth, QueryResult * result)
{
	return result->add_operation_info(depth, "delete", table_space->get_alias_table_name(), output_rows, finished_time);
}
