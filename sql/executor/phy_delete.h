#ifndef PHY_DELETE_H
#define PHY_DELETE_H
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
        
		class PhyDelete : public SingleChildPhyOperator
		{
		private:
			PhyDelete() = delete;
			PhyDelete(PhyOperator_s &child, const TableSpace_s& table_space);
		public:
			~PhyDelete();
            static PhyOperator_s make_delete(PhyOperator_s &child,
											const String&database,
											const String& table,
											const Vector<String> &engine_args,
											const Expression_s& row_id,
											bool is_delete_all = false);
			//物理算子必须提供的接口
			u32 inner_open() override;
			u32 close() override;
			u32 reset() override;
			u32 inner_get_next_row(Row_s &row) override;
			u32 type() const override;
		private:
			TableSpace_s table_space;
			Expression_s row_id;
			bool is_delete_all;
		};
	}
}

#endif	//PHY_DELETE_H