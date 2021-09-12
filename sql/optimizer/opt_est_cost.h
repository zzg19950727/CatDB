#ifndef OPT_EST_COST_H
#define OPT_EST_COST_H
struct TableCostParam {
double rows;
double block_count;
double column_count;
};
struct JoinCostParam {
double left_rows;
double left_cost;
double right_rows;
double right_cost;
};
class ExtCostUtil {
double cost_table_scan(TableCostParam &param);
double cost_hash_join(JoinCostParam &param);
double cost_nl_join(JoinCostParam &param);
};

#endif	//OPT_EST_COST_H
