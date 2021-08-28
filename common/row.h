﻿#ifndef ROW_H
#define ROW_H
#include "type.h"
#include "object.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);

		class ColumnDesc
		{
		public:
			ColumnDesc();
			void set_tid(u32 table_id);
			u32 get_tid() const {return table_id;}
			void set_cid(u32 column_id);
			u32 get_cid() const {return column_id;}
			void set_tid_cid(u32 table_id, u32 column_id);
			void get_tid_cid(u32& table_id, u32& column_id)const;
			int operator==(const ColumnDesc& other)const;
			u32 get_data_type()const;
			void set_data_type(u32 type);
			KV_STRING(
				K(table_id),
				K(column_id),
				K(data_type)
			);

		private:
			u32 table_id;
			u32 column_id;
			u32 data_type;
		};

		class RowDesc
		{
		public:
			RowDesc();
			RowDesc(u32 column_count);
			u32 get_column_idx(const ColumnDesc& c_desc, u32 &idx) const;
			u32 get_column_desc(u32 idx, ColumnDesc& c_desc) const;
			u32 set_column_desc(u32 idx, const ColumnDesc& c_desc);
			u32 get_column_num() const;
			u32 add_column_desc(const ColumnDesc& col_desc);
			KV_STRING(
				K(column_count),
				K(column_desc)
			);

		private:
			Vector<ColumnDesc> column_desc;
			u32 column_count;
		};

		class Row
		{
		protected:
			Row() = delete;
			Row(const RowDesc &row_desc);
		public:
			~Row();
			static Row_s make_row(const RowDesc &row_desc);
			static Row_s deep_copy(const Row_s &other);
			//深拷贝
			void assign(const Row &other);
			void set_row_id(u32 row_id);
			u32 get_row_id()const;
			RowDesc& get_row_desc();
			void set_row_desc(const RowDesc &row_desc);
			virtual u32 get_cell(const ColumnDesc& c_desc, Object_s& cell) const;
			u32 get_cell(u32 idx, Object_s& cell) const;
			u32 set_cell(const ColumnDesc& c_desc, Object_s cell);
			u32 set_cell(u32 idx, Object_s cell);
			bool equal(const Row_s& other) const;
			KV_STRING(
				K(row_id),
				K(row_desc),
				K(cells)
			);

		private:
			Vector<Object_s> cells;
			RowDesc row_desc;
			u32 row_id;

		private:
			DISALLOW_COPY_AND_ASSIGN(Row)
		};

		class RowAgent : public Row
		{
		private:
			RowAgent();
		public:
			static Row_s make_agent_row(const Row_s& row1, const Row_s& row2);
			u32 add_row(const Row_s& row);
			u32 get_cell(const ColumnDesc& c_desc, Object_s& cell) const;
		private:
			Vector<Row_s> agent_row;
		};
	}
}
#endif	//ROW_H