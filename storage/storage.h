#ifndef STORAGE_H
#define STORAGE_H
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Buffer);
	}
	namespace Storage {
		DECLARE(IoService);
		DECLARE(CatEngine);
		using Common::Buffer_s;

		class CatEngine
		{
		public:
			CatEngine();
			~CatEngine();

			static CatEngine_s make_cat_engine();
			void set_data_dir(const String& dir);
			String get_data_dir()const;
			void set_max_blocks(u32 count);
			u32 get_max_blocks() const;
			u32 end_page_offset(const String& database, const String& table, u32& offset);
			u32 delete_all_row(const String& database, const String& table);
			u64 get_table_size(const String& database, const String& table);
			u32 delete_table(const String& database, const String& table);
			u32 create_table(const String& database, const String& table);
			u32 delete_database(const String& database);
			u32 create_database(const String& database);
			u32 create_new_page_buffer(const String& database, const String& table, u32 page_offset, Buffer_s& buffer);
			u32 get_page_buffer(const String& database, const String& table, u32 page_offset, Buffer_s& buffer);
			u32 free_page_buffer(const String& database, const String& table, u32 page_offset);
			u32 free_page_memory(u32 count);

		private:
			void init_cache_manager();
			void timer_for_free_cache();
			void timer_for_preread_cache();
			String get_table_path(const String& database, const String& table)const;
			u32 read_page_buffer(const String& table_path, u32 page_offset, Buffer_s& buffer);
			u32 open_table(const String& table_path, IoService_s& file_handle);
			u32 cache_size()const;
			struct Node {
				u32 ref;
				Buffer_s buf;
				String key;
				u32 offset;
				Node():ref(0){}
			};
			typedef List<Node>::iterator NodeIter;
			//空闲缓存，HashMap+List实现o(1)的LRU算法
			//内存不足时会从这释放
			List<Node> free_node_list;
			HashMap<String, HashMap<u32, NodeIter>> free_node_map;
			//正被系统引用的缓存
			HashMap<String, HashMap<u32, Node>> using_node_map;
			HashMap<String, IoService_s> io_services;
			String data_dir;
			u32 max_buffer_blocks;
		};
	}
}

#endif	//STORAGE_H
