#include "update_generator.h"

UpdateGenerator::UpdateGenerator(Config &conf)
    :DMLGenerator(conf)
{
}

void UpdateGenerator::generate_query(string &query)
{
    ++conf.query_count;
    update_table = generate_basic_table();
    generate_from_list();
    generate_where();
    generate_set_value();
    to_string(query);
}

void UpdateGenerator::generate_set_value()
{
    int n = (std::rand() % conf.columns.size()) + 1;
    for (int i = 0; i < n; ++i) {
        string value;
        expr_generator.generate_arith_expr(value);
        set_value += conf.columns[i] + " = " + value;
        if (i < n - 1) {
            set_value += ", ";
        }
    }
}

void UpdateGenerator::to_string(string &query)
{
    query = "UPDATE ";
    query += update_table + " ";
    query += "SET " + set_value + " ";
    query += "FROM " + from_list + " ";
    query += where;
}
