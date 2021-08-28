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
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 inner_get_next_row(Row_s &row);
			u32 type() const;
		protected:
			bool less(const Row_s& lhs, const Row_s& rhs)const;
			bool greater(const Row_s& lhs, const Row_s& rhs)const;
			bool compare(const Row_s& lhs, const Row_s& rhs)const;
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
			u32 type() const;
		private:
			u32 sort_rows() override;
			u32 topn;
		};
	}
}

#endif	//PHY_SORT_H
