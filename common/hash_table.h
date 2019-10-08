#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "type.h"

namespace CatDB {
	namespace Sql {
		DECLARE(Expression);
	}
	namespace Common {
		using Sql::Expression_s;
		DECLARE(Row);

		class HashTable
		{
		public:
			HashTable();
			~HashTable();
			void clear();
			bool empty()const;
			u32 build(const Row_s& row);
			//hash distinct、hash group使用
			u32 probe(const Row_s& row);
			//hash join使用
			u32 probe(const Row_s& row, Queue<Row_s>& out_rows);
			u32 add_hash_column(const Expression_s& col_expr);
			void clear_hash_columns();
			u32 add_probe_column(const Expression_s& col_expr);
			void clear_probe_columns();
			u32 set_probe_condition(const Expression_s& probe_expr);
			u32 bucket_count()const;
			u32 sort_bucket(u32 idx);
			const Vector<Row_s>& bucket(u32 idx);

		private:
			bool less(const Row_s& lhs, const Row_s& rhs);
			u32 hash(Vector<Expression_s>& cols, const Row_s& row);
			bool equal(const Row_s& lhs, const Row_s& rhs);

			HashMap<u32, Vector<Row_s>> buckets;
			Vector<Expression_s> hash_cols;
			Vector<Expression_s> probe_cols;
			Expression_s probe_condition;
		};
	}
}

#endif	//HASH_TABLE_H
