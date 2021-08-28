#include "phy_update.h"
#include "phy_expression.h"
#include "phy_filter.h"
#include "table_space.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Storage;

PhyUpdate::PhyUpdate(PhyOperator_s& child, const TableSpace_s& table_space)
    :SingleChildPhyOperator(child),
     table_space(table_space)
{
}

PhyUpdate::~PhyUpdate()
{

}

PhyOperator_s PhyUpdate::make_update(PhyOperator_s& child,
                                  const String&database, 
                                  const String& table,
                                  const Expression_s& row_id,
                                  const Vector<Expression_s>& value_exprs)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database);
    PhyUpdate* op = new PhyUpdate(child, table_space);
    op->row_id = row_id;
    op->value_exprs = value_exprs;
    return PhyOperator_s(op);
}

u32 PhyUpdate::inner_open()
{
    u32 ret1 = table_space->open();
    u32 ret2 = child->open();
    if (FAIL(ret1)) {
        return ret1;
    } else {
        return ret2;
    }
}

u32 PhyUpdate::close()
{
    u32 ret1 = table_space->close();
    u32 ret2 = child->close();
    if (FAIL(ret1)) {
        return ret1;
    } else {
        return ret2;
    }
}

u32 PhyUpdate::reset()
{
    u32 ret1 = table_space->reset();
    u32 ret2 = child->reset();
    if (FAIL(ret1)) {
        return ret1;
    } else {
        return ret2;
    }
}

u32 PhyUpdate::inner_get_next_row(Row_s &row)
{
    u32 ret;
    while((ret = child->get_next_row(row)) == SUCCESS){
        Object_s result = row_id->get_result(row);
        MY_ASSERT(T_NUMBER == result->get_type());
		Number_s num = result;
        cur_row->set_row_id( num->value() );
        for (u32 i = 0; SUCC(ret) && i < value_exprs.size(); ++i) {
            CHECK(cur_row->set_cell(i, value_exprs[i]->get_result(row)));
        }
        CHECK(table_space->update_row(cur_row));
    }
    return ret;
}

u32 PhyUpdate::type()const
{
    return PhyOperator::UPDATE;
}