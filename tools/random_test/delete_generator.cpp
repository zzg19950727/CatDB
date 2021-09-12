#include "delete_generator.h"

DeleteGenerator::DeleteGenerator(Config &conf)
    :DMLGenerator(conf)
{
}

void DeleteGenerator::generate_query(string &query)
{
    ++conf.query_count;
    delete_table = generate_basic_table();
    generate_from_list();
    generate_where();
    to_string(query);
}

void DeleteGenerator::to_string(string &query)
{
    query = "DELETE ";
    query += delete_table + " ";
    query += "FROM " + from_list + " ";
    query += where;
}
