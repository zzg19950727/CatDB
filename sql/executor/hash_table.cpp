#include "hash_table.h"
#include "phy_expression.h"
#include "phy_filter.h"
#include "object.h"
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
	num = 50000;
	this->bucket_num = cal_next_prime(num);
	buckets = TableType(this->bucket_num);
	cur_bucket = buckets.begin();
	cur_bucket_pos = 0;
}

HashTable::~HashTable()
{

}

void HashTable::clear()
{
	for (u32 i = 0; i < buckets.size(); ++i) {
		buckets[i].clear();
	}
	is_empty = true; 
	cur_bucket = buckets.begin();
	cur_bucket_pos = 0;
}

bool HashTable::empty() const
{
	return is_empty;
}

u32 HashTable::build(const Row_s & row)
{
	u32 hash_value = hash(hash_exprs, row);
	buckets[hash_value].push_back(Element(row, false));
	is_empty = false;
	return SUCCESS;
}

u32 HashTable::probe(const Row_s & row, bool ignore_mark)
{
	u32 hash_value = hash(probe_exprs, row);
	const auto& bucket = buckets[hash_value];
	for (auto iter = bucket.cbegin(); iter != bucket.cend(); ++iter) {
		if (ignore_mark && iter->second) {
			continue;
		} else if (!filter_row(iter->first, row)) {
			continue;
		} else if (equal(iter->first, row))
			return SUCCESS;
	}
	return ROW_NOT_FOUND;
}

u32 HashTable::probe_all_rows(const Row_s & row, Queue<Row_s>& out_rows, bool ignore_mark)
{
	u32 hash_value = hash(probe_exprs, row);
	auto& bucket = buckets[hash_value];
	for (auto iter = bucket.begin(); iter != bucket.end(); ++iter) {
		if (ignore_mark && iter->second) {
			continue;
		} else if (filter_row(iter->first, row)) {
			continue;
		} else if (equal(iter->first, row)) {
			out_rows.push(iter->first);
			iter->second = true;
		}
	}
	if (out_rows.empty()){
		return ROW_NOT_FOUND;
	} else {
		return SUCCESS;
	}
}

u32 HashTable::probe_all_rows(const Row_s & row, bool ignore_mark)
{
	u32 hash_value = hash(probe_exprs, row);
	auto& bucket = buckets[hash_value];
	bool is_empty = true;
	for (auto iter = bucket.begin(); iter != bucket.end(); ++iter) {
		if (ignore_mark && iter->second) {
			continue;
		} else if (filter_row(iter->first, row)) {
			continue;
		} else if (equal(iter->first, row)) {
			is_empty = false;
			iter->second = true;
		}
	}
	if (is_empty){
		return ROW_NOT_FOUND;
	} else {
		return SUCCESS;
	}
}

u32 HashTable::set_hash_exprs(const Vector<Expression_s> & exprs)
{
	hash_exprs = exprs;
	return SUCCESS;
}

u32 HashTable::set_probe_exprs(const Vector<Expression_s> & exprs)
{
	probe_exprs = exprs;
	return SUCCESS;
}

u32 HashTable::set_hash_expr(Expression_s & expr)
{
	hash_exprs.push_back(expr);
	return SUCCESS;
}

u32 HashTable::set_probe_expr(Expression_s & expr)
{
	probe_exprs.push_back(expr);
	return SUCCESS;
}

u32 HashTable::set_other_condition(const Vector<Expression_s>& exprs)
{
	other_condition = PhyFilter::make_filter(exprs);
	return SUCCESS;
}

u32 HashTable::sort_bucket()
{
	auto less_func = std::bind(&HashTable::less, this, std::placeholders::_1, std::placeholders::_2);
	for (u32 i = 0; i < buckets.size(); ++i) {
		if (!buckets[i].empty()) {
			std::sort(buckets[i].begin(), buckets[i].end(), less_func);
		}
	}
	return SUCCESS;
}

u32 HashTable::get_next_row(Row_s& row, bool ignore_mark)
{
	u32 ret = SUCCESS;
	bool stop = false;
	while (!stop) {
		if (cur_bucket == buckets.end()) {
			ret = NO_MORE_ROWS;
			stop = true;
		} else if (cur_bucket_pos >= cur_bucket->size()) {
			++cur_bucket;
			cur_bucket_pos = 0;
		} else if (ignore_mark && cur_bucket->at(cur_bucket_pos).second) {
			++cur_bucket_pos;
		} else {
			row = cur_bucket->at(cur_bucket_pos).first;
			stop = true;
			++cur_bucket_pos;
		}
	}
	return ret;
}

bool HashTable::less(const Element & lhs, const Element & rhs)
{
	for (u32 i = 0; i < hash_exprs.size(); ++i){
		Object_s left = hash_exprs[i]->get_result(lhs.first);
		Object_s right = hash_exprs[i]->get_result(rhs.first);
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

u32 HashTable::hash(Vector<Expression_s>& exprs, const Row_s & row)
{
	Hash<u32> hash;
	u32 value = 0;
	if (exprs.empty()) {
		for (u32 i = 0; i < row->get_row_desc().get_column_num(); ++i){
			Object_s obj;
			row->get_cell(i, obj);
			value = hash(value + obj->hash());
		}
	} else {
		for (u32 i = 0; i < exprs.size(); ++i) {
			Object_s obj = exprs[i]->get_result(row);
			if (obj->get_type() == T_ERROR_RESULT) {
				LOG_ERR("error hash column found", K(obj));
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
	if (probe_exprs.empty()) {
		return lhs->equal(rhs);
	} else if (probe_exprs.size() != hash_exprs.size()) {
		return false;
	} else {
		for (u32 i = 0; i < probe_exprs.size(); ++i) {
			Object_s r_lhs = hash_exprs[i]->get_result(lhs);
			Object_s r_rhs = probe_exprs[i]->get_result(rhs);
			if (r_lhs->is_null() && r_rhs->is_null()) {
				continue;
			}
			Object_s result = r_lhs->operator==(r_rhs);
			if (!result->bool_value()) {
				return false;
			}
		}
		return true;
	}
}

bool HashTable::filter_row(const Row_s& left_row, const Row_s& right_row)
{
	if (!other_condition) {
		return true;
	} else {
		Row_s agent_row = RowAgent::make_agent_row(left_row, right_row);
		return (*other_condition)(agent_row);
	}
}
