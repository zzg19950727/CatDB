#ifndef DELETE_GENERATOR_H
#define DELETE_GENERATOR_H
#include "dml_generator.h"

class DeleteGenerator : public DMLGenerator {
public:
    DeleteGenerator(Config &conf);
    ~DeleteGenerator() {}
    void generate_query(string &query)override;
private:
	void generate_from_list()override;
    void to_string(string &query)override;
private:
    string delete_table;
};

#endif //DELETE_GENERATOR_H
