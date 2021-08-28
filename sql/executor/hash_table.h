#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "type.h"

namespace CatDB {
	namespace Sql {
		DECLARE(Expression);
		DECLARE(PhyFilter);
	}
	namespace Common {
		using Sql::Expression_s;
		using Sql::PhyFilter_s;
		DECLARE(Row);

		class HashTable
		{
		public:
			typedef Pair<Row_s, bool> Element;
			typedef Vector<Vector<Element>> TableType;
			typedef TableType::iterator BucketIterator;
			HashTable(u32 bucket_num =10000);
			~HashTable();
			void clear();
			bool empty()const;

			u32 build(const Row_s& row);
			//hash distinct、hash group使用
			u32 probe(const Row_s& row, bool ignore_mark = false);
			//hash join使用
			u32 probe_all_rows(const Row_s& row, Queue<Row_s>& out_rows, bool ignore_mark = false);
			u32 probe_all_rows(const Row_s& row, bool ignore_mark = false);

			u32 set_hash_exprs(const Vector<Expression_s>& exprs);
			u32 set_probe_exprs(const Vector<Expression_s>& exprs);
			u32 set_hash_expr(Expression_s& expr);
			u32 set_probe_expr(Expression_s& expr);
			u32 set_other_condition(const Vector<Expression_s>& expr);

			u32 sort_bucket();
			u32 get_next_row(Row_s& row, bool ignore_mark = false);

		private:
			bool less(const Element& lhs, const Element& rhs);
			u32 hash(Vector<Expression_s>& exprs, const Row_s& row);
			bool equal(const Row_s& lhs, const Row_s& rhs);
			inline bool filter_row(const Row_s& left_row, const Row_s& right_row);

			TableType buckets;
			Vector<Expression_s> hash_exprs;
			Vector<Expression_s> probe_exprs;
			PhyFilter_s other_condition;
			BucketIterator cur_bucket;
			u32 cur_bucket_pos;
			u32 bucket_num;
			bool is_empty;
		};
	}
}

#endif	//HASH_TABLE_H
