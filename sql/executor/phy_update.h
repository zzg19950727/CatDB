#ifndef PHY_UPDATE_H
#define PHY_UPDATE_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
	}
    namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		using Storage::TableSpace_s;
        using Common::Row_s;
		DECLARE(Expression);
        
		class PhyUpdate : public SingleChildPhyOperator
		{
		private:
			PhyUpdate() = delete;
			PhyUpdate(PhyOperator_s& child,const TableSpace_s& table_space);
		public:
			~PhyUpdate();
			static PhyOperator_s make_update(PhyOperator_s& child,
											const String&database,
											const String& table,
											const Vector<String> &engine_args,
											const Expression_s& row_id,
											const Vector<Expression_s>& value_exprs);
			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row() override;
			u32 type() const override;
			void set_update_desc(const RowDesc& desc);
			void set_access_desc(const RowDesc &desc);
		private:
			TableSpace_s table_space;
			Expression_s row_id;
			Vector<Expression_s> value_exprs;
			Row_s update_row;
			Row_s access_row;
			RowDesc access_desc;
			RowDesc update_desc;
		};
	}
}

#endif	//PHY_UPDATE_H