#include "table_space.h"
#include "io_service.h"
#include "cat_engine/cat_table_space.h"
#include "csv_engine/csv_table_space.h"
#include "global_context.h"
#include "object.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Storage;

TableSpace::TableSpace()
{

}

TableSpace::~TableSpace()
{
}

TableSpace_s TableSpace::make_table_space(const String& table_name, 
                                        const String& database, 
                                        const Vector<String> &args,
                                        double sample_value,
										bool read_only)
{
    if (!args.empty() && args[0] == "CSV") {
        return CSVTableSpace::make_table_space(table_name, 
											   database, 
											   args, 
											   sample_value,
											   read_only);
    } else {
        return CatTableSpace::make_table_space(table_name, 
											   database, 
											   args, 
											   sample_value, 
											   read_only);
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
		LOG_ERR("delete table error", K(table_name), K(ret));
		return ret;
	} else {
		return SUCCESS;
	}
}

u32 TableSpace::create_table(const String& database, const String& table_name)
{
	String path = table_path(database, table_name);
	u32 ret = IoService::create_file(path);
	if (ret != SUCCESS) {
		LOG_ERR("create table error", K(table_name), K(ret));
		return ret;
	} else {
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
	String path = SYS_CONF.data_dir() + "/" + database + "/" + table;
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
	String path = SYS_CONF.data_dir() + "/" + database;
#ifdef _WIN32
	for (u32 i = 0; i < path.size(); ++i) {
		if (path[i] == '/') {
			path[i] = '\\';
		}
	}
#endif
	return path;
}
