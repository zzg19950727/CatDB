#include "query.h"
#include "expression.h"
#include "error.h"
#include "log.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

Query::Query(PhyOperator_s& child, const Vector<Expression_s>& query_list)
	:SingleChildPhyOperator(child),
	query_list(query_list)
{
}

PhyOperator_s Query::make_query(PhyOperator_s& child, const Vector<Expression_s>& query_list)
{
	return PhyOperator_s(new Query(child, query_list));
}

u32 Query::add_query_item(const Expression_s& item)
{
	query_list.push_back(item);
	return SUCCESS;
}

u32 Query::set_alias_table_id(u32 id)
{
	alias_table_id = id;
	return SUCCESS;
}

u32 Query::get_alias_table_id() const
{
	return alias_table_id;
}

u32 Query::open()
{
	return child->open();
}

u32 Query::close()
{
	return child->close();
}

u32 Query::reset()
{
	return child->reset();
}

u32 Query::reopen(const Row_s & row)
{
	return child->reopen(row);
}

u32 Query::get_next_row(Row_s & row)
{
	while (child->get_next_row(row) == SUCCESS) {
		row = make_row(row);
		return SUCCESS;
	}
	return NO_MORE_ROWS;
}

u32 Query::type() const
{
	return PhyOperator::QUERY;
}

u32 CatDB::Sql::Query::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "query", "", output_rows, finished_time);
	return child->explain_operator(depth + 1, result);
}

Row_s CatDB::Sql::Query::make_row(const Row_s & row)
{
	RowDesc row_desc(query_list.size());
	for (u32 i = 0; i < query_list.size(); ++i) {
		ColumnDesc col_desc;
		col_desc.set_tid_cid(alias_table_id, i);
		row_desc.set_column_desc(i, col_desc);
	}
	Row_s new_row = Row::make_row(row_desc);
	for (u32 i = 0; i < query_list.size(); ++i) {
		Object_s cell = query_list[i]->get_result(row);
		new_row->set_cell(i, cell);
	}
	return new_row;
}
