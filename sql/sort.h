#ifndef SORT_H
#define SORT_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);

		class Sort : public SingleChildPhyOperator
		{
		private:
			Sort() = delete;
			Sort(PhyOperator_s& child);
		public:
			static PhyOperator_s make_sort(PhyOperator_s& child, 
				const Vector<Expression_s>& sort_cols, 
				bool asc = true);
			u32 add_sort_columns(const Expression_s& expr);
			void set_asc(bool asc);
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
		
		private:
			bool less(const Row_s& lhs, const Row_s& rhs)const;
			bool greater(const Row_s& lhs, const Row_s& rhs)const;
			u32 sort_rows();
			Vector<Row_s> rows;
			Vector<Expression_s> sort_cols;
			//当前读取状态
			u32 pos;
			bool asc;
		};

		class TopNSort : public SingleChildPhyOperator
		{
		public:
			TopNSort() = delete;
			TopNSort(PhyOperator_s& child, u32 topn);

			static PhyOperator_s make_topn_sort(PhyOperator_s& child,
				const Vector<Expression_s>& sort_cols,
				u32 topn,
				bool asc = true);
			u32 add_sort_columns(const Expression_s& expr);
			void set_asc(bool asc);
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
		
			bool greater(const Row_s& lhs, const Row_s& rhs)const;
			bool less(const Row_s& lhs, const Row_s& rhs)const;
		private:
			u32 sort_rows();
			Vector<Row_s> rows;
			Vector<Expression_s> sort_cols;
			//当前读取状态
			u32 pos;
			u32 topn;
			//是否要求是升序
			bool asc;
		};
	}
}

#endif	//SORT_H
