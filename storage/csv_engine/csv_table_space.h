#ifndef CSV_TABLE_SPACE_H
#define CSV_TABLE_SPACE_H
#include "table_space.h"
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Storage {
		DECLARE(IoService);
		DECLARE(CSVTableSpace);
		using Common::Row_s;
		/*简单存储引擎*/
		class CSVTableSpace : public TableSpace
		{
		private:
			CSVTableSpace(const Vector<String> &args, bool read_only);
		public:
			~CSVTableSpace();
			static TableSpace_s make_table_space(const String& table_name, 
												 const String& database,
												 const Vector<String> &args, 
												 double sample_value,
												 bool read_only);
			//对外获取记录接口
			u32 open()override;
			u32 get_next_row(Row_s& row)override;
			u32 reset()override;
			u32 close()override;

			u32 insert_row(const Row_s& row)override;
			u32 get_row(u32 row_id, Row_s& row)override;
			u32 delete_row(u32 row_id)override;
			u32 delete_all_row()override;
			u32 update_row(u32 row_id, const Row_s& update_row, const Row_s& access_row)override;

		private:
			u32 buffer_empty();
			u32 buffer_read();
			u32 buffer_move();
			u32 read_row(Row_s& row);
			u32 read_row_reuse_mem(Row_s& row);
			u32 write_row(const Row_s& row);
			u32 buffer_full();

		private:
			const u32 buffer_size = 64 * PAGE_SIZE;
			IoService_s io;
			u64 row_id;
			bool need_write_buffer;

			String path;
			u32 field_count;
            char field_split;
			char line_split;

			u32 *column_pos;
			char *buffer;
			u32 buf_start;
			u32 buf_end;
			u32 buf_read_pos;
			u32 column_cnt;
		private:
			DISALLOW_COPY_AND_ASSIGN(CSVTableSpace)
		};
	}
}

#endif	//CSV_TABLE_SPACE_H