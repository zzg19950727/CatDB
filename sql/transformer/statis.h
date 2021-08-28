#ifndef STATIS_H
#define STATIS_H
#include "type.h"
namespace CatDB {
	namespace Parser {
		DECLARE(TableStmt);
	}
	namespace Sql {
		DECLARE(Expression);
		using Parser::TableStmt;
		u32 get_table_row_count(TableStmt* table);
		double scale_ndv(const Expression_s& expr, double filte_rows);
		u32 cal_table_select_rows(TableStmt* table, const Expression_s& filter);
		double cal_join_select_rows(double left_rows, double right_rows, const Expression_s& join_cond);
		double cal_expression_selectivity(const Expression_s& expr, bool scale_ndv = false);
		double cal_single_expression_selectivity(const Expression_s& expr, bool scale_ndv = false);
	}
}

#endif	//STATIS_H