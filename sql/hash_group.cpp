#include "hash_group.h"
#include "expression.h"
#include "filter.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::HashGroup::HashGroup(PhyOperator_s & child)
	:SingleChildPhyOperator(child),
	cur_bucket_idx(0),
	cur_bucket_pos(0),
	out_when_empty_input(false)
{
}

CatDB::Sql::HashGroup::~HashGroup()
{
}

PhyOperator_s CatDB::Sql::HashGroup::make_hash_group(PhyOperator_s & child,
	const Vector<Expression_s>& group_columns,
	const Expression_s & agg_expr)
{
	HashGroup* op = new HashGroup(child);
	op->set_group_columns(group_columns);
	op->set_agg_expr(agg_expr);
	op->init_hash_table();
	return PhyOperator_s(op);
}

PhyOperator_s CatDB::Sql::HashGroup::make_hash_group(PhyOperator_s & child, 
	const Vector<Expression_s>& group_columns, 
	const Expression_s & agg_expr, 
	const Filter_s & filter)
{
	HashGroup* op = new HashGroup(child);
	op->set_group_columns(group_columns);
	op->set_agg_expr(agg_expr);
	op->init_hash_table();
	op->set_filter(filter);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::HashGroup::set_group_columns(const Vector<Expression_s>& expr)
{
	group_cols = expr;
	return SUCCESS;
}

u32 CatDB::Sql::HashGroup::set_agg_expr(const Expression_s & expr)
{
	agg_func_expr = expr;
	return SUCCESS;
}

u32 CatDB::Sql::HashGroup::set_filter(const Filter_s & filter)
{
	this->filter = filter;
	return SUCCESS;
}

u32 CatDB::Sql::HashGroup::open()
{
	u32 ret = child->open();
	if (ret != SUCCESS){
		Log(LOG_ERR, "HashGroup", "open child operator error:%s", err_string(ret));
		return ret;
	}else{
		return build_hash_table();
	}
}

u32 CatDB::Sql::HashGroup::close()
{
	return child->close();
}

u32 CatDB::Sql::HashGroup::reset()
{
	AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_func_expr.get());
	agg_func->reset();
	cur_bucket_idx = 0;
	cur_bucket_pos = 0;
	out_when_empty_input = false;
	return child->reset();
}

u32 CatDB::Sql::HashGroup::reopen(const Row_s & row)
{
	AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_func_expr.get());
	agg_func->reset();
	cur_bucket_idx = 0;
	cur_bucket_pos = 0;
	out_when_empty_input = false;
	
	u32 ret = child->reopen(row);
	if (ret != SUCCESS) {
		return ret;
	}
	else {
		return build_hash_table();
	}
}

u32 CatDB::Sql::HashGroup::get_next_row(Row_s & row)
{
	if (cur_bucket_idx >= hash_table.bucket_count()){
		//如果没有子节点没有任何输入，则输出对应聚合函数结果一次
		if (hash_table.empty() && !out_when_empty_input){
			//make row
			Row_s tmp_null;
			Object_s result = agg_func_expr->get_result(tmp_null);
			row = make_row(result);
			out_when_empty_input = true;
			if (filter && !(*filter)(row))
				return NO_MORE_ROWS;
			else
				return SUCCESS;
		}else{
			return NO_MORE_ROWS;
		}
	}else if (agg_func_expr->get_type() != Expression::Aggregate){
		Log(LOG_ERR, "HashGroup", "expression must be aggregate function");
		return ERR_EXPR_TYPE;
	}
	const Vector<Row_s>& bucket = hash_table.bucket(cur_bucket_idx);
	const Row_s& cur_group_row = bucket[cur_bucket_pos];
	AggregateExpression* agg_func = dynamic_cast<AggregateExpression*>(agg_func_expr.get());
	agg_func->reset();
	for (; cur_bucket_pos < bucket.size(); ++cur_bucket_pos){
		//是否是同一group
		if (this->euqal(cur_group_row, bucket[cur_bucket_pos])){
			agg_func->add_row(bucket[cur_bucket_pos]);
		}else{
			break;
		}
	}
	if (cur_bucket_pos == bucket.size()){
		++cur_bucket_idx;
		cur_bucket_pos = 0;
	}
	Row_s tmp_null;
	Object_s result = agg_func->get_result(tmp_null);
	row = make_row(result);
	if (filter && !(*filter)(row))
		return get_next_row(row);
	else
		return SUCCESS;
}

u32 CatDB::Sql::HashGroup::type() const
{
	return PhyOperator::HASH_GROUP;
}

u32 CatDB::Sql::HashGroup::init_hash_table()
{
	hash_table.clear_hash_columns();
	hash_table.clear_probe_columns();
	for (u32 i = 0; i < group_cols.size(); ++i){
		hash_table.add_hash_column(group_cols[i]);
		hash_table.add_probe_column(group_cols[i]);
	}
	return SUCCESS;
}

u32 CatDB::Sql::HashGroup::build_hash_table()
{
	hash_table.clear();
	Row_s row;
	while (child->get_next_row(row) == SUCCESS){
		hash_table.build(row);
	}
	u32 size = hash_table.bucket_count();
	for (u32 i = 0; i < size; ++i){
		hash_table.sort_bucket(i);
	}
	return SUCCESS;
}

bool CatDB::Sql::HashGroup::euqal(const Row_s & lhs, const Row_s & rhs)
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

Row_s CatDB::Sql::HashGroup::make_row(const Object_s & result)
{
	RowDesc row_desc(1);
	ColumnDesc col_desc;
	col_desc.set_tid_cid(alias_table_id, 0);
	row_desc.set_column_desc(0, col_desc);
	Row_s row = Row::make_row(row_desc);
	row->set_cell(0, result);
	return row;
}
