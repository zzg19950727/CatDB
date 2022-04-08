#include "server.h"
#include <iostream>
int main(int argc, char* argv[])
{
	String config_path = "catdb.conf";
	if (argc == 2) {
		config_path = argv[1];
	}
	CatDB::Server::ServerService service(config_path);
	if (service.run() != 0) {
		std::cout << "start server failed!" << std::endl;
	}
	service.close_connection();
	return 0;
}
