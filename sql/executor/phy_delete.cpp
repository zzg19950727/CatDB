#include "phy_delete.h"
#include "phy_expression.h"
#include "table_space.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Storage;

PhyDelete::PhyDelete(PhyOperator_s &child, const TableSpace_s& table_space)
    :SingleChildPhyOperator(child),
	table_space(table_space)
{
}

PhyDelete::~PhyDelete()
{

}


PhyOperator_s PhyDelete::make_delete(PhyOperator_s &child,
								  const String&database,
								  const String& table,
								  const Vector<String> &engine_args,
								  const Expression_s& row_id,
								  bool is_delete_all)
{
	TableSpace_s table_space = TableSpace::make_table_space(table, database, engine_args);
    PhyDelete* op = new PhyDelete(child, table_space);
	op->is_delete_all = is_delete_all;
	op->row_id = row_id;
    return PhyOperator_s(op);
}

u32 PhyDelete::inner_open()
{
	u32 ret = child->open();
	if (FAIL(ret)) {
		return ret;
	}
    return table_space->open();
}

u32 PhyDelete::close()
{
	u32 ret = SUCCESS;
	u32 ret1 = child->close();
	u32 ret2 = table_space->close();
	if (FAIL(ret1)) {
		return ret1;
	} else {
		return ret2;
	}
}

u32 PhyDelete::reset()
{
    return child->reset();
}

u32 PhyDelete::inner_get_next_row()
{
    u32 ret = SUCCESS;
	Row_s row;
	//delete all rows
	if (is_delete_all) {
		ret = table_space->delete_all_row();
		if (ret != SUCCESS) {
			LOG_ERR("delete all rows error", K(ret));
			return ret;
		} else {
			return NO_MORE_ROWS;
		}
	}
	while (SUCC(child->get_next_row(row))) {
		set_input_rows(row);
		CHECK(row_id->get_result(exec_ctx));
		Object_s &result = exec_ctx->output_result;
		if (result->is_null()) {
            continue;
        }
		increase_affected_rows();
		return table_space->delete_row(result->value());
	}
	return ret;
}

u32 PhyDelete::type()const
{
    return PhyOperator::DELETE;
}
