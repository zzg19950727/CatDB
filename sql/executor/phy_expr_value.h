#ifndef PHY_EXPR_VALUE_H
#define PHY_EXPR_VALUE_H
#include "phy_operator.h"

namespace CatDB {
    namespace Common {
        DECLARE(Object);
    }
	namespace Sql {
        using Common::Object_s;
        using Common::Row_s;
        using Common::RowDesc;

		class PhyExprValue : public PhyOperator
		{
		private:
			PhyExprValue();
		public:
			~PhyExprValue();
			static PhyOperator_s make_expr_value();
			//物理算子必须提供的接口
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 inner_get_next_row(Row_s &row);
            u32 add_value(Vector<Expression_s> &value);
			u32 type() const override;
			void set_access_desc(const RowDesc &desc) {access_desc = desc;}
		private:
			Vector<Vector<Expression_s>> value_list;
			RowDesc access_desc;
            u32 pos;
		};
	}
}

#endif	//PHY_EXPR_VALUE_H
