#ifndef PAGE_DUMP_H
#define PAGE_DUMP_H
#include "type.h"

namespace CatDB {
    namespace Storage {
        DECLARE(Page);
		DECLARE(CatIoService);

        class PageDump {
            public:
                PageDump(const String& data_file);
                bool dump_page(String &page_info);
            private:
                CatIoService_s io_service;
                Page_s page;
        };
    }
}
#endif  //PAGE_DUMP_H