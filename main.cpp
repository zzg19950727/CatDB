#include "server.h"
#include "tpch.h"

int main(int argc, char* argv[])
{
	CatDB::Server::ServerService service("catdb.conf");
	service.run();
	service.close_connection();
    exit(0);
	return 0;
}
