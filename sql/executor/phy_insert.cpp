#include "phy_insert.h"
#include "table_space.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Storage;

PhyInsert::PhyInsert(PhyOperator_s &child, const TableSpace_s& table_space)
    :SingleChildPhyOperator(child),
    table_space(table_space)
{

}

PhyInsert::~PhyInsert()
{

}

PhyOperator_s PhyInsert::make_insert(PhyOperator_s &child,
                                  const String&database,
                                  const String& table,
                                  const Vector<String> &engine_args)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database, engine_args);
    return PhyOperator_s(new PhyInsert(child, table_space));
}

u32 PhyInsert::inner_open()
{
    u32 ret = child->open();
    if (ret != SUCCESS) {
        return ret;
    }
    return table_space->open();
}

u32 PhyInsert::close()
{
    table_space->close();
	return SUCCESS;
}

u32 PhyInsert::reset()
{
    return table_space->reset();
}

u32 PhyInsert::reopen(const Row_s& row)
{
    return SUCCESS;
}

u32 PhyInsert::inner_get_next_row(Row_s &row)
{
    u32 ret = SUCCESS;
	while ((ret = child->get_next_row(row)) == SUCCESS) {
        increase_affected_rows();
		return table_space->insert_row(row);
	}
	return ret;
}

u32 PhyInsert::type() const
{
    return PhyOperator::INSERT;
}
