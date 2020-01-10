#include "server.h"
#include "tpch.h"

int main(int argc, char* argv[])
{
	CatDB::Server::ServerService service("catdb.conf");
	//init_db();
	//load_tpch_data();
	service.run();
	return 0;
}