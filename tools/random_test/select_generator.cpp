#include "select_generator.h"

SelectGenerator::SelectGenerator(Config &conf)
    :DMLGenerator(conf),
    is_single_output(false)
{
}

void SelectGenerator::generate_query(string &query)
{
    ++conf.query_count;
    generate_from_list();
    generate_where();
    generate_group_by();
    generate_having();
    generate_distinct();
    generate_select_list();
    generate_order_by();
    generate_limit();
    to_string(query);
}

void SelectGenerator::generate_group_by()
{
    if (!conf.can_use_group_by) {
        return;
    }
    int type = std::rand() % 2;
    if (1 == type) {
        int count = (std::rand() % 5) + 1;
        group_by = "GROUP BY ";
        for (int i = 0; i < count; ++i) {
            string expr;
            expr_generator.generate_arith_expr(expr);
            group_by_exprs.push_back(expr);
            group_by += expr;
            if (i < count - 1) {
                group_by += ", ";
            }
        }
        expr_generator.add_group_by_exprs(group_by_exprs);
    }
}

void SelectGenerator::generate_having()
{
    if (!conf.can_use_having || group_by_exprs.empty()) {
        return;
    }
    int type = std::rand() % 2;
    if (1 == type) {
        string expr;
        expr_generator.generate_expr(expr);
        having = "HAVING " + expr;
    }
}

void SelectGenerator::generate_distinct()
{
    if (!conf.can_use_distinct) {
        return;
    }
    int type = std::rand() % 2;
    if (1 == type) {
        distinct = "DISTINCT";
    }
}

void SelectGenerator::generate_select_list()
{
    string expr;
    int type = std::rand() % 2;
    if (is_single_output) {
        expr_generator.generate_arith_expr(expr);
        select_list = expr + " AS ref0";
    } else if (group_by_exprs.empty() && 0 == type && conf.can_use_group_by) {
        //scalar
        for (int i = 0; i < conf.columns.size(); ++i) {
            expr_generator.generate_aggr_expr(expr);
            select_list = expr + " AS " + conf.columns[i];
            if (i < conf.columns.size() - 1) {
                select_list += ", ";
            }
        }
    } else {
        for (int i = 0; i < conf.columns.size(); ++i) {
            expr_generator.generate_arith_expr(expr);
            select_list = expr + " AS " + conf.columns[i];
            if (i < conf.columns.size() - 1) {
                select_list += ", ";
            }
        }
    }
}

void SelectGenerator::generate_order_by()
{
    if (!conf.can_use_order_by) {
        return ;
    }
    order_by = "ORDER BY ";
    for (int i = 1; i <= conf.columns.size(); ++i) {
        order_by += std::to_string(i);
        int type = std::rand() % 2;
        if (1 == type) {
            order_by += " ASC ";
        } else {
            order_by += " DESC ";
        }
        if (i < conf.columns.size()) {
            order_by += ",";
        }
    }
}

void SelectGenerator::generate_limit()
{
    if (!conf.can_use_limit) {
        return ;
    }
    int type = std::rand() % 3;
    if (1 == type) {
        limit = "LIMIT " + std::to_string(std::rand()%100);
    } else if (2 == type) {
        limit = "LIMIT " + std::to_string(std::rand()%100) + ", " + std::to_string(std::rand()%100);
    }
}

void SelectGenerator::to_string(string &query)
{
    query = "SELECT ";
    query += distinct + " ";
    query += select_list + " ";
    query += "FROM " + from_list + " ";
    query += where + " ";
    query += group_by + " ";
    query += having + " ";
    query += order_by + " ";
    query += limit;
}

void SetGenerator::generate_query(string &query)
{
    static vector<string> set_ops = {"UNION ALL",
                                     "UNION",
                                     "INTERSECT",
                                     "EXCEPT"};
    generate_other_query(left_query);
    generate_other_query(right_query);
    set_op = random_list(set_ops);
    query = "(" + left_query + ") " + set_op + " (" + right_query + ")";
}