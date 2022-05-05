#ifndef PHY_WINDOW_FUNC_H
#define PHY_WINDOW_FUNC_H
#include "phy_win_expression.h"
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);

		class PhyWindowFunc : public SingleChildPhyOperator, public WindowFuncCalculator
		{
		public:
			PhyWindowFunc() = delete;
			PhyWindowFunc(PhyOperator_s& child,
							Vector<Expression_s> &win_items,
                            Vector<Expression_s> &partition_by_exprs);
		public:
			~PhyWindowFunc();
			static PhyOperator_s make_window_func(PhyOperator_s& child,
												   Vector<Expression_s> &win_items,
                                                   Vector<Expression_s> &partition_by_exprs);

			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
		private:
            u32 is_new_window(bool &is_new);

            Vector<Expression_s> partition_by_exprs;
            Vector<Object_s> partition_by_values;
            Vector<Row_s> cur_window_rows;
            Row_s last_row;
			Row_s win_result_row;
            int row_pos;
		};

	}
}

#endif	//PHY_WINDOW_FUNC_H