#ifndef STORAGE_H
#define STORAGE_H
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
	}
	namespace Storage {
		using Common::Row_s;

		class StorageEngine
		{
		public:
			virtual ~StorageEngine();
			virtual u32 open_table(const String& table) = 0;
			virtual u32 get_next_row(Row_s& row) = 0;
			virtual u32 reset() = 0;
			virtual u32 close() = 0;
			virtual u32 insert_row(const Row_s& row) = 0;
			virtual u32 update_row(const Row_s& row) = 0;
			virtual u32 delete_row(u32 row_id) = 0;
			virtual u32 get_table_id()const = 0;
		};
	}
}

#endif	//STORAGE_H
