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
    if (!conf.can_use_group_by || conf.use_scalar_group_by) {
        return;
    }
    int type = std::rand() % 2;
    if (1 == type) {
        int count = (std::rand() % 5) + 1;
        group_by = "GROUP BY ";
        vector<string> exprs;
        for (int i = 0; i < count; ++i) {
            string expr;
            expr_generator.generate_arith_expr(expr);
            exprs.push_back(expr);
            group_by += expr;
            if (i < count - 1) {
                group_by += ", ";
            }
        }
        group_by_exprs = exprs;
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
        expr_generator.set_can_use_aggr(true);
        expr_generator.generate_expr(expr);
        expr_generator.set_can_use_aggr(false);
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
    select_list = "";
    int type = std::rand() % 2;
	int count = 1;
	if (!is_single_output) {
		count = conf.columns.size();
	}
    if (conf.use_scalar_group_by) {
        //scalar
        for (int i = 0; i < count; ++i) {
	        string expr;
            expr_generator.generate_aggr_expr(expr);
            select_list += expr + " AS " + conf.columns[i];
            if (i < count - 1) {
                select_list += ", ";
            }
        }
    } else {
        expr_generator.set_can_use_aggr(true);
        for (int i = 0; i < count; ++i) {
	        string expr;
            expr_generator.generate_arith_expr(expr);
            select_list += expr + " AS " + conf.columns[i];
            if (i < count - 1) {
                select_list += ", ";
            }
        }
        expr_generator.set_can_use_aggr(false);
    }
}

void SelectGenerator::generate_order_by()
{
    if (!conf.can_use_order_by) {
        return ;
    }
    order_by = "ORDER BY ";
	int count = 1;
	if (!is_single_output) {
		count = conf.columns.size();
	}
    for (int i = 1; i <= count; ++i) {
        order_by += std::to_string(i);
        int type = std::rand() % 2;
        if (1 == type) {
            order_by += " ASC ";
        } else {
            order_by += " DESC ";
        }
        if (i < count) {
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
        limit = "LIMIT " + std::to_string(std::rand()%10);
    } else if (2 == type) {
        limit = "LIMIT " + std::to_string(std::rand()%10) + ", " + std::to_string(std::rand()%10);
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
