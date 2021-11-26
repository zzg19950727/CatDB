#ifndef TPCH_H
#define TPCH_H
#include "query_ctx.h"
using namespace CatDB::Sql;

class LoadTpch {
    QueryCtx_s query_ctx;
public:
    LoadTpch(QueryCtx_s &query_ctx);
    ~LoadTpch();

    void create_table();
    void load_nation_data();
    void load_region_data();
    void load_supplier_data();
    void load_part_data();
    void load_customer_data();
    void load_partsupp_data();
    void load_orders_data();
    void load_lineitem_data();
private:
    bool execute_sql(const String& query);
    void splite(const String& line, char c, Vector<String>& list);
};

#endif