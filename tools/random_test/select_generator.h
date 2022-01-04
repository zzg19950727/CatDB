#ifndef SELECT_GENERATOR_H
#define SELECT_GENERATOR_H
#include "dml_generator.h"

class SelectGenerator : public DMLGenerator {
public:
    SelectGenerator(Config &conf);
    ~SelectGenerator() {}
    void generate_query(string &query)override;
    void set_is_single() { is_single_output = true; }
private:
    void generate_group_by();
    void generate_having();
    void generate_distinct();
    void generate_select_list();
    void generate_order_by();
    void generate_limit();
    void to_string(string &query)override;
protected:
    vector<string> group_by_exprs;
    string distinct;
    string select_list;
    string group_by;
    string having;
    string order_by;
    string limit;
    bool is_single_output;
};

class SetGenerator : public SelectGenerator {
public:
    SetGenerator(Config &conf)
        :SelectGenerator(conf){}
    ~SetGenerator() {}
    void generate_query(string &query);
private:
    string left_query;
    string right_query;
    string set_op;
};

#endif //SELECT_GENERATOR_H