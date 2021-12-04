#ifndef STORAGE_H
#define STORAGE_H
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(Page);
		DECLARE(CatIoService);
		DECLARE(PageManager);

		class PageManager
		{
		public:
			PageManager(const String& database, const String& table_name);
			~PageManager();

			u32 get_page_from_row_id(u32 row_id, Page_s& page);
			u32 get_page_from_offset(u32 page_offset, Page_s& page);
			u32 read_page(u32 page_offset, Page_s& page);
			u32 create_page(u32 page_offset, Page_s& page);
			u32 get_last_page(Page_s& page);
			u32 delete_all_row();
			u32 clear();
			void set_data_dir(const String& dir);
			void reset_all_page();
		private:
			//��֤page��������
			List<Page_s> pages_copy;
			HashMap<u32, Page_s>  pages;
			String data_dir;
			String database;
			String table_name;
			CatIoService_s io;
		};

		class CacheManager
		{
		public:
			CacheManager();
			~CacheManager();
			void set_data_dir(const String& dir);
			String get_data_dir()const;


		private:
			HashMap<String, PageManager_s> tables;
			String data_dir;
		};
	}
}

#endif	//STORAGE_H
