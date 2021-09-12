#include "expr_generator.h"
#include "select_generator.h"

ExprGenerator::ExprGenerator(Config &conf)
    :conf(conf),
    is_on_condition(false),
    in_aggr(false)
{

}

void ExprGenerator::add_parent_params(vector<ParentParam> params)
{
    for (int i = 0; i < params.size(); ++i) {
        parent_params.push_back(params[i]);
    }
}

void ExprGenerator::add_parent_param(ParentParam &param)
{
    parent_params.push_back(param);
}

void ExprGenerator::add_table(string &table)
{
    tables.push_back(table);
}

void ExprGenerator::add_group_by_exprs(vector<string> &exprs)
{
    for (int i = 0; i < exprs.size(); ++i) {
        group_by_exprs.push_back(exprs[i]);
    }
}

void ExprGenerator::generate_expr(string &expr)
{
    length = 0;
    int type = std::rand() % 2;
    if (0 == type) {
        generate_cmp_expr(expr);
    } else {
        generate_logical_expr(expr);
    }
}

void ExprGenerator::generate_on_condition(string expr, const vector<string> &joined_tables)
{
    this->joined_tables = joined_tables;
    is_on_condition = true;
    generate_expr(expr);
    is_on_condition = false;
}

void ExprGenerator::ExprGenerator::generate_other_query(string &query)
{
    int type = std::rand() % 2;
    if (1 == type && conf.can_use_set) {
        ParentParam param;
        param.tables = tables;
        param.group_by_exprs = group_by_exprs;
        SetGenerator generator(conf);
        generator.add_parent_params(parent_params);
        generator.add_parent_param(param);
        generator.generate_query(query);
        generator.set_is_single();
    } else {
        ParentParam param;
        param.tables = tables;
        param.group_by_exprs = group_by_exprs;
        SelectGenerator generator(conf);
        generator.add_parent_params(parent_params);
        generator.add_parent_param(param);
        generator.generate_query(query);
        generator.set_is_single();
    }
    query = "(" + query + ")";
}
#include <iostream>
void ExprGenerator::generate_arith_expr(string &expr)
{
    //std::cout << "arith:" << length << " " << conf.max_expr_length << std::endl;
    static vector<string> op = {" + ", " - ",
                                " * ", " / "};
    string l_expr, r_expr;
    int type = std::rand() % 2;
    if (0 == type || length >= conf.max_expr_length) {
        generate_simple_expr(l_expr);
        length += l_expr.length();
    } else {
        //std::cout << "arith in:" << length << " " << conf.max_expr_length << std::endl;
        generate_arith_expr(l_expr);
    }
    type = std::rand() % 2;
    if (0 == type || length >= conf.max_expr_length) {
        generate_simple_expr(r_expr);
        length += r_expr.length();
    } else {
        //std::cout << "arith in:" << length << " " << conf.max_expr_length << std::endl;
        generate_arith_expr(r_expr);
    }
    expr = l_expr + random_list(op) + r_expr;
    type = std::rand() % 2;
    if (0 == type) {
        expr = "(" + expr + ")";
    }
}

void ExprGenerator::generate_simple_expr(string &expr)
{
    int type = std::rand() % 4;
    if (0 == type) {
        generate_column_expr(expr);
    } else if (1 == type) {
        generate_const_expr(expr);
    } else if (2 == type) {
        if (in_aggr || group_by_exprs.empty()) {
            generate_column_expr(expr);
        } else {
            generate_aggr_expr(expr);
        }
    } else {
        if (!conf.can_use_subquery || conf.query_count >= conf.max_query_count) {
            generate_column_expr(expr);
        } else {
            generate_query_ref_expr(expr);
        }
    }
}

void ExprGenerator::generate_column_expr(string &expr)
{
    int type = std::rand() % (parent_params.size()+1);
    if (type >= parent_params.size()) {
        if (!group_by_exprs.empty()) {
            expr = random_list(group_by_exprs);
        } else if (is_on_condition) {
            expr = random_list(joined_tables) + "." + random_list(conf.columns);
        } else {
            expr = random_list(tables) + "." + random_list(conf.columns);
        }
    } else {
        ParentParam &param = parent_params[type];
        if (!param.group_by_exprs.empty()) {
            expr = random_list(param.group_by_exprs);
        } else {
            expr = random_list(param.tables) + "." + random_list(conf.columns);
        }
    }
}

void ExprGenerator::generate_const_expr(string &expr)
{
    int type = std::rand() %3;
    if (0 == type) {
        expr = random_number();
    } else if (1 == type) {
        expr = random_string();
    } else {
        expr = random_time();
    }
}

void ExprGenerator::generate_aggr_expr(string &expr)
{
    static vector<string> func = {"COUNT", "SUM", "AVG", "MIN", "MAX"};
    static vector<string> distinct = {"", "DISTINCT "};
    string arith_expr;
    in_aggr = true;
    generate_arith_expr(arith_expr);
    in_aggr = false;
    expr = random_list(func) + "(" + random_list(distinct) + arith_expr + ")";
}

void ExprGenerator::generate_query_ref_expr(string &expr)
{
    generate_other_query(expr);
    length += expr.length();
}

void ExprGenerator::generate_logical_expr(string &expr)
{
    //std::cout << "log:" << length << " " << conf.max_expr_length << std::endl;
    static vector<string> op = {" AND ", " OR "};
    string l_expr, r_expr;
    int type = std::rand() % 2;
    if (0 == type || length >= conf.max_expr_length) {
        generate_cmp_expr(l_expr);
    } else {
        //std::cout << "log in:" << length << " " << conf.max_expr_length << std::endl;
        generate_logical_expr(l_expr);
    }
    type = std::rand() % 2;
    if (0 == type || length >= conf.max_expr_length) {
        generate_cmp_expr(r_expr);
    } else {
        //std::cout << "log in:" << length << " " << conf.max_expr_length << std::endl;
        generate_logical_expr(r_expr);
    }
    expr = l_expr + random_list(op) + r_expr;
    type = std::rand() % 2;
    if (0 == type) {
        expr = "(" + expr + ")";
    }
}

void ExprGenerator::generate_cmp_expr(string &expr)
{
    int type = std::rand() % 6;
    if (0 == type) {
        generate_gt_expr(expr);
    } else if (1 == type) {
        generate_is_expr(expr);
    } else if (2 == type) {
        generate_btw_expr(expr);
    } else if (3 == type) {
        if (!conf.can_use_subquery || conf.query_count >= conf.max_query_count) {
            generate_gt_expr(expr);
        } else {
            generate_in_expr(expr);
        }
    } else if (4 == type) {
        if (!conf.can_use_subquery || conf.query_count >= conf.max_query_count) {
            generate_gt_expr(expr);
        } else {
            generate_exists_expr(expr);
        }
    } else {
        generate_case_when_expr(expr);
    }
}

void ExprGenerator::generate_gt_expr(string &expr)
{
    static vector<string> op = {" > ", " >= ",
                                " < ", " <= ",
                                " = ", " != ",
                                " LIKE ", " NOT LIKE "};
    string l_expr, r_expr;
    generate_arith_expr(l_expr);
    generate_arith_expr(r_expr);
    expr = l_expr + random_list(op) + r_expr;
}

void ExprGenerator::generate_is_expr(string &expr)
{
    static vector<string> op = {" IS NULL", " IS NOT NULL"};
    string l_expr;
    generate_arith_expr(l_expr);
    expr = l_expr + random_list(op);
}

void ExprGenerator::generate_btw_expr(string &expr)
{
    static vector<string> op = {" BETWEEN ", " NOT BETWEEN "};
    string l_expr, r_expr1, r_expr2;
    generate_arith_expr(l_expr);
    generate_arith_expr(r_expr1);
    generate_arith_expr(r_expr2);
    expr = l_expr + random_list(op) + r_expr1 + " AND " + r_expr2;
}

void ExprGenerator::generate_in_expr(string &expr)
{
    static vector<string> op = {" IN ", " NOT IN "};
    string l_expr, r_expr;
    generate_arith_expr(l_expr);
    generate_query_ref_expr(r_expr);
    expr = l_expr + random_list(op) + r_expr;
}

void ExprGenerator::generate_exists_expr(string &expr)
{
    static vector<string> op = {"EXISTS ", "NOT EXISTS "};
    string r_expr;
    generate_query_ref_expr(r_expr);
    expr = random_list(op) + r_expr;
}

void ExprGenerator::generate_case_when_expr(string &expr)
{
    int type = std::rand() % 2;
    if (0 == type) {
        int n = (std::rand() % 10) + 1;
        string cond;
        generate_arith_expr(cond);
        expr = "CASE " + cond;
        for (int i = 0; i < n; ++i) {
            generate_arith_expr(cond);
            expr += " WHEN " + cond;
            generate_arith_expr(cond);
            expr += " THEN " + cond;
        }
        generate_arith_expr(cond);
        expr += " ELSE " + cond + " END";
    } else {
        int n = (std::rand() % 10) + 1;
        string cond;
        expr = "CASE";
        for (int i = 0; i < n; ++i) {
            generate_cmp_expr(cond);
            expr += " WHEN " + cond;
            generate_cmp_expr(cond);
            expr += " THEN " + cond;
        }
        generate_cmp_expr(cond);
        expr += " ELSE " + cond + " END";
    }
}
