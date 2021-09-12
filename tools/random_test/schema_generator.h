#ifndef SCHEMA_GENERATOR_H
#define SCHEMA_GENERATOR_H

#include "expr_generator.h"
#include "random.h"
#include <fstream>

class SchemaGenerator {
public:
    SchemaGenerator(Config &conf, 
                    int table_count, 
                    int column_count, 
                    int max_row_count)
        :conf(conf), 
        table_count(table_count), 
        column_count(column_count), 
        max_row_count(max_row_count) {}
    ~SchemaGenerator() {}
    void generate_schema(std::ofstream &out);
private:
    void generate_table(string &table_name, vector<string> &column_tpye, string &table_define);
    void generate_table_data(string &table_name, vector<string> &column_tpye, string &table_data);
private:
    Config &conf;
    int table_count;
    int column_count;
    int max_row_count;
};

#endif  //SCHEMA_GENERATOR_H