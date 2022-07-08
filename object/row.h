#ifndef ROW_H
#define ROW_H
#include "print_helper.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);

		class ColumnDesc
		{
		public:
			ColumnDesc();
			void set_cid(u32 column_id);
			u32 get_cid() const;
			const DataType& get_data_type()const;
			void set_data_type(const DataType& type);
			KV_STRING(
				K(column_id),
				K(data_type)
			);

		private:
			u32 column_id;
			DataType data_type;
		};

		class RowDesc
		{
		public:
			RowDesc();
			RowDesc(u32 column_count);
			u32 get_column_desc(u32 idx, ColumnDesc& c_desc) const;
			u32 set_column_desc(u32 idx, const ColumnDesc& c_desc);
			u32 get_column_num() const;
			u32 add_column_desc(const ColumnDesc& col_desc);
			KV_STRING(
				K(column_desc)
			);

		private:
			Vector<ColumnDesc> column_desc;
		};

		DECLARE(Row);
		class Row
		{
		protected:
			Row() = delete;
			Row(int column_count);
		public:
			virtual ~Row();
			static Row_s make_row(int column_count);
			static Row_s deep_copy(const Row_s &other);
			u32 get_cell_num() const;
			inline u32 get_cell(u32 idx, Object_s& cell) const { cell = cells[idx]; return 0;}
			u32 set_cell(u32 idx, Object_s& cell);
			u32 equal(const Row_s& other, const bool null_safe, bool &res) const;
			void set_op_id(u32 id) { op_id = id; }
			inline u32 get_op_id() const { return op_id; }
			KV_STRING(
				K(op_id),
				K(cells)
			);

		private:
			Vector<Object_s> cells;
			u32 op_id;
		private:
			DISALLOW_COPY_AND_ASSIGN(Row)
		};
	}
}
#endif	//ROW_H