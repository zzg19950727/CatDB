#include "insert_generator.h"

InsertGenerator::InsertGenerator(Config &conf)
    :DMLGenerator(conf)
{
}

void InsertGenerator::generate_query(string &query)
{
    ++conf.query_count;
    insert_table = generate_basic_table();
    generate_other_query(select_value);
    to_string(query);
    query_select_table_data = "SELECT * FROM " + insert_table;
}

void InsertGenerator::to_string(string &query)
{
    query = "INSERT INTO ";
    query += insert_table + " ";
    query += select_value;
}
