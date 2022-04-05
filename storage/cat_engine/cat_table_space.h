#ifndef CAT_TABLE_SPACE_H
#define CAT_TABLE_SPACE_H
#include "table_space.h"
#include "page.h"
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Storage {
		DECLARE(Page);
		DECLARE(CatIoService);
		DECLARE(CatTableSpace);
		using Common::Row_s;

		class LRUManger {
		public:
			LRUManger();
			bool find_page(u32 offset, Page_s &page);
			bool add_page(Page_s &new_page, Page_s &old_page);
		private:
			struct Node {
				Node()
				:pre(NULL),
				next(NULL),
				count(0)
				{}

				Node *pre;
				Node *next;
				u32 count;
				Page_s page;
			};

			void visit_node(Node* node);
			void del_node(Node* node);
			void add_node_after(Node *head, Node *node);
			void add_node_before(Node *tail, Node *node);

			Node head;
			Node tail;
			static const u32 MAX_SIZE = 63;
		};

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
												 double sample_size,
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
			u32 update_row(u32 row_id, 
						   const Row_s& update_row, 
						   const Row_s& access_row)override;

		private:
			u32 get_page_from_row_id(u32 row_id, Page_s& page);
			u32 write_page_to_disk(const Page_s& page);

		private:
			LRUManger page_manager;
			CatIoService_s io;
			Page_s cur_page;
			bool read_only;
			bool is_empty_table_space;
		
		private:
			DISALLOW_COPY_AND_ASSIGN(CatTableSpace)
		};
	}
}

#endif	//CAT_TABLE_SPACE_H