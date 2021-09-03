#ifndef PHY_VIEW_H
#define PHY_VIEW_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Common::Row_s;
		DECLARE(Expression);
		class PhyView : public SingleChildPhyOperator
		{
		private:
			PhyView() = delete;
			PhyView(PhyOperator_s& child);
		public:
			static PhyOperator_s make_view(PhyOperator_s& child);
			
			u32 inner_open();
			u32 close();
			u32 reset();
			u32 inner_get_next_row(Row_s &row);
			u32 type() const;
		};
	}
}

#endif	//PHY_VIEW_H