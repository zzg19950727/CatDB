#ifndef EXPR_GENERATOR_H
#define EXPR_GENERATOR_H
#include "random.h"

struct Config {
    vector<string> tables;
    vector<string> columns;
    int alias_table_id = 1;
    int max_table_count = 1;
	int min_table_count = 1;
    int max_query_count = 1;
    int max_expr_length = 5;
    int query_count = 0;
	bool use_simple_expr = false;
    bool can_use_set = false;
    bool can_use_distinct = true;
    bool can_use_view = false;
    bool can_use_joined_table = false;
    bool can_use_group_by = false;
    bool can_use_having = false;
    bool can_use_order_by = false;
    bool can_use_limit = false;
    bool can_use_subquery = false;
    bool can_use_where = false;
	bool use_scalar_group_by = false;
	bool is_single_output = false;
};

struct ParentParam {
    vector<string> tables;
    vector<string> group_by_exprs;
};

class ExprGenerator {
public:
    ExprGenerator(Config &conf);
    ~ExprGenerator() {}
    void add_parent_params(vector<ParentParam> params);
    void add_parent_param(ParentParam &param);
    void add_table(string &table);
    void add_group_by_exprs(vector<string> &expr);
    void generate_expr(string &expr);
    void generate_arith_expr(string &expr);
    void generate_aggr_expr(string &expr);
    void generate_on_condition(string expr, const vector<string> &joined_tables);
    void set_can_use_aggr(bool use) { can_use_aggr = use; }
private:
    void generate_other_query(string &query);
    void generate_simple_expr(string &expr);
    void generate_column_expr(string &expr);
    void generate_const_expr(string &expr);
    void generate_query_ref_expr(string &expr);
    void generate_logical_expr(string &expr);
    void generate_cmp_expr(string &expr);
    void generate_gt_expr(string &expr);
    void generate_is_expr(string &expr);
    void generate_btw_expr(string &expr);
    void generate_in_expr(string &expr);
    void generate_exists_expr(string &expr);
    void generate_case_when_expr(string &expr);
private:
    Config &conf;
    vector<ParentParam> parent_params;
    vector<string> tables;
    vector<string> joined_tables;
    vector<string> group_by_exprs;
    double length;
    bool is_on_condition;
    bool in_aggr;
    bool can_use_aggr;
};

#endif //EXPR_GENERATOR_H
