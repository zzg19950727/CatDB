#ifndef TABLE_SPACE_H
#define TABLE_SPACE_H
#include "type.h"

namespace CatDB {
	namespace Common{
		DECLARE(Row);
	}
	namespace Storage {
		DECLARE(Page);
		DECLARE(TableSpace);
		DECLARE(PageManager);
		using Common::Row_s;
		/*简单存储引擎*/
		class TableSpace
		{
		private:
			TableSpace();
		public:
			~TableSpace();
			static TableSpace_s make_table_space(const String& table_name, const String& database="");
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
			static u32 delete_table(const String& database, const String& table_name);
			static u32 create_table(const String& database, const String& table_name);

		private:
			PageManager_s page_manager;
			u32 cur_page_offset;

		private:
			DISALLOW_COPY_AND_ASSIGN(TableSpace)
		};
	}
}

#endif	//TABLE_SPACE_H