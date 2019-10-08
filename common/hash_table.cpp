#include "hash_table.h"
#include "expression.h"
#include "Object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Common::HashTable::HashTable()
{
}

CatDB::Common::HashTable::~HashTable()
{

}

void CatDB::Common::HashTable::clear()
{
	buckets.clear();
}

bool CatDB::Common::HashTable::empty() const
{
	return buckets.empty();
}

u32 CatDB::Common::HashTable::build(const Row_s & row)
{
	u32 hash_value = hash(hash_cols, row);
	buckets[hash_value].push_back(row);
	return SUCCESS;
}

u32 CatDB::Common::HashTable::probe(const Row_s & row)
{
	u32 hash_value = hash(probe_cols, row);
	if (buckets.find(hash_value) == buckets.cend()){
		return ROW_NOT_FOUND;
	}else{
		const auto& bucket = buckets[hash_value];
		for (auto iter = bucket.cbegin(); iter != bucket.cend(); ++iter){
			if (equal(*iter, row))
				return SUCCESS;
		}
		return ROW_NOT_FOUND;
	}
}

u32 CatDB::Common::HashTable::probe(const Row_s & row, Queue<Row_s>& out_rows)
{
	u32 hash_value = hash(probe_cols, row);
	if (buckets.find(hash_value) == buckets.cend()){
		return ROW_NOT_FOUND;
	}else{
		const auto& bucket = buckets[hash_value];
		for (auto iter = bucket.cbegin(); iter != bucket.cend(); ++iter){
			Row_s new_row = RowAgent::make_agent_row(row, *iter);
			Object_s result = probe_condition->get_result(new_row);
			if (result->get_type() == T_ERROR_RESULT){
				Log(LOG_WARN, "HashTable", "probe condition calc error");
				continue;
			}else if (result->bool_value()){
				out_rows.push(*iter);
			}
		}
	}
	if (out_rows.empty()){
		return ROW_NOT_FOUND;
	}else{
		return SUCCESS;
	}
}

u32 CatDB::Common::HashTable::add_hash_column(const Expression_s & col_expr)
{
	hash_cols.push_back(col_expr);
	return SUCCESS;
}

void CatDB::Common::HashTable::clear_hash_columns()
{
	hash_cols.clear();
}

u32 CatDB::Common::HashTable::add_probe_column(const Expression_s & col_expr)
{
	probe_cols.push_back(col_expr);
	return SUCCESS;
}

void CatDB::Common::HashTable::clear_probe_columns()
{
	probe_cols.clear();
}

u32 CatDB::Common::HashTable::set_probe_condition(const Expression_s & probe_expr)
{
	probe_condition = probe_expr;
	return SUCCESS;
}

u32 CatDB::Common::HashTable::bucket_count() const
{
	return buckets.size();
}

u32 CatDB::Common::HashTable::sort_bucket(u32 idx)
{
	if (idx >= buckets.size()){
		Log(LOG_ERR, "HashTable", "idx %u bigger than bucket size %u", idx, buckets.size());
		return ERR_BUCKET_IDX;
	}
	auto less_func = std::bind(&HashTable::less, this, std::placeholders::_1, std::placeholders::_2);
	u32 i = 0;
	auto iter = buckets.begin();
	for (; iter != buckets.end(); ++iter, ++i){
		if (i == idx){
			break;
		}
	}
	std::sort(iter->second.begin(), iter->second.end(), less_func);
	return SUCCESS;
}

const Vector<Row_s>& CatDB::Common::HashTable::bucket(u32 idx)
{
	if (idx >= buckets.size()){
		Log(LOG_ERR, "HashTable", "idx %u bigger than bucket size %u", idx, buckets.size());
		//TODO ´íÎó´¦Àí£¿
	}
	u32 i = 0;
	for (auto iter = buckets.cbegin(); iter != buckets.cend(); ++iter, ++i){
		if (i == idx)
			return iter->second;
	}
	//naver goto here
	return buckets.cbegin()->second;
}

bool CatDB::Common::HashTable::less(const Row_s & lhs, const Row_s & rhs)
{
	for (u32 i = 0; i < hash_cols.size(); ++i){
		Object_s left = hash_cols[i]->get_result(lhs);
		Object_s right = hash_cols[i]->get_result(rhs);
		Object_s result = left->operator==(right);
		if (result->bool_value())
			continue;
		result = left->operator>(right);
		if (result->bool_value())
			return false;
		else
			return true;
	}
	return false;
}

u32 CatDB::Common::HashTable::hash(Vector<Expression_s>& cols, const Row_s & row)
{
	Hash<u32> hash;
	u32 value = 0;
	if (cols.empty()){
		for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i){
			Object_s obj;
			row->get_cell(i, obj);
			value = hash(value + obj->hash());
		}
	}else {
		for (u32 i = 0; i < cols.size(); ++i){
			Object_s obj = cols[i]->get_result(row);
			if (obj->get_type() == T_ERROR_RESULT){
				//Log(LOG_ERR, "HashTable", "error hash column found");
				continue;
			}
			value = hash(value + obj->hash());
		}
	}
	return value;
}

bool CatDB::Common::HashTable::equal(const Row_s & lhs, const Row_s & rhs)
{
	bool equal = true;
	if (probe_cols.empty()){
		if (lhs->get_row_desc().get_column_num() != rhs->get_row_desc().get_column_num()){
			Log(LOG_ERR, "HashTable", "two row have different count column");
			return false;
		}
		for (u32 i = 0; i < lhs->get_row_desc().get_column_num(); ++i){
			Object_s r_lhs, r_rhs;
			lhs->get_cell(i, r_lhs);
			rhs->get_cell(i, r_rhs);
			Object_s result = r_lhs->operator==(r_rhs);
			if (!result->bool_value()){
				return false;
			}
		}
		return true;
	}else{
		for (u32 i = 0; i < probe_cols.size(); ++i){
			Object_s r_lhs = probe_cols[i]->get_result(lhs);
			Object_s r_rhs = probe_cols[i]->get_result(rhs);
			Object_s result = r_lhs->operator==(r_rhs);
			if (!result->bool_value()){
				return false;
			}
		}
		return true;
	}
}
