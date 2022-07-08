#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);
		DECLARE(ExecCtx);
		DECLARE(HashTable);

		class HashTable {
		private:
			struct RowNode {
				RowNode(const Row_s &row)
					:next(NULL),
					row(row),
					mark(false)
				{}
				inline void mark_row() { mark = true; }
				
				RowNode *next;
				Row_s row;
				bool mark;
			};
			struct BucketNode {
				BucketNode()
					:next(NULL),
					head(NULL)
				{}

				BucketNode *next;
				RowNode *head;
			};
			typedef Vector<BucketNode*> TableType;
		public:
			struct RowIterator {
				RowIterator()
					:last(NULL),
					cur(NULL)
				{}
				inline void mark_last_row()
				{
					if (last) {
						last->mark_row();
					}
				}
				inline bool get_next_row(Row_s &row)
				{
					if (cur) {
						row = cur->row;
						last = cur;
						cur = cur->next;
						return true;
					} else {
						return false;
					}
				}
				inline bool get_next_no_mark_row(Row_s &row)
				{
					bool b_ret = false;
					while (cur) {
						if (!cur->mark) {
							row = cur->row;
							last = cur;
							cur = cur->next;
							b_ret = true;
							break;
						} else {
							cur = cur->next;
						}
					}
					return b_ret;
				}
				inline bool get_next_mark_row(Row_s &row)
				{
					bool b_ret = false;
					while (cur) {
						if (cur->mark) {
							row = cur->row;
							last = cur;
							cur = cur->next;
							b_ret = true;
							break;
						} else {
							cur = cur->next;
						}
					}
					return b_ret;
				}

				RowNode *last;
				RowNode *cur;
			};
		private:
			struct BucketIterator {
				BucketIterator()
					:cur_bucket_pos(-1),
					bucket_node(NULL)
				{ }
				inline bool get_next_bucket(TableType& hash_table, RowIterator &iter)
				{
					bool b_ret = false;
					while (cur_bucket_pos < (int)hash_table.size()) {
						if (bucket_node) {
							iter.cur = bucket_node->head;
							bucket_node = bucket_node->next;
							b_ret = true;
							break;
						} else {
							++cur_bucket_pos;
							if (cur_bucket_pos < (int)hash_table.size()) {
								bucket_node = hash_table[cur_bucket_pos];
							}
						}
					}
					return b_ret;
				}
				
				int cur_bucket_pos;
				BucketNode *bucket_node;
			};
		public:
			HashTable(u32 bucket_num =10000);
			~HashTable();
			static HashTable_s make_hash_table(u32 bucket_num =10000);
			void reset();
			void set_exec_ctx(const ExecCtx_s& ctx);
			void set_null_safe(bool value) { null_safe = value; }

			u32 build(const Row_s& row);
			u32 build_without_check(const Row_s& row);
			
			u32 probe(const Row_s& row, bool &hit);
			u32 probe_bucket(const Row_s& row, RowIterator &iter, bool &hit);

			u32 set_hash_exprs(const Vector<Expression_s>& exprs);
			u32 set_probe_exprs(const Vector<Expression_s>& exprs);
			u32 set_hash_expr(Expression_s& expr);
			u32 set_probe_expr(Expression_s& expr);

			inline bool get_next_bucket(RowIterator &iter)
			{
				return bucket_iter.get_next_bucket(buckets, iter);
			}

		private:
			u32 hash(Vector<Expression_s>& exprs, const Row_s& row);
			u32 equal(const Row_s& lhs, const Row_s& rhs, bool &is_valid);
			u32 equal_bucket(const Row_s& lhs, const Row_s& rhs, bool &is_valid);

			TableType buckets;
			Vector<Expression_s> hash_exprs;
			Vector<Expression_s> probe_exprs;
			BucketIterator bucket_iter;
			RowIterator row_iter;
			ExecCtx_s exec_ctx;
			u32 bucket_num;
			bool null_safe;
		};
	}
}

#endif	//HASH_TABLE_H
