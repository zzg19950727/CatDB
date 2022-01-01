#ifndef TABLE_SPACE_H
#define TABLE_SPACE_H
#include "type.h"

namespace CatDB {
	namespace Common{
		DECLARE(Row);
	}
	namespace Storage {
		DECLARE(TableSpace);
		using Common::Row_s;
		/*简单存储引擎*/
		class TableSpace
		{
		protected:
			TableSpace();
		public:
			virtual ~TableSpace();
			static TableSpace_s make_table_space(const String& table_name, 
                                                 const String& database,
                                                 const Vector<String> &args,
                                                 double sample_size = 1,
												 bool read_only = false);
			//对外获取记录接口
			virtual u32 open() = 0;
			virtual u32 get_next_row(Row_s& row) = 0;
			virtual u32 reset() = 0;
			virtual u32 close() = 0;

			virtual u32 insert_row(const Row_s& row) = 0;
			virtual u32 update_row(const Row_s& row) = 0;
			virtual u32 delete_row(u32 row_id) = 0;
			virtual u32 delete_all_row() = 0;

			static u64 table_space_size(const String& database, const String& table_name);
			static u32 delete_table(const String& database, const String& table_name);
			static u32 create_table(const String& database, const String& table_name);
			static u32 delete_database(const String& database);
			static u32 create_database(const String& database);

		protected:
			static String table_path(const String & database, const String & table);
			static String database_path(const String & database);

		public:
			static String data_dir;
			static String recycle_dir;

		protected:
			String database;
			String table_name;
		
		private:
			DISALLOW_COPY_AND_ASSIGN(TableSpace)
		};
	}
}

#endif	//TABLE_SPACE_H