#ifndef PHY_SUBQUERY_EVALUATE_H
#define PHY_SUBQUERY_EVALUATE_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Sql {
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
			u32 inner_get_next_row(Row_s &row) override;
			u32 type() const override;
		};
	}
}

#endif	//PHY_SUBQUERY_EVALUATE_H
