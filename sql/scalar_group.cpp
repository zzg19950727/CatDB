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

PhyOperator_s CatDB::Sql::ScalarGroup::make_scalar_group(PhyOperator_s & child)
{
	ScalarGroup* op = new ScalarGroup(child);
	return PhyOperator_s(op);
}

PhyOperator_s CatDB::Sql::ScalarGroup::make_scalar_group(PhyOperator_s & child,
	const Filter_s & filter)
{
	ScalarGroup* op = new ScalarGroup(child);
	op->set_filter(filter);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::ScalarGroup::add_agg_expr(const Expression_s & expr)
{
	agg_funcs.push_back(expr);
	return SUCCESS;
}

u32 CatDB::Sql::ScalarGroup::set_alias_table_id(u32 id)
{
	alias_table_id = id;
	return SUCCESS;
}

u32 CatDB::Sql::ScalarGroup::get_alias_table_id() const
{
	return alias_table_id;
}

u32 CatDB::Sql::ScalarGroup::open()
{
	u32 ret = child->open();
	if (ret != SUCCESS){
		LOG_ERR("open child operator error", err_string(ret));
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
		LOG_ERR("reopen child operator error", err_string(ret));
		return ret;
	}else {
		if (filter)
			filter->reset(row);
		reset_agg_func();
		return calc_aggregate();
	}
}

u32 CatDB::Sql::ScalarGroup::get_next_row(Row_s & row)
{
	if (result){
		row = result;
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

u32 CatDB::Sql::ScalarGroup::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "scalar group", "", output_rows, finished_time);
	return child->explain_operator(depth + 1, result);
}

void CatDB::Sql::ScalarGroup::set_filter(const Filter_s & filter)
{
	this->filter = filter;
}

u32 CatDB::Sql::ScalarGroup::calc_aggregate()
{
	Row_s row;
	while (child->get_next_row(row) == SUCCESS){
		add_row_to_agg_func(row);
	}
	result = make_row();
	return SUCCESS;
}

void CatDB::Sql::ScalarGroup::reset_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_funcs[i].get());
		agg_func->reset();
	}
}

u32 CatDB::Sql::ScalarGroup::add_row_to_agg_func(const Row_s & row)
{
	u32 ret = SUCCESS;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_funcs[i].get());
		ret = agg_func->add_row(row);
		if (ret != SUCCESS) {
			break;
		}
	}
	return ret;
}

Row_s CatDB::Sql::ScalarGroup::make_row()
{
	RowDesc row_desc(agg_funcs.size());
	Row_s row = Row::make_row(row_desc);
	Row_s tmp_null;
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		ColumnDesc col_desc;
		col_desc.set_tid_cid(alias_table_id, i);
		Object_s result = agg_funcs[i]->get_result(tmp_null);
		row->get_row_desc().set_column_desc(i, col_desc);
		row->set_cell(i, result);
	}
	return row;
}
