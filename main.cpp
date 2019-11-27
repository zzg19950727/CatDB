#include "tpch.h"
#include "server.h"

int main()
{
	CatDB::Server::ServerService service("catdb.conf1");
	service.run();
	return 0;
}