#include "dml_generator.h"
#include "select_generator.h"

DMLGenerator::DMLGenerator(Config &conf)
    :expr_generator(conf),
    conf(conf)
{
}

void DMLGenerator::add_parent_params(vector<ParentParam> params)
{
    for (int i = 0; i < params.size(); ++i) {
        parent_params.push_back(params[i]);
    }
    expr_generator.add_parent_params(params);
}

void DMLGenerator::add_parent_param(ParentParam &param)
{
    parent_params.push_back(param);
    expr_generator.add_parent_param(param);
}

void DMLGenerator::generate_other_query(string &query)
{
    int type = std::rand() % 2;
    if (1 == type && conf.can_use_set) {
        SetGenerator generator(conf);
        generator.add_parent_params(parent_params);
        generator.generate_query(query);
    } else {
        SelectGenerator generator(conf);
        generator.add_parent_params(parent_params);
        generator.generate_query(query);
    }
}

void DMLGenerator::generate_from_list()
{
    if (tables.size() >= conf.max_table_count) {
        return;
    }
    int type = std::rand() % 2;
    if (!tables.empty()) {
        from_list += ", ";
    }
    from_list += generate_table_item();
    if (1 == type || tables.size() < conf.min_table_count) {
        generate_from_list();
    }
}

string DMLGenerator::generate_table_item(vector<string> *list)
{
    int type = std::rand() % 3;
    if (0 == type) {
        return generate_basic_table(list);
    } else if (1 == type) {
        if (conf.can_use_view && conf.query_count < conf.max_query_count) {
            return generate_view_table(list);
        } else {
            return generate_basic_table(list);
        }
    } else {
        if (conf.can_use_joined_table && tables.size() < conf.max_table_count) {
            return generate_joined_table(list);
        } else {
            return generate_basic_table(list);
        }
    }
}

string DMLGenerator::generate_basic_table(vector<string> *list)
{
    string table_name = random_list(conf.tables);
    string alias_name = generate_alias_table();
    tables.push_back(alias_name);
    expr_generator.add_table(alias_name);
    if (list) {
        list->push_back(alias_name);
    }
    return table_name + " " + alias_name;
}

string DMLGenerator::generate_view_table(vector<string> *list)
{
    string query;
    generate_other_query(query);
    string table_name = "(" + query + ")";
    string alias_name = generate_alias_table();
    tables.push_back(alias_name);
    expr_generator.add_table(alias_name);
    if (list) {
        list->push_back(alias_name);
    }
    return table_name + " " + alias_name;
}

string DMLGenerator::generate_joined_table(vector<string> *list)
{
    static vector<string> join_type = {" INNER JOIN ",
                                 " LEFT JOIN ",
                                 " RIGHT JOIN ",
                                 " FULL JOIN "};
    vector<string> joined_tables;
    string table = generate_table_item(&joined_tables) +
           random_list(join_type) + 
           generate_table_item(&joined_tables);
    string on_condition;
    expr_generator.generate_on_condition(on_condition, joined_tables);
    table += " ON " + on_condition;
    if (list) {
        for (int i = 0; i < joined_tables.size(); ++i) {
            list->push_back(joined_tables[i]);
        }
    }
    return table;
}

void DMLGenerator::generate_where()
{
    int type = std::rand() % 2;
    if (1 == type && conf.can_use_where) {
        string expr;
        expr_generator.generate_expr(expr);
        where = "WHERE " + expr;
    }
}

string DMLGenerator::generate_alias_table()
{
    return string("T") + std::to_string(conf.alias_table_id++);
}
