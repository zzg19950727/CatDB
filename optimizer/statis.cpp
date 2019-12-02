#include "schema_checker.h"
#include "select_plan.h"
#include "expr_stmt.h"
#include "statis.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Parser;

u32 CatDB::Sql::get_table_row_count(const Vector<TableStmt*>& table_list)
{
	for (u32 i = 0; i < table_list.size(); ++i) {
		if (table_list[i]->is_tmp_table) {
			SelectPlan* select_plan = dynamic_cast<SelectPlan*>(table_list[i]->subplan.get());
			if (select_plan) {
				table_list[i]->select_rows = select_plan->get_select_rows();
			}
			else {
				table_list[i]->select_rows = 1;
			}
		}
		else {
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			table_list[i]->select_rows = checker->statis_table_row_count(table_list[i]->database, table_list[i]->table_name);
		}
	}
	return SUCCESS;
}

u32 CatDB::Sql::cal_table_select_rows(const Vector<TableStmt*>& table_list, 
								const HashMap<TableStmt*, Expression_s>& table_filters)
{
	get_table_row_count(table_list);
	for (u32 i = 0; i < table_list.size(); ++i) {
		//暂时不支持NDV缩放计划
		if (table_list[i]->is_tmp_table) {
			continue;
		}
		auto iter = table_filters.find(table_list[i]);
		if (iter == table_filters.cend()) {
			continue;
		}
		const Expression_s& filter = iter->second;

	}
	return SUCCESS;
}