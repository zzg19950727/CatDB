#include "server.h"
#include "tpch.h"

int main(int argc, char* argv[])
{
	CatDB::Server::ServerService service("catdb.conf");
	if (argc > 2) {
		if (argv[1] == "init") {
			init_db();
		}
	}
	service.run();
	return 0;
}
