#ifndef PHY_HASH_GROUP_H
#define PHY_HASH_GROUP_H

#include "phy_operator.h"
#include "hash_table.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Row_s;
		using Common::Object_s;
		using Common::HashTable;
		DECLARE(Filter);
		DECLARE(Expression);

		class PhyHashGroup : public SingleChildPhyOperator
		{
		private:
			PhyHashGroup() = delete;
			PhyHashGroup(PhyOperator_s& child);
		public:
			~PhyHashGroup();
			static PhyOperator_s make_hash_group(PhyOperator_s& child, 
				const Vector<Expression_s>& group_exprs,
				const Vector<Expression_s>& agg_funcs);

			//物理算子必须提供的接口
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 inner_get_next_row(Row_s &row);
			u32 type() const;
		private:
			u32 build_hash_table();
			bool euqal(const Row_s& lhs, const Row_s& rhs);
			void reset_agg_func();
			u32 add_row_to_agg_func(const Row_s& row);
		private:
			Common::HashTable hash_table;
			Vector<Expression_s> group_exprs;
			Vector<Expression_s> agg_funcs;
			//当前集合函数计算状态
			Row_s first_group_row;
			//当没有输入的时候需要输出一行结果
			bool is_build_hash_table;
			bool start_new_group;
		};

	}
}

#endif	//PHY_HASH_GROUP_H