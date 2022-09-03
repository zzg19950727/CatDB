#ifndef TABLE_SPACE_H
#define TABLE_SPACE_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Storage {
		DECLARE(TableSpace);
		using Common::Row_s;
		using Common::RowDesc;
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
                                                 double sample_value = -1,
												 bool read_only = false);
			//对外获取记录接口
			virtual u32 open() = 0;
			virtual u32 get_next_row(Row_s& row) = 0;
			virtual u32 reset() = 0;
			virtual u32 close() = 0;

			virtual u32 insert_row(const Row_s& row) = 0;
			virtual u32 get_row(u32 row_id, Row_s& row) = 0;
			virtual u32 delete_row(u32 row_id) = 0;
			virtual u32 delete_all_row() = 0;
			virtual u32 update_row(u32 row_id, const Row_s& update_row, const Row_s& access_row) = 0;

			void set_access_desc(const RowDesc& desc) { access_desc = desc; }
			void set_update_desc(const RowDesc& desc) { update_desc = desc; }

			static u64 table_space_size(const String& database, const String& table_name);
			static u32 delete_table(const String& database, const String& table_name);
			static u32 create_table(const String& database, const String& table_name);
			static u32 delete_database(const String& database);
			static u32 create_database(const String& database);

		protected:
			static String table_path(const String & database, const String & table);
			static String database_path(const String & database);

		protected:
			String database;
			String table_name;
			RowDesc access_desc;
			RowDesc update_desc;
			u32 sample_value;
		
		private:
			DISALLOW_COPY_AND_ASSIGN(TableSpace)
		};
	}
}

#endif	//TABLE_SPACE_H