#ifndef INSERT_GENERATOR_H
#define INSERT_GENERATOR_H
#include "dml_generator.h"

class InsertGenerator : public DMLGenerator {
public:
    InsertGenerator(Config &conf);
    ~InsertGenerator() {}
    void generate_query(string &query)override;
private:
    void to_string(string &query)override;
private:
    string insert_table;
    string select_value;
};

#endif //INSERT_GENERATOR_H