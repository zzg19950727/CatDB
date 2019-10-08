#include "scalar_group.h"
#include "expression.h"
#include "filter.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::ScalarGroup::ScalarGroup(PhyOperator_s & child)
	:SingleChildPhyOperator(child)
{
}

CatDB::Sql::ScalarGroup::~ScalarGroup()
{
}

PhyOperator_s CatDB::Sql::ScalarGroup::make_scalar_group(PhyOperator_s & child, 
	const Expression_s& agg_expr)
{
	ScalarGroup* op = new ScalarGroup(child);
	op->set_agg_expr(agg_expr);
	return PhyOperator_s(op);
}

PhyOperator_s CatDB::Sql::ScalarGroup::make_scalar_group(PhyOperator_s & child, 
	const Expression_s & agg_expr, 
	const Filter_s & filter)
{
	ScalarGroup* op = new ScalarGroup(child);
	op->set_agg_expr(agg_expr);
	op->set_filter(filter);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::ScalarGroup::set_agg_expr(const Expression_s & expr)
{
	agg_func_expr = expr;
	return SUCCESS;
}

u32 CatDB::Sql::ScalarGroup::open()
{
	u32 ret = child->open();
	if (ret != SUCCESS){
		Log(LOG_ERR, "HashGroup", "open child operator error:%s", err_string(ret));
		return ret;
	}else{
		return calc_aggregate();
	}
}

u32 CatDB::Sql::ScalarGroup::close()
{
	return child->close();
}

u32 CatDB::Sql::ScalarGroup::reset()
{
	return child->reset();
}

u32 CatDB::Sql::ScalarGroup::reopen(const Row_s & row)
{
	u32 ret = child->reopen(row);
	if (ret != SUCCESS) {
		Log(LOG_ERR, "HashGroup", "reopen child operator error:%s", err_string(ret));
		return ret;
	}else {
		if (filter)
			filter->reset(row);
		return calc_aggregate();
	}
}

u32 CatDB::Sql::ScalarGroup::get_next_row(Row_s & row)
{
	if (result){
		row = make_row(result);
		if (filter && !(*filter)(row))
			return NO_MORE_ROWS;
		result.reset();
		return SUCCESS;
	}else{
		return NO_MORE_ROWS;
	}
}

u32 CatDB::Sql::ScalarGroup::type() const
{
	return PhyOperator::SCALAR_GROUP;
}

void CatDB::Sql::ScalarGroup::set_filter(const Filter_s & filter)
{
	this->filter = filter;
}

Row_s CatDB::Sql::ScalarGroup::make_row(const Object_s & result)
{
	RowDesc row_desc(1);
	ColumnDesc col_desc;
	col_desc.set_tid_cid(alias_table_id, 0);
	row_desc.set_column_desc(0, col_desc);
	Row_s row = Row::make_row(row_desc);
	row->set_cell(0, result);
	return row;
}

u32 CatDB::Sql::ScalarGroup::calc_aggregate()
{
	Row_s row;
	AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_func_expr.get());
	agg_func->reset();
	while (child->get_next_row(row) == SUCCESS){
		agg_func->add_row(row);
	}
	Row_s tmp_null;
	result = agg_func->get_result(tmp_null);
	return SUCCESS;
}
