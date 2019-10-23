#ifndef SCALAR_GROUP_H
#define SCALAR_GROUP_H

#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Row_s;
		using Common::Object_s;
		DECLARE(Filter);
		DECLARE(Expression);

		class ScalarGroup : public SingleChildPhyOperator
		{
		public:
			ScalarGroup() = delete;
			ScalarGroup(PhyOperator_s& child);
		public:
			~ScalarGroup();
			static PhyOperator_s make_scalar_group(PhyOperator_s& child, 
				const Expression_s& agg_expr);
			static PhyOperator_s make_scalar_group(PhyOperator_s& child, 
				const Expression_s& agg_expr, 
				const Filter_s& filter);
			u32 set_agg_expr(const Expression_s& expr);

			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;

		private:
			void set_filter(const Filter_s& filter);
			Row_s make_row(const Object_s& result);
			u32 calc_aggregate();

			Expression_s agg_func_expr;
			u32 alias_table_id;
			Object_s result;
			Filter_s filter;
		};

	}
}

#endif	//SCALAR_GROUP_H