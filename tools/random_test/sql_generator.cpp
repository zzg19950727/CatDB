#include "select_generator.h"
#include "update_generator.h"
#include "delete_generator.h"
#include "insert_generator.h"
#include "schema_generator.h"
#include "config_read.h"
#include <iostream>
#include <ctime>

void print_usage()
{
    std::cout << "SqlGenerator config_file case_file" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        print_usage();
        return 0;
    }
	std::srand(std::time(nullptr));
    Config conf;
	CatDB::Server::Config file_config(argv[1]);
    string type = file_config.value("query_type");
    int table_count = file_config.int_value("table_count");
    int column_count = file_config.int_value("column_count");
    int row_count = file_config.int_value("row_count");
    int query_count = file_config.int_value("query_count");
	table_count = table_count < 1 ? 1 : table_count;
	column_count = column_count < 1 ? 1 : column_count;
	row_count = row_count < 1 ? 1 : row_count;
	query_count = query_count < 1 ? 1 : query_count;	

    conf.max_table_count = file_config.int_value("max_table_count_in_one_stmt");
	conf.min_table_count = file_config.int_value("min_table_count_in_one_stmt");
    conf.max_query_count = file_config.int_value("max_stmt_count");
    conf.max_expr_length = file_config.int_value("max_expr_length");
	conf.use_simple_expr = file_config.int_value("use_simple_expr");;
    conf.can_use_set = file_config.int_value("can_use_set");
    conf.can_use_distinct = file_config.int_value("can_use_distinct");
    conf.can_use_view = file_config.int_value("can_use_view");
    conf.can_use_joined_table = file_config.int_value("can_use_joined_table");
    conf.can_use_group_by = file_config.int_value("can_use_group_by");
    conf.can_use_having = file_config.int_value("can_use_having");
    conf.can_use_order_by = file_config.int_value("can_use_order_by");
    conf.can_use_limit = file_config.int_value("can_use_limit");
    conf.can_use_subquery = file_config.int_value("can_use_subquery");
    conf.can_use_where = file_config.int_value("can_use_where");
	conf.use_scalar_group_by = file_config.int_value("use_scalar_group_by");
	conf.is_single_output = file_config.int_value("is_single_output");

    std::ofstream out;
    out.open(argv[2]);
    SchemaGenerator schema_generator(conf, table_count, column_count, row_count);
    schema_generator.generate_schema(out);
    DMLGenerator *generator = NULL;
    
    string query;
    for (int i = 0; i < query_count; ++i) {
        conf.query_count = 0;
        conf.alias_table_id = 1;
        if ("select" == type) {
            SelectGenerator *sel_generator = NULL;
            if (conf.can_use_set) {
                sel_generator = new SetGenerator(conf);
            } else {
                sel_generator = new SelectGenerator(conf);
            }
            if (conf.is_single_output) {	
                sel_generator->set_is_single();
            }
            generator = sel_generator;
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
        generator->generate_query(query);
		out << "EXPLAIN " << query << ";\n";
        out << query << ";\n";
        if (!generator) {
            delete generator;
            generator = NULL;
        }
    }
    out.close();
    return 0;
}
