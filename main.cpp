#include "server.h"
#include "tpch.h"
#include <iostream>
int main(int argc, char* argv[])
{
	String config_path = "catdb.conf";
	if (argc == 2) {
		config_path = argv[1];
	}
	CatDB::Server::ServerService service(config_path);
	service.run();
	service.close_connection();
	exit(0);
	return 0;
}
