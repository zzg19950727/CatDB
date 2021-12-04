#ifndef CAT_TABLE_SPACE_H
#define CAT_TABLE_SPACE_H
#include "table_space.h"
#include "type.h"

namespace CatDB {
	namespace Common{
		DECLARE(Row);
	}
	namespace Storage {
		DECLARE(Page);
		DECLARE(CatIoService);
		DECLARE(CatTableSpace);
		using Common::Row_s;
		/*简单存储引擎*/
		class CatTableSpace : public TableSpace
		{
		private:
			CatTableSpace();
		public:
			~CatTableSpace();
			static TableSpace_s make_table_space(const String& table_name, 
												 const String& database,
												 const Vector<String> &args, 
												 double sample_size = 1);
			//对外获取记录接口
			u32 open()override;
			u32 get_next_row(Row_s& row)override;
			u32 reset()override;
			u32 close()override;

			u32 insert_row(const Row_s& row)override;
			u32 update_row(const Row_s& row)override;
			u32 delete_row(u32 row_id)override;
			u32 delete_all_row()override;

		private:
			u32 get_page_from_row_id(u32 row_id, Page_s& page);
			u32 get_page_from_offset(u32 page_offset, Page_s& page);
			u32 read_page(u32 page_offset, Page_s& page);
			u32 create_page(u32 page_offset, Page_s& page);
			u32 get_last_page(Page_s& page);
			void reset_all_page();

		private:
			//保证page有序析构
			List<Page_s> pages_copy;
			HashMap<u32, Page_s>  pages;
			CatIoService_s io;
			u32 cur_page_offset;
			u32 page_skip_size;
		
		private:
			DISALLOW_COPY_AND_ASSIGN(CatTableSpace)
		};
	}
}

#endif	//CAT_TABLE_SPACE_H