#ifndef CAT_IO_SERVICE_H
#define CAT_IO_SERVICE_H
#include "io_service.h"
#include "type.h"

namespace CatDB {
	namespace Storage {
		DECLARE(Page);
		DECLARE(CatIoService);

		class CatIoService : public IoService
		{
		private:
			CatIoService();
		public:
			~CatIoService();
			static CatIoService_s make_cat_io_service();
			u32 read_page(Page_s& page);
			u32 read_next_page(Page_s& page);
			u32 read_first_page(Page_s& page);
			u32 read_last_page(Page_s& page);
			u32 write_page(const Page_s& page);
			u32 write_next_page(const Page_s& page);
		private:
			u32 end_offset(u64& offset) override;
			DISALLOW_COPY_AND_ASSIGN(CatIoService)
		};
	}
}

#endif	//CAT_IO_SERVICE_H
