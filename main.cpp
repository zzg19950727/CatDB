#include "server.h"
#include "cat_engine/page_dump.h"
#include <iostream>

int main(int argc, char* argv[])
{
	String config_path = "catdb.conf";
	if (argc == 2) {
		config_path = argv[1];
	} else if (argc == 3) {
		String file = argv[1];
		int index = std::atoi(argv[2]);
		CatDB::Storage::PageDump dump(file);
		String page_info;
		if (dump.dump_page(page_info, index)) {
			std::cout << page_info << std::endl;
		} else {
			std::cout << "no page" << std::endl;
		}
		return 0;
	}
	CatDB::Server::ServerService service(config_path);
	if (service.run() != 0) {
		std::cout << "start server failed!" << std::endl;
	}
	service.close_connection();
	return 0;
}
