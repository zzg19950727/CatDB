#include "phy_update.h"
#include "phy_expression.h"
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
                                  const Vector<String> &engine_args,
                                  const Expression_s& row_id,
                                  const Vector<Expression_s>& value_exprs)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database, engine_args);
    PhyUpdate* op = new PhyUpdate(child, table_space);
    op->row_id = row_id;
    op->value_exprs = value_exprs;
    return PhyOperator_s(op);
}

u32 PhyUpdate::inner_open()
{
    u32 ret = SUCCESS;
    u32 ret1 = table_space->open();
    u32 ret2 = child->open();
    update_row = Row::make_row(update_desc.get_column_num());
    update_row->set_op_id(operator_id);
    access_row = Row::make_row(access_desc.get_column_num());
    access_row->set_op_id(operator_id);
    if (FAIL(ret1)) {
        return ret1;
    } else {
        return ret2;
    }
}

u32 PhyUpdate::close()
{
    u32 ret = SUCCESS;
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
    u32 ret = SUCCESS;
    u32 ret1 = table_space->reset();
    u32 ret2 = child->reset();
    if (FAIL(ret1)) {
        return ret1;
    } else {
        return ret2;
    }
}

u32 PhyUpdate::inner_get_next_row()
{
    u32 ret = SUCCESS;
    Row_s row;
    while(SUCC(child->get_next_row(row))){
        set_input_rows(row);
        CHECK(row_id->get_result(exec_ctx));
        Object_s result = exec_ctx->output_result;
        if (result->is_null()) {
            continue;
        }
        for (u32 i = 0; i < value_exprs.size(); ++i) {
            CHECK(value_exprs[i]->get_result(exec_ctx));
            CHECK(update_row->set_cell(i, exec_ctx->output_result));
        }
        CHECK(table_space->update_row(result->value(), 
                                      update_row, 
                                      access_row));
        increase_affected_rows();
    }
    return ret;
}

u32 PhyUpdate::type()const
{
    return PhyOperator::UPDATE;
}

void PhyUpdate::set_update_desc(const RowDesc& desc) 
{ 
    update_desc = desc; 
    table_space->set_update_desc(desc);
}

void PhyUpdate::set_access_desc(const RowDesc &desc) 
{ 
    access_desc = desc; 
    table_space->set_access_desc(desc);
}