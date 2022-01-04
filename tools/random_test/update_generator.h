#ifndef UPDATE_GENERATOR_H
#define UPDATE_GENERATOR_H
#include "dml_generator.h"

class UpdateGenerator : public DMLGenerator {
public:
    UpdateGenerator(Config &conf);
    ~UpdateGenerator() {}
    void generate_query(string &query)override;
private:
    void generate_from_list()override;
    void generate_set_value();
    void to_string(string &query)override;
private:
    string update_table;
    string set_value;
};

#endif //UPDATE_GENERATOR_H