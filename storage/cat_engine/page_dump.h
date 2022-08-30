#ifndef PAGE_DUMP_H
#define PAGE_DUMP_H
#include "type.h"
#include "cat_io_service.h"
#include "object.h"
#include "page.h"

namespace CatDB {
    namespace Storage {
        DECLARE(Page);
		DECLARE(CatIoService);

        class PageDump {
            public:
                PageDump(const String& data_file);
                bool dump_page(String &page_info, u32 page_id);
            private:
                void dump_header(Page_s &page, std::stringstream &str_stream);
                void hex_string(const char* data, u32 size, String& str);
                void dump_record(Page_s &page, int idx, std::stringstream &str_stream);

                CatIoService_s io_service;
                Page_s page;
                u32 index;
        };
    }
}
#endif  //PAGE_DUMP_H