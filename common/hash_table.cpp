#include "hash_table.h"
#include "expression.h"
#include "Object.h"
#include "error.h"
#include "row.h"
#include "log.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

enum { PRIME_NUM = 24 };
static const u32 PRIME_LIST[PRIME_NUM] =
{
	53l,         97l,         193l,       389l,       769l,
	1543l,       3079l,       6151l,      12289l,     24593l,
	49157l,      98317l,      196613l,    393241l,    786433l,
	1572869l,    3145739l,    6291469l,   12582917l,  25165843l,
	50331653l,   100663319l,  201326611l, 402653189l
};
inline u32 cal_next_prime(u32 n)
{
	u32 ret = n;
	if (n > 0)
	{
		const u32* first = PRIME_LIST;
		const u32* last = PRIME_LIST + PRIME_NUM;
		const u32* pos = std::lower_bound(first, last, n);
		ret = ((pos == last) ? *(last - 1) : *pos);
	}
	return ret;
}

HashTable::HashTable(u32 num)
	:is_empty(true)
{
	num = 6000000;
	this->bucket_num = cal_next_prime(num);
}

HashTable::~HashTable()
{

}

void HashTable::clear()
{
	buckets.clear();
}

bool HashTable::empty() const
{
	return is_empty;
}

u32 HashTable::build(const Row_s & row)
{
	u32 hash_value = hash(hash_cols, row);
	buckets[hash_value].push_back(row);
	is_empty = false;
	return SUCCESS;
}

u32 HashTable::probe(const Row_s & row)
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

u32 HashTable::probe(const Row_s & row, Queue<Row_s>& out_rows)
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

u32 HashTable::probe_and_drop(const Row_s & row)
{
	u32 hash_value = hash(probe_cols, row);
	bool found = false;
	if (buckets.find(hash_value) == buckets.cend()) {
		return ROW_NOT_FOUND;
	}
	else {
		const auto& bucket = buckets[hash_value];
		Vector<Row_s> new_bucket;
		for (auto iter = bucket.cbegin(); iter != bucket.cend(); ++iter) {
			Row_s new_row = RowAgent::make_agent_row(row, *iter);
			Object_s result = probe_condition->get_result(new_row);
			if (result->get_type() == T_ERROR_RESULT) {
				Log(LOG_WARN, "HashTable", "probe condition calc error");
				new_bucket.push_back(*iter);
				continue;
			}
			else if (result->bool_value()) {
				found = true;
				continue;
			}
			else {
				new_bucket.push_back(*iter);
			}
		}
		buckets[hash_value] = new_bucket;
	}
	if (!found) {
		return ROW_NOT_FOUND;
	}
	else {
		return SUCCESS;
	}
}

u32 HashTable::add_hash_column(const Expression_s & col_expr)
{
	hash_cols.push_back(col_expr);
	return SUCCESS;
}

void HashTable::clear_hash_columns()
{
	hash_cols.clear();
}

u32 HashTable::add_probe_column(const Expression_s & col_expr)
{
	probe_cols.push_back(col_expr);
	return SUCCESS;
}

void HashTable::clear_probe_columns()
{
	probe_cols.clear();
}

u32 HashTable::set_probe_condition(const Expression_s & probe_expr)
{
	probe_condition = probe_expr;
	return SUCCESS;
}

u32 HashTable::bucket_count() const
{
	return buckets.size();
}

HashTable::BucketIterator HashTable::begin_bucket()
{
	if (buckets.empty()) {
		return buckets.end();
	}
	auto iter =  buckets.begin();
	if (!iter->second.empty()) {
		return iter;
	}
	while (iter != buckets.end()) {
		if (!iter->second.empty()) {
			return iter;
		}
		else {
			++iter;
		}
	}
	return iter;
}

HashTable::BucketIterator HashTable::end_bucket()
{
	return buckets.end();
}

u32 HashTable::sort_bucket(BucketIterator & iter)
{
	if (iter == buckets.end()) {
		Log(LOG_ERR, "HashTable", "end of buckets");
		return ERR_BUCKET_IDX;
	}
	auto less_func = std::bind(&HashTable::less, this, std::placeholders::_1, std::placeholders::_2);
	std::sort(iter->second.begin(), iter->second.end(), less_func);
	return SUCCESS;
}

const Vector<Row_s>& HashTable::bucket(const BucketIterator & iter)
{
	if (iter == buckets.end()) {
		Log(LOG_TRACE, "HashTable", "end of buckets");
		//TODO 错误处理
	}
	return iter->second;
}

bool HashTable::bucket_empty(const BucketIterator & iter)
{
	if (iter == buckets.end()) {
		return true;
	}
	else {
		return iter->second.empty();
	}
}

u32 HashTable::next_none_empty_bucket(BucketIterator & iter)
{
	while (iter != buckets.end()) {
		++iter;
		if (iter != buckets.end() && !iter->second.empty()) {
			return SUCCESS;
		}
	}
	return END_OF_BUCKETS;
}

bool HashTable::less(const Row_s & lhs, const Row_s & rhs)
{
	for (u32 i = 0; i < hash_cols.size(); ++i){
		Object_s left = hash_cols[i]->get_result(lhs);
		Object_s right = hash_cols[i]->get_result(rhs);
		if (left->is_null() && right->is_null()) {
			continue;
		}
		else if (left->is_null()) {
			return true;
		}
		else if (right->is_null()) {
			return false;
		}
		else {
			Object_s result = left->operator==(right);
			if (result->bool_value())
				continue;
			result = left->operator>(right);
			if (result->bool_value())
				return false;
			else
				return true;
		}
	}
	return false;
}

u32 HashTable::hash(Vector<Expression_s>& cols, const Row_s & row)
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
	return value % bucket_num;
}

bool HashTable::equal(const Row_s & lhs, const Row_s & rhs)
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
			if (r_lhs->is_null() && r_rhs->is_null()) {
				return true;
			}
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
			if (r_lhs->is_null() && r_rhs->is_null()) {
				return true;
			}
			Object_s result = r_lhs->operator==(r_rhs);
			if (!result->bool_value()){
				return false;
			}
		}
		return true;
	}
}
