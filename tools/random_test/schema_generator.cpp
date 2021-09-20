#include "schema_generator.h"

void SchemaGenerator::generate_schema(std::ofstream &out)
{
    string drop_database = "DROP DATABASE IF EXISTS RANDOM_TEST;";
    string create_database = "CREATE DATABASE RANDOM_TEST;";
    string use_database = "USE RANDOM_TEST;";
    out << drop_database << "\n";
    out << create_database << "\n";
    out << use_database << "\n";
    for (int i = 1; i <= table_count; ++i) {
        string table_name = "T" + std::to_string(i);
        string table_define;
        vector<string> column_tpye;
        generate_table(table_name, column_tpye, table_define);
        out << table_define << "\n";
        for (int j = 0; j < max_row_count; ++j) {
            string table_data;
            generate_table_data(table_name, column_tpye, table_data);
            out << table_data << "\n";
        }
	out << "SELECT * FROM " << table_name << ";\n";
        conf.tables.push_back(table_name);
    }
    for (int i = 1; i <= column_count; ++i) {
        string column = "C" + std::to_string(i);
        conf.columns.push_back(column);
    }
}

void SchemaGenerator::generate_table(string &table_name, vector<string> &column_tpye, string &table_define)
{
    static vector<string> types = {"INT", "NUMERIC", "DOUBLE", "VARCHAR", "DATETIME"};
    table_define = "DROP TABLE IF EXISTS " + table_name + ";\n";
    table_define += "CREATE TABLE " + table_name + "(";
    for (int i = 1; i <= column_count; ++i) {
        string type = "NUMERIC";random_list(types);
        table_define += "C" + std::to_string(i) + " " + type;
        if (i < column_count) {
            table_define += ", ";
        }
        column_tpye.push_back(type);
    }
    table_define += ");";
}

void SchemaGenerator::generate_table_data(string &table_name, vector<string> &column_tpye, string &table_data)
{
    table_data = "INSERT INTO " + table_name + " VALUES(";
    for (int i = 0; i < column_tpye.size(); ++i) {
        if (column_tpye[i] == "VARCHAR") {
            table_data += random_string();
        } else if (column_tpye[i] == "DATETIME") {
            table_data += random_time();
        } else {
            table_data += random_number(max_row_count);
        }
        if (i < column_tpye.size() - 1) {
            table_data += ", ";
        }
    }
    table_data += ");";
}
