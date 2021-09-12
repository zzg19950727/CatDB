#include "select_generator.h"
#include "update_generator.h"
#include "delete_generator.h"
#include "insert_generator.h"
#include "schema_generator.h"
#include <iostream>
#include <ctime>

void print_usage()
{
    std::cout << "SqlGenerator type table_count column_count row_count query_count case_file" << std::endl;
    std::cout << "  type: select, insert, update, delete" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 7) {
        print_usage();
        return 0;
    }
    string type = argv[1];
    int table_count = std::atoi(argv[2]);
    int column_count = std::atoi(argv[3]);
    int row_count = std::atoi(argv[4]);
    int query_count = std::atoi(argv[5]);
    std::srand(std::time(nullptr));
    Config conf;
    std::ofstream out;
    out.open(argv[6]);
    SchemaGenerator schema_generator(conf, table_count, column_count, row_count);
    schema_generator.generate_schema(out);
    DMLGenerator *generator = NULL;
    if ("select" == type) {
        generator = new SelectGenerator(conf);
    } else if ("insert" == type) {
        generator = new InsertGenerator(conf);
    } else if ("update" == type) {
        generator = new UpdateGenerator(conf);
    } else if ("delete" == type) {
        generator = new DeleteGenerator(conf);
    } else {
        print_usage();
        return 0;
    }
    string query;
    for (int i = 0; i < query_count; ++i) {
        generator->generate_query(query);
        out << query << ";\n";
    }
    out.close();
    if (!generator) {
        delete generator;
    }
    return 0;
}