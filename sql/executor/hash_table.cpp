#include "hash_table.h"
#include "phy_expression.h"
#include "phy_operator.h"
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
:null_safe(true)
{
	num = 50000;
	this->bucket_num = cal_next_prime(num);
	buckets = TableType(this->bucket_num);
}

HashTable::~HashTable()
{
	reset();
}

HashTable_s HashTable::make_hash_table(u32 bucket_num)
{
	return HashTable_s(new HashTable(bucket_num));
}

void HashTable::reset()
{
	for (u32 i = 0; i < buckets.size(); ++i) {
		BucketNode *bucket_node = buckets[i];
		BucketNode *temp1 = NULL;
		RowNode *row_node = NULL;
		RowNode *temp2 = NULL;
		while (bucket_node) {
				temp1 = bucket_node->next;
				row_node = bucket_node->head;
				while (row_node) {
						temp2 = row_node->next;
						delete row_node;
						row_node = temp2;
				}
				bucket_node->head = NULL;
				delete bucket_node;
				bucket_node = temp1;
		}
		buckets[i] = NULL;
	}
}

void HashTable::set_exec_ctx(const ExecCtx_s& ctx)
{
	exec_ctx = ExecCtx::make_exec_ctx(ctx->param_store);
}

u32 HashTable::build(const Row_s & row)
{
	u32 ret = SUCCESS;
	u32 hash_value = hash(hash_exprs, row);
	BucketNode *bucket_node = buckets[hash_value];
	bool is_equal = false;
	while (bucket_node) {
		CHECK(equal_bucket(bucket_node->head->row, row, is_equal));
		if (is_equal) {
			RowNode *row_node = new RowNode(row);
			row_node->next = bucket_node->head;
			bucket_node->head = row_node;
			break;
		} else {
			bucket_node = bucket_node->next;
		}
	}
	if (!bucket_node) {
		BucketNode *bucket_node = new BucketNode;
		RowNode *head = new RowNode(row);
		bucket_node->head = head;
		bucket_node->next = buckets[hash_value];
		buckets[hash_value] = bucket_node;
	}
	return ret;
}

u32 HashTable::build_without_check(const Row_s & row)
{
	u32 ret = SUCCESS;
	u32 hash_value = hash(hash_exprs, row);
	BucketNode *bucket_node = new BucketNode;
	RowNode *head = new RowNode(row);
	bucket_node->head = head;
	bucket_node->next = buckets[hash_value];
	buckets[hash_value] = bucket_node;
	return ret;
}

u32 HashTable::probe(const Row_s & row, bool &hit)
{
	u32 ret = SUCCESS;
	hit = false;
	u32 hash_value = hash(probe_exprs, row);
	BucketNode* bucket_node = buckets[hash_value];
	bool is_equal = false;
	while (bucket_node) {
		RowNode *row_node = bucket_node->head;
		CHECK(equal(row_node->row, row, is_equal));
		if (!is_equal) {
			bucket_node = bucket_node->next;
			continue;
		} else {
			hit = true;
			break;
		}
	}
	return ret;
}

u32 HashTable::probe_bucket(const Row_s& row, RowIterator &iter, bool &hit)
{
	u32 ret = SUCCESS;
	hit = false;
	u32 hash_value = hash(probe_exprs, row);
	BucketNode* bucket_node = buckets[hash_value];
	bool is_equal = false;
	while (bucket_node) {
		RowNode *row_node = bucket_node->head;
		CHECK(equal(row_node->row, row, is_equal));
		if (!is_equal) {
			bucket_node = bucket_node->next;
			continue;
		} else {
			hit = true;
			iter.cur = row_node;
			break;
		}
	}
	return ret;
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

u32 HashTable::hash(Vector<Expression_s>& exprs, const Row_s & row)
{
	u32 ret = SUCCESS;
	Hash<u32> hash;
	u32 value = 0;
	if (exprs.empty()) {
		Object_s obj;
		for (u32 i = 0; i < row->get_cell_num(); ++i){
			row->get_cell(i, obj);
			value = hash(value + obj->hash());
		}
	} else {
		CHECK(exec_ctx->set_input_rows(row));
		for (u32 i = 0; i < exprs.size(); ++i) {
			CHECK(exprs[i]->get_result(exec_ctx));
			value = hash(value + exec_ctx->output_result->hash());
		}
	}
	return value % bucket_num;
}

u32 HashTable::equal(const Row_s & lhs, const Row_s & rhs, bool &is_valid)
{
	u32 ret = SUCCESS;
	is_valid = false;
	if (probe_exprs.empty()) {
		CHECK(lhs->equal(rhs, null_safe, is_valid));
		LOG_ERR("zzg:", K(lhs), K(rhs), K(is_valid));
	} else {
		int res = 0;
		Object_s l_obj, r_obj;
		for (u32 i = 0; i < probe_exprs.size(); ++i) {
			CHECK(exec_ctx->set_input_rows(lhs));
			CHECK(hash_exprs[i]->get_result(exec_ctx));
			l_obj = exec_ctx->output_result;
			CHECK(exec_ctx->set_input_rows(rhs));
			CHECK(probe_exprs[i]->get_result(exec_ctx));
			r_obj = exec_ctx->output_result;
			if (null_safe &&
				l_obj->is_null() && 
				r_obj->is_null()) {
				is_valid = true;
			} else {
				CHECK(l_obj->compare(r_obj, res));
				is_valid = CMP_RES_EQ == res;
			}
		}
	}
	return ret;
}

u32 HashTable::equal_bucket(const Row_s & lhs, const Row_s & rhs, bool &is_valid)
{
	u32 ret = SUCCESS;
	is_valid = false;
	if (hash_exprs.empty()) {
		CHECK(lhs->equal(rhs, true, is_valid));
	} else {
		int res = 0;
		Object_s l_obj, r_obj;
		for (u32 i = 0; i < hash_exprs.size(); ++i) {
			CHECK(exec_ctx->set_input_rows(lhs));
			CHECK(hash_exprs[i]->get_result(exec_ctx));
			l_obj = exec_ctx->output_result;
			CHECK(exec_ctx->set_input_rows(rhs));
			CHECK(hash_exprs[i]->get_result(exec_ctx));
			r_obj = exec_ctx->output_result;
			if (l_obj->is_null() && 
				r_obj->is_null()) {
				is_valid = true;
			} else {
				CHECK(l_obj->compare(r_obj, res));
				is_valid = CMP_RES_EQ == res;
			}
		}
	}
	return ret;
}