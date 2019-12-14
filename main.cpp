#include "server.h"
#include "tpch.h"

int main()
{
	CatDB::Server::ServerService service("catdb.conf");
	//load_tpch_data();
	service.run();
	return 0;
}