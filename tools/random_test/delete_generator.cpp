#include "delete_generator.h"

DeleteGenerator::DeleteGenerator(Config &conf)
    :DMLGenerator(conf)
{
}

void DeleteGenerator::generate_query(string &query)
{
    ++conf.query_count;
    generate_from_list();
    generate_where();
    to_string(query);
    query_select_table_data = "SELECT * FROM " + delete_table;
}

void DeleteGenerator::generate_from_list()
{
	if (!tables.empty() && tables.size() >= conf.max_table_count) {
        return;
    }
    int type = std::rand() % 2;    
	if (!tables.empty()) {
        from_list += ", ";
		from_list += generate_table_item();
    } else {
		from_list += generate_basic_table();
		delete_table = tables[0];
	}
    if (1 == type || tables.size() < conf.min_table_count) {
        generate_from_list();
    }
}

void DeleteGenerator::to_string(string &query)
{
    query = "DELETE ";
    query += delete_table + " ";
    query += "FROM " + from_list + " ";
    query += where;
}
