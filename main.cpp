#include "server.h"

int main()
{
	CatDB::Server::ServerService service("catdb.conf");
	service.run();
	return 0;
}