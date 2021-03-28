#ifndef TPCH_H
#define TPCH_H
void init_db();
void create_table();
void load_nation_data();
void load_region_data();
void load_supplier_data();
void load_part_data();
void load_customer_data();
void load_partsupp_data();
void load_orders_data();
void load_lineitem_data();
#endif