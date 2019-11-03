#include "hash_group.h"
#include "expression.h"
#include "filter.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

HashGroup::HashGroup(PhyOperator_s & child)
	:SingleChildPhyOperator(child),
	cur_bucket_pos(0),
	out_when_empty_input(false),
	end_of_bucket(false)
{
	cur_bucket = hash_table.end_bucket();
}

HashGroup::~HashGroup()
{
}

PhyOperator_s HashGroup::make_hash_group(PhyOperator_s & child,
	const Vector<Expression_s>& group_columns)
{
	HashGroup* op = new HashGroup(child);
	op->set_group_columns(group_columns);
	op->init_hash_table();
	return PhyOperator_s(op);
}

PhyOperator_s HashGroup::make_hash_group(PhyOperator_s & child, 
	const Vector<Expression_s>& group_columns,
	const Filter_s & filter)
{
	HashGroup* op = new HashGroup(child);
	op->set_group_columns(group_columns);
	op->init_hash_table();
	op->set_filter(filter);
	return PhyOperator_s(op);
}

u32 HashGroup::set_group_columns(const Vector<Expression_s>& expr)
{
	group_cols = expr;
	return SUCCESS;
}

u32 HashGroup::add_agg_expr(const Expression_s & expr)
{
	agg_funcs.push_back(expr);
	return SUCCESS;
}

u32 HashGroup::set_filter(const Filter_s & filter)
{
	this->filter = filter;
	return SUCCESS;
}

void HashGroup::set_agg_table_id(u32 id)
{
	agg_table_id = id;
}

u32 HashGroup::get_agg_table_id() const
{
	return agg_table_id;
}

u32 HashGroup::open()
{
	u32 ret = child->open();
	if (ret != SUCCESS){
		Log(LOG_ERR, "HashGroup", "open child operator error:%s", err_string(ret));
		return ret;
	}else{
		ret = build_hash_table();
		if (ret != SUCCESS) {
			Log(LOG_ERR, "HashGroup", "build hash table error:%s", err_string(ret));
			return ret;
		}
		return ret;
	}
}

u32 HashGroup::close()
{
	return child->close();
}

u32 HashGroup::reset()
{
	reset_agg_func();
	cur_bucket_pos = 0;
	out_when_empty_input = false;
	cur_bucket = hash_table.begin_bucket();
	return child->reset();
}

u32 HashGroup::reopen(const Row_s & row)
{
	reset_agg_func();
	cur_bucket_pos = 0;
	out_when_empty_input = false;
	
	u32 ret = child->reopen(row);
	if (ret != SUCCESS) {
		return ret;
	}
	else {
		ret = build_hash_table();
		if (ret != SUCCESS) {
			Log(LOG_ERR, "HashGroup", "build hash table error:%s", err_string(ret));
			return ret;
		}
		
		return ret;
	}
}

u32 HashGroup::get_next_row(Row_s & row)
{
	if (cur_bucket == hash_table.end_bucket() || hash_table.empty() || end_of_bucket){
		//如果没有子节点没有任何输入，则输出对应聚合函数结果一次
		if (hash_table.empty() && !out_when_empty_input){
			//make row
			row.reset();
			row = make_row( row );
			out_when_empty_input = true;
			if (filter && !(*filter)(row))
				return NO_MORE_ROWS;
			else
				return SUCCESS;
		}else{
			return NO_MORE_ROWS;
		}
	}
	const Vector<Row_s>& bucket = hash_table.bucket(cur_bucket);
	const Row_s& cur_group_row = bucket[cur_bucket_pos];
	reset_agg_func();
	for (; cur_bucket_pos < bucket.size(); ++cur_bucket_pos){
		//是否是同一group
		if (this->euqal(cur_group_row, bucket[cur_bucket_pos])){
			add_row_to_agg_func(bucket[cur_bucket_pos]);
		}else{
			break;
		}
	}
	if (cur_bucket_pos == bucket.size()){
		u32 ret = hash_table.next_none_empty_bucket(cur_bucket);
		if (ret != SUCCESS) {
			end_of_bucket = true;
		}
		cur_bucket_pos = 0;
	}
	row = make_row(cur_group_row);
	if (filter && !(*filter)(row))
		return get_next_row(row);
	else
		return SUCCESS;
}

u32 HashGroup::type() const
{
	return PhyOperator::HASH_GROUP;
}

u32 HashGroup::init_hash_table()
{
	hash_table.clear_hash_columns();
	hash_table.clear_probe_columns();
	for (u32 i = 0; i < group_cols.size(); ++i){
		hash_table.add_hash_column(group_cols[i]);
		hash_table.add_probe_column(group_cols[i]);
	}
	return SUCCESS;
}

u32 HashGroup::build_hash_table()
{
	hash_table.clear();
	Row_s row;
	while (child->get_next_row(row) == SUCCESS){
		hash_table.build(row);
	}
	cur_bucket = hash_table.begin_bucket();
	auto iter = hash_table.begin_bucket();
	while(iter != hash_table.end_bucket()){
		hash_table.sort_bucket(iter);
		u32 ret = hash_table.next_none_empty_bucket(iter);
		if (ret != SUCCESS) {
			break;
		}
	}
	return SUCCESS;
}

bool HashGroup::euqal(const Row_s & lhs, const Row_s & rhs)
{
	for (u32 i = 0; i < group_cols.size(); ++i){
		Object_s lhs_value = group_cols[i]->get_result(lhs);
		Object_s rhs_value = group_cols[i]->get_result(rhs);
		Object_s result = lhs_value->operator==(rhs_value);
		if (!result->bool_value()){
			return false;
		}
	}
	return true;
}

void HashGroup::reset_agg_func()
{
	for (u32 i = 0; i < agg_funcs.size(); ++i) {
		AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_funcs[i].get());
		agg_func->reset();
	}
}

u32 HashGroup::add_row_to_agg_func(const Row_s & row)
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

Row_s HashGroup::make_row(const Row_s& row)
{
	RowDesc row_desc(group_cols.size() + agg_funcs.size());
	Row_s new_row = Row::make_row(row_desc);
	u32 i = 0;
	for (; i < group_cols.size(); ++i) {
		ColumnExpression* column = dynamic_cast<ColumnExpression*>(group_cols[i].get());
		new_row->get_row_desc().set_column_desc(i, column->get_column_desc());
		new_row->set_cell(i, group_cols[i]->get_result(row));
	}
	Row_s tmp_null;
	for (u32 j = 0; j < agg_funcs.size(); ++i, ++j) {
		ColumnDesc col_desc;
		col_desc.set_tid_cid(agg_table_id, j);
		Object_s result = agg_funcs[j]->get_result(tmp_null);
		new_row->get_row_desc().set_column_desc(i, col_desc);
		new_row->set_cell(i, result);
	}
	return new_row;
}
