#include "update_generator.h"

UpdateGenerator::UpdateGenerator(Config &conf)
    :DMLGenerator(conf)
{
}

void UpdateGenerator::generate_query(string &query)
{
    ++conf.query_count;
    generate_from_list();
    generate_where();
    generate_set_value();
    to_string(query);
    query_select_table_data = "SELECT * FROM " + update_table;
}

void UpdateGenerator::generate_from_list()
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
		update_table = tables[0];
	}
    if (1 == type || tables.size() < conf.min_table_count) {
        generate_from_list();
    }
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
