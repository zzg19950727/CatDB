#ifndef PHY_SORT_H
#define PHY_SORT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);
		DECLARE(HashTable);

		class PhySort : public SingleChildPhyOperator
		{
		protected:
			PhySort() = delete;
			PhySort(PhyOperator_s& child);
		public:
			static PhyOperator_s make_sort(PhyOperator_s& child, 
										const Vector<Expression_s>& sort_exprs, 
										const Vector<bool> &asc);
			//物理算子必须提供的接口
			virtual u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			virtual u32 inner_get_next_row() override;
			u32 type() const override;
		protected:
			u32 quick_sort(Vector<Row_s> &arr, int begin, int end);
			bool compare(const Row_s& lhs, const Row_s& rhs);
			virtual u32 sort_rows();
			Vector<Row_s> rows;
			Vector<Expression_s> sort_exprs;
			Vector<bool> asc;
			bool null_first;
			//当前读取状态
			u32 pos;
			bool is_start;
		};

		class PhyTopNSort : public PhySort
		{
		public:
			PhyTopNSort() = delete;
			PhyTopNSort(PhyOperator_s& child, u32 topn);

			static PhyOperator_s make_topn_sort(PhyOperator_s& child,
												const Vector<Expression_s>& sort_exprs,
												const Vector<bool> &asc,
												u32 topn);
			u32 type() const override;
		private:
			u32 sort_rows() override;
			u32 topn;
		};

		class PhyPartitionSort : public PhySort
		{
		public:
			PhyPartitionSort() = delete;
			PhyPartitionSort(PhyOperator_s& child, const Vector<Expression_s>& partition_exprs);

			static PhyOperator_s make_partition_sort(PhyOperator_s& child,
													const Vector<Expression_s>& sort_exprs,
													const Vector<bool> &asc,
													const Vector<Expression_s>& partition_exprs);
			u32 type() const override;
			virtual u32 inner_open() override;
			virtual u32 inner_get_next_row() override;
		private:
			u32 sort_rows() override;
			Vector<Expression_s> partition_exprs;
			HashTable_s hash_table;
		};
	}
}

#endif	//PHY_SORT_H
