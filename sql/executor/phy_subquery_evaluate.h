#ifndef PHY_SUBQUERY_EVALUATE_H
#define PHY_SUBQUERY_EVALUATE_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
        DECLARE(PhySubqueryEvaluate);
		class PhySubqueryEvaluate : public MultiChildPhyOperator
		{
		private:
			PhySubqueryEvaluate() = delete;
			PhySubqueryEvaluate(const PhyOperator_s& first_child);
		public:
			~PhySubqueryEvaluate();
			static PhyOperator_s make_subquery_evaluate(const PhyOperator_s& first_child);
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;

			u32 reset_subplan(u32 subplan_id);
			u32 get_subplan_next_row(u32 subplan_id, Row_s &row);
		};
	}
}

#endif	//PHY_SUBQUERY_EVALUATE_H
