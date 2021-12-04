#include "table_space.h"
#include "io_service.h"
#include "cat_engine/cat_table_space.h"
#include "csv_engine/csv_table_space.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Storage;

String TableSpace::data_dir = ".";
String TableSpace::recycle_dir = ".";

TableSpace::TableSpace()
{

}

TableSpace::~TableSpace()
{
}

TableSpace_s TableSpace::make_table_space(const String& table_name, 
                                        const String& database, 
                                        const Vector<String> &args,
                                        double sample_size)
{
    if (!args.empty() && args[0] == "CSV") {
        return CSVTableSpace::make_table_space(table_name, database, args, sample_size);
    } else {
        return CatTableSpace::make_table_space(table_name, database, args, sample_size);
    }
}

u64 TableSpace::table_space_size(const String & database, const String & table_name)
{
	String path = table_path(database, table_name);
	return IoService::get_file_size(path);
}

u32 TableSpace::delete_table(const String& database, const String& table_name)
{
	IoService_s io = IoService::make_io_service();
	String path = table_path(database, table_name);
	u32 ret = IoService::delete_file(path);
	if (ret != SUCCESS) {
		LOG_ERR("delete table error", K(table_name), err_string(ret));
		return ret;
	} else {
		return SUCCESS;
	}
}

u32 TableSpace::create_table(const String& database, const String& table_name)
{
	IoService_s io = IoService::make_io_service();
	String path = table_path(database, table_name);
	u32 ret = io->open(path);
	if (ret != SUCCESS) {
		LOG_ERR("create table error", K(table_name), err_string(ret));
		return ret;
	} else {
		io->close();
		return SUCCESS;
	}
}

u32 TableSpace::delete_database(const String & database)
{
	String path = database_path(database);
	IoService::remove_dir(path);
	return SUCCESS;
}

u32 TableSpace::create_database(const String & database)
{
	String path = database_path(database);
	IoService::make_dir(path);
	return SUCCESS;
}

String TableSpace::table_path(const String & database, const String & table)
{
	String path = data_dir + "/" + database + "/" + table;
#ifdef _WIN32
	for (u32 i = 0; i < path.size(); ++i) {
		if (path[i] == '/') {
			path[i] = '\\';
		}
	}
#endif
	return path;
}

String TableSpace::database_path(const String & database)
{
	String path = data_dir + "/" + database;
#ifdef _WIN32
	for (u32 i = 0; i < path.size(); ++i) {
		if (path[i] == '/') {
			path[i] = '\\';
		}
	}
#endif
	return path;
}
