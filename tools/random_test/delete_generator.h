#ifndef DELETE_GENERATOR_H
#define DELETE_GENERATOR_H
#include "dml_generator.h"

class DeleteGenerator : public DMLGenerator {
public:
    DeleteGenerator(Config &conf);
    ~DeleteGenerator() {}
    void generate_query(string &query);
private:
    void to_string(string &query);
private:
    string delete_table;
};

#endif //DELETE_GENERATOR_H