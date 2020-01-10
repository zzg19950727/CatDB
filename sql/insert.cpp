#include "table_space.h"
#include "insert.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Storage;

Insert::Insert(const TableSpace_s& table_space)
    :table_space(table_space)
{

}

Insert::~Insert()
{

}

PhyOperator_s Insert::make_insert(const String&database, const String& table, const String& alias_table_name)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
	table_space->set_alias_table_name(alias_table_name);
    return PhyOperator_s(new Insert(table_space));
}

u32 Insert::open()
{
    return table_space->open();
}

u32 Insert::close()
{
    table_space->close();
	return SUCCESS;
}

u32 Insert::reset()
{
    return table_space->reset();
}

u32 Insert::reopen(const Row_s& row)
{
    return SUCCESS;
}

u32 Insert::get_next_row(Row_s &row)
{
    return table_space->insert_row(row);
}

u32 Insert::type() const
{
    return PhyOperator::INSERT;
}

u32 Insert::explain_operator(u32 depth, QueryResult * result)
{
	return result->add_operation_info(depth, "insert", table_space->get_alias_table_name(), output_rows, finished_time);
}
