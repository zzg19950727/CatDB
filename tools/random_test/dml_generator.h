#ifndef DML_GENERATOR_H
#define DML_GENERATOR_H
#include "expr_generator.h"

class DMLGenerator {
public:
    DMLGenerator(Config &conf);
    ~DMLGenerator() {}
    void add_parent_params(vector<ParentParam> params);
    void add_parent_param(ParentParam &param);
    virtual void generate_query(string &query) = 0;
protected:
    void generate_other_query(string &query);
    void generate_from_list();
    string generate_table_item(vector<string> *list = NULL);
    string generate_basic_table(vector<string> *list = NULL);
    string generate_view_table(vector<string> *list = NULL);
    string generate_joined_table(vector<string> *list = NULL);
    void generate_where();
    virtual void to_string(string &query) = 0;
    string generate_alias_table();
protected:
    ExprGenerator expr_generator;
    Config &conf;
    vector<ParentParam> parent_params;
    vector<string> tables;
    string from_list;
    string where;
};

#endif //DML_GENERATOR_H