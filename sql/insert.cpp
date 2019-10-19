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

PhyOperator_s Insert::make_insert(const TableSpace_s& table_space)
{
    return PhyOperator_s(new Insert(table_space));
}

u32 Insert::open()
{
    return table_space->open();
}

u32 Insert::close()
{
    table_space->close();
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