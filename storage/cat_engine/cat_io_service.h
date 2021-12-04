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
			u32 write_page(const Page_s& page);
			u32 write_page(const Page* page);
			u32 end_offset(u64& offset) override;
		private:
			DISALLOW_COPY_AND_ASSIGN(CatIoService)
		};
	}
}

#endif	//CAT_IO_SERVICE_H
