#ifndef TABLE_SPACE_H
#define TABLE_SPACE_H
#include "type.h"

namespace CatDB {
	namespace Common{
		DECLARE(Row);
	}
	namespace Storage {
		DECLARE(Page);
		DECLARE(IoService);
		DECLARE(TableSpace);
		using Common::Row_s;
		/*简单存储引擎*/
		class TableSpace
		{
		private:
			TableSpace(const String& table_id);
		public:
			~TableSpace();
			static TableSpace_s make_table_space(const String& table_id);
			//对外获取记录接口
			u32 open();
			u32 get_next_row(Row_s& row);
			u32 reset();
			u32 close();
			//
			u32 insert_row(const Row_s& row);
			u32 update_row(const Row_s& row);
			u32 delete_row(u32 row_id);
			u32 delete_all_row();
			u32 delete_table();
			u32 create_table(const String& table_id);
			u32 get_table_id()const;
		private:
			u32 get_page_from_row_id(u32 row_id, Page_s& page);
			u32 get_page_from_offset(u32 page_offset, Page_s& page);
			u32 read_page(u32 page_offset, Page_s& page);
			u32 create_page(u32 page_offset, Page_s& page);
			u32 get_last_page(Page_s& page);
		private:
			//保证page有序析构
			List<Page_s> pages_copy;
			HashMap<u32, Page_s>  pages;
			String table_id;
			IoService_s io;
			u32 cur_page_offset;

		private:
			DISALLOW_COPY_AND_ASSIGN(TableSpace)
		};
	}
}

#endif	//TABLE_SPACE_H