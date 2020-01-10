#ifndef PLAN_FILTER_H
#define PLAN_FILTER_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Filter);

		class PlanFilter : public SingleChildPhyOperator
		{
		private:
			PlanFilter() = delete;
			PlanFilter(PhyOperator_s& child, const Filter_s& filter);
		public:
			static PhyOperator_s make_plan_filter(PhyOperator_s& child, const Filter_s& filter);
			//物理算子必须提供的接口
			u32 open();
			u32 close();
			u32 reset();
			u32 reopen(const Row_s& row);
			u32 get_next_row(Row_s &row);
			u32 type() const;
			u32 explain_operator(u32 depth, QueryResult* result)override;
		private:
			Filter_s filter;
		};
	}
}

#endif	//PLAN_FILTER_H
