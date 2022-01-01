#ifndef IO_SERVICE_H
#define IO_SERVICE_H
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(IoService);

		class IoService
		{
		protected:
			IoService();
		public:
			virtual ~IoService();
			static IoService_s make_io_service();
			u32 open(const String& table_file);
			u32 read_buffer(char* buffer, u64 offset, u64 &size);
			u32 write_buffer(const char* buffer, u64 offset, u64 &size);
			u32 read_buffer(char* buffer, u64 &size);
			u32 write_buffer(const char* buffer, u64 &size);
			virtual u32 end_offset(u64& offset);
			void close();
			u32 move_to_file_end();
			static u32 create_file(const String& table_file);
			static u32 delete_file(const String& table_file);
			static u32 clear_file(const String& table_file);
			static u64 get_file_size(const String& table_file);
			static u32 make_dir(const String& dir);
			static u32 remove_dir(const String& dir);
			static u32 move_dir(const String& src_dir, const String& dst_dir);

		private:
			FILE* file_handle;
		private:
			DISALLOW_COPY_AND_ASSIGN(IoService)
		};
	}
}

#endif	//IO_SERVICE_H
