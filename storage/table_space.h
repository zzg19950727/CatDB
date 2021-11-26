﻿#ifndef TABLE_SPACE_H
#define TABLE_SPACE_H
#include "type.h"

namespace CatDB {
	namespace Common{
		DECLARE(Row);
	}
	namespace Storage {
		DECLARE(Page);
		DECLARE(TableSpace);
		DECLARE(IoService);
		using Common::Row_s;
		/*简单存储引擎*/
		class TableSpace
		{
		private:
			TableSpace();
		public:
			~TableSpace();
			static TableSpace_s make_table_space(const String& table_name, const String& database="", double sample_size = 1);
			//对外获取记录接口
			u32 open();
			u32 get_next_row(Row_s& row);
			u32 reset() override;
			u32 close() override;

			//
			u32 insert_row(const Row_s& row);
			u32 update_row(const Row_s& row);
			u32 delete_row(u32 row_id);
			u32 delete_all_row();
			void set_ref_table_id(u32 table_id) {ref_table_id = table_id;}

			static u64 table_space_size(const String& database, const String& table_name);
			static u32 delete_table(const String& database, const String& table_name);
			static u32 create_table(const String& database, const String& table_name);
			static u32 delete_database(const String& database);
			static u32 create_database(const String& database);

		private:
			u32 get_page_from_row_id(u32 row_id, Page_s& page);
			u32 get_page_from_offset(u32 page_offset, Page_s& page);
			u32 read_page(u32 page_offset, Page_s& page);
			u32 create_page(u32 page_offset, Page_s& page);
			u32 get_last_page(Page_s& page);
			void reset_all_page();
			static String table_path(const String & database, const String & table);
			static String database_path(const String & database);

		public:
			static String data_dir;
			static String recycle_dir;

		private:
			//保证page有序析构
			List<Page_s> pages_copy;
			HashMap<u32, Page_s>  pages;
			String database;
			String table_name;
			IoService_s io;
			u32 cur_page_offset;
			u32 page_skip_size;
			u32 ref_table_id;
		
		private:
			DISALLOW_COPY_AND_ASSIGN(TableSpace)
		};
	}
}

#endif	//TABLE_SPACE_H