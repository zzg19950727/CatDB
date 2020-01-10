#include "schema_checker.h"
#include "select_plan.h"
#include "expression.h"
#include "expr_stmt.h"
#include "object.h"
#include "statis.h"
#include "error.h"
#include "log.h"
#include <cmath>

using namespace CatDB::Sql;
using namespace CatDB::Common;
using namespace CatDB::Parser;

u32 CatDB::Sql::get_table_row_count(TableStmt* table)
{
	if (table->is_tmp_table) {
		SelectPlan* select_plan = dynamic_cast<SelectPlan*>(table->subplan.get());
		if (select_plan) {
			table->row_count = select_plan->get_select_rows();
		}
		else {
			table->row_count = 1;
		}
	}
	else {
		SchemaChecker_s checker = SchemaChecker::make_schema_checker();
		table->row_count = checker->statis_table_row_count(table->database, table->table_name);
	}
	return SUCCESS;
}

double CatDB::Sql::scale_ndv(const Expression_s & expr, double filte_rows)
{
	double new_ndv = filte_rows;
	if (expr->get_type() == Expression::Column) {
		ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
		u32 tid, cid;
		col_expr->col_desc.get_tid_cid(tid, cid);
		SchemaChecker_s checker = SchemaChecker::make_schema_checker();
		u32 ndv = checker->statis_column_ndv(tid, cid);
		u32 row_count = checker->statis_table_row_count(tid);
		if (filte_rows < row_count) {
			new_ndv *= (1 - pow((1 - filte_rows / row_count), row_count / ndv));
		}
	}
	return new_ndv;
}

u32 CatDB::Sql::cal_table_select_rows(TableStmt* table, const Expression_s& filter)
{
	double sel = 1;
	if (filter) {
		sel = cal_expression_selectivity(filter);
	}
	get_table_row_count(table);
	table->select_rows = table->row_count * sel;
	if (table->select_rows < 1) {
		table->select_rows = 1;
	}
	return SUCCESS;
}

double CatDB::Sql::cal_join_select_rows(double left_rows, double right_rows, const Expression_s& join_cond)
{
	double sel = 1;
	if (join_cond) {
		sel = cal_expression_selectivity(join_cond, true);
	}

	if (1.0 / (left_rows > right_rows ? left_rows : right_rows) > sel) {
		sel = 1.0 / (left_rows > right_rows ? left_rows : right_rows);
	}
	double ret = right_rows * sel * left_rows;
	if (ret < 1)
		ret = 1;
	return ret;
}

double CatDB::Sql::cal_expression_selectivity(const Expression_s & expr, bool scale_ndv)
{
	double ret = 1.0;
	switch (expr->get_type())
	{
	case Expression::Binary:
	{
		BinaryExpression* bin_expr = dynamic_cast<BinaryExpression*>(expr.get());
		if (bin_expr->op.get_type() == ExprStmt::OP_OR) {
			double ret1 = cal_expression_selectivity(bin_expr->first_expr, scale_ndv);
			double ret2 = cal_expression_selectivity(bin_expr->second_expr, scale_ndv);
			ret = ret1 + ret2;
			if (ret > 1) {
				ret = 1.0;
			}
		}
		else if (bin_expr->op.get_type() == ExprStmt::OP_AND) {
			double ret1 = cal_expression_selectivity(bin_expr->first_expr, scale_ndv);
			double ret2 = cal_expression_selectivity(bin_expr->second_expr, scale_ndv);
			ret = ret1 * ret2;
		}
		else {
			ret = cal_single_expression_selectivity(expr, scale_ndv);
		}
		break;
	}
	default:
		ret = cal_single_expression_selectivity(expr, scale_ndv);
	}
	return ret;
}

double cal_const_or_column_null_selectivity(const Expression_s & expr)
{
	double ret = 1.0;
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	switch (expr->get_type())
	{
	case Expression::Const:
	{
		ConstExpression* const_expr = dynamic_cast<ConstExpression*>(expr.get());
		Object_s result = const_expr->get_result(nullptr);
		ret = result->is_null();
		break;
	}
	case Expression::Column:
	{
		ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
		u32 tid, cid;
		col_expr->col_desc.get_tid_cid(tid, cid);
		u32 row_count = checker->statis_table_row_count(tid);
		u32 null_count = checker->statis_column_null_count(tid, cid);
		ret = 1.0 * null_count / row_count;
		break;
	}
	default:
		ret = 1.0;
	}
	return ret;
}

double cal_equal_selectivity(const Expression_s & left_expr, const Expression_s & right_expr, bool scale_ndv)
{
	double ret = 1.0 / 3.0;
	if (left_expr->get_type() == Expression::Const) {
		if (right_expr->get_type() == Expression::Const) {
			Object_s v1 = left_expr->get_result(nullptr);
			Object_s v2 = right_expr->get_result(nullptr);
			ret = v1->operator==(v2)->bool_value();
		}
		else if (right_expr->get_type() == Expression::Column) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(right_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 ndv = checker->statis_column_ndv(tid, cid);
			ret = 1.0 / ndv;
		}
	}
	else if (left_expr->get_type() == Expression::Column) {
		if (right_expr->get_type() == Expression::Const) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(left_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 ndv = checker->statis_column_ndv(tid, cid);
			ret = 1.0 / ndv;
		}
		else if (right_expr->get_type() == Expression::Column) {
			ColumnExpression* left_col_expr = dynamic_cast<ColumnExpression*>(left_expr.get());
			u32 tid, cid;
			left_col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 ndv1 = checker->statis_column_ndv(tid, cid);

			ColumnExpression* right_col_expr = dynamic_cast<ColumnExpression*>(right_expr.get());
			right_col_expr->col_desc.get_tid_cid(tid, cid);
			u32 ndv2 = checker->statis_column_ndv(tid, cid);

			if (scale_ndv && left_col_expr->table && right_col_expr->table) {
				ndv1 *= (1 - pow((1 - left_col_expr->table->select_rows / left_col_expr->table->row_count), left_col_expr->table->row_count / ndv1));
				ndv2 *= (1 - pow((1 - right_col_expr->table->select_rows / right_col_expr->table->row_count), right_col_expr->table->row_count / ndv2));
			}
			ret = ndv1 > ndv2 ? 1.0 / ndv1 : 1.0 / ndv2;
		}
	}
	return ret;
}

double cal_greater_selectivity(const Expression_s & left_expr, const Expression_s & right_expr)
{
	double ret = 1.0 / 3.0;
	if (left_expr->get_type() == Expression::Const) {
		if (right_expr->get_type() == Expression::Const) {
			Object_s v1 = left_expr->get_result(nullptr);
			Object_s v2 = right_expr->get_result(nullptr);
			ret = v1->operator>(v2)->bool_value();
		}
		else if (right_expr->get_type() == Expression::Column) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(right_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 max_value = checker->statis_column_max_value(tid, cid);
			u32 min_value = checker->statis_column_min_value(tid, cid);
			Object_s v1 = left_expr->get_result(nullptr);
			if (v1->get_type() == T_NUMBER) {
				Number* col_value = dynamic_cast<Number*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 0.0;
				}
				else if (value >= max_value) {
					ret = 1.0;
				}
				else {
					ret = (value - min_value)*1.0 / (max_value - min_value);
				}
			}
			else if (v1->get_type() == T_DATETIME) {
				DateTime* col_value = dynamic_cast<DateTime*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 0.0;
				}
				else if (value >= max_value) {
					ret = 1.0;
				}
				else {
					ret = (value - min_value)*1.0 / (max_value - min_value);
				}
			}
		}
	}
	else if (left_expr->get_type() == Expression::Column) {
		if (right_expr->get_type() == Expression::Const) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(left_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 max_value = checker->statis_column_max_value(tid, cid);
			u32 min_value = checker->statis_column_min_value(tid, cid);
			Object_s v1 = right_expr->get_result(nullptr);
			if (v1->get_type() == T_NUMBER) {
				Number* col_value = dynamic_cast<Number*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 1.0;
				}
				else if (value >= max_value) {
					ret = 0.0;
				}
				else {
					ret = (max_value - value)*1.0 / (max_value - min_value);
				}
			}
			else if (v1->get_type() == T_DATETIME) {
				DateTime* col_value = dynamic_cast<DateTime*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 1.0;
				}
				else if (value >= max_value) {
					ret = 0.0;
				}
				else {
					ret = (max_value - value)*1.0 / (max_value - min_value);
				}
			}
		}
		else if (right_expr->get_type() == Expression::Column) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(left_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 max_value1 = checker->statis_column_max_value(tid, cid);
			u32 min_value1 = checker->statis_column_min_value(tid, cid);

			col_expr = dynamic_cast<ColumnExpression*>(right_expr.get());
			col_expr->col_desc.get_tid_cid(tid, cid);
			u32 max_value2 = checker->statis_column_max_value(tid, cid);
			u32 min_value2 = checker->statis_column_min_value(tid, cid);

			if (min_value1 >= max_value2) {
				ret = 1.0;
			}
			else if (min_value2 >= max_value1) {
				ret = 0.0;
			}
		}
	}
	return ret;
}

double cal_less_selectivity(const Expression_s & left_expr, const Expression_s & right_expr)
{
	double ret = 1.0 / 3.0;
	if (left_expr->get_type() == Expression::Const) {
		if (right_expr->get_type() == Expression::Const) {
			Object_s v1 = left_expr->get_result(nullptr);
			Object_s v2 = right_expr->get_result(nullptr);
			ret = v1->operator<(v2)->bool_value();
		}
		else if (right_expr->get_type() == Expression::Column) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(right_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 max_value = checker->statis_column_max_value(tid, cid);
			u32 min_value = checker->statis_column_min_value(tid, cid);
			Object_s v1 = left_expr->get_result(nullptr);
			if (v1->get_type() == T_NUMBER) {
				Number* col_value = dynamic_cast<Number*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 1.0;
				}
				else if (value >= max_value) {
					ret = 0.0;
				}
				else {
					ret = (max_value - value)*1.0 / (max_value - min_value);
				}
			}
			else if (v1->get_type() == T_DATETIME) {
				DateTime* col_value = dynamic_cast<DateTime*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 1.0;
				}
				else if (value >= max_value) {
					ret = 0.0;
				}
				else {
					ret = (max_value - value)*1.0 / (max_value - min_value);
				}
			}
		}
	}
	else if (left_expr->get_type() == Expression::Column) {
		if (right_expr->get_type() == Expression::Const) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(left_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 max_value = checker->statis_column_max_value(tid, cid);
			u32 min_value = checker->statis_column_min_value(tid, cid);
			Object_s v1 = right_expr->get_result(nullptr);
			if (v1->get_type() == T_NUMBER) {
				Number* col_value = dynamic_cast<Number*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 0.0;
				}
				else if (value >= max_value) {
					ret = 1.0;
				}
				else {
					ret = (value - min_value)*1.0 / (max_value - min_value);
				}
			}
			else if (v1->get_type() == T_DATETIME) {
				DateTime* col_value = dynamic_cast<DateTime*>(v1.get());
				double value = col_value->value();
				if (value <= min_value) {
					ret = 0.0;
				}
				else if (value >= max_value) {
					ret = 1.0;
				}
				else {
					ret = (value - min_value)*1.0 / (max_value - min_value);
				}
			}
		}
		else if (right_expr->get_type() == Expression::Column) {
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(left_expr.get());
			u32 tid, cid;
			col_expr->col_desc.get_tid_cid(tid, cid);
			SchemaChecker_s checker = SchemaChecker::make_schema_checker();
			u32 max_value1 = checker->statis_column_max_value(tid, cid);
			u32 min_value1 = checker->statis_column_min_value(tid, cid);

			col_expr = dynamic_cast<ColumnExpression*>(right_expr.get());
			col_expr->col_desc.get_tid_cid(tid, cid);
			u32 max_value2 = checker->statis_column_max_value(tid, cid);
			u32 min_value2 = checker->statis_column_min_value(tid, cid);

			if (min_value1 >= max_value2) {
				ret = 0.0;
			}
			else if (min_value2 >= max_value1) {
				ret = 1.0;
			}
		}
	}
	return ret;
}

double cal_between_selectivity(const Expression_s& first_expr, const Expression_s& second_expr, const Expression_s& third_expr)
{
	double ret = 1.0 / 3.0;
	if (first_expr->get_type() == Expression::Column
		&& second_expr->get_type() == Expression::Const
		&& third_expr->get_type() == Expression::Const) {
		ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(first_expr.get());
		u32 tid, cid;
		col_expr->col_desc.get_tid_cid(tid, cid);
		SchemaChecker_s checker = SchemaChecker::make_schema_checker();
		u32 max_value = checker->statis_column_max_value(tid, cid);
		u32 min_value = checker->statis_column_min_value(tid, cid);

		Object_s v1 = second_expr->get_result(nullptr);
		Object_s v2 = third_expr->get_result(nullptr);
		if (v1->get_type() == T_NUMBER
			&& v2->get_type() == T_NUMBER) {
			Number* col_value = dynamic_cast<Number*>(v1.get());
			double value1 = col_value->value();
			col_value = dynamic_cast<Number*>(v2.get());
			double value2 = col_value->value();
			if (value1 > value2) {
				ret = 0.0;
			}
			else if (min_value >= max_value) {
				ret = (min_value >= value1) && (min_value <= value2);
			}
			else {
				ret = (value2 - value1)*1.0 / (max_value - min_value);
			}
		}
	}
	return ret;
}

double CatDB::Sql::cal_single_expression_selectivity(const Expression_s & expr, bool scale_ndv)
{
	double ret = 1.0 / 3.0;
	SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	switch (expr->get_type())
	{
	case Expression::Const:
	{
		ConstExpression* const_expr = dynamic_cast<ConstExpression*>(expr.get());
		Object_s result = const_expr->get_result(nullptr);
		ret = result->bool_value();
		break;
	}
	case Expression::Column:
	{
		ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
		u32 tid, cid;
		col_expr->col_desc.get_tid_cid(tid, cid);
		u32 row_count = checker->statis_table_row_count(tid);
		u32 null_count = checker->statis_column_null_count(tid, cid);
		ret = 1.0 * null_count / row_count;
		break;
	}
	case Expression::Subplan:
	{
		SubplanExpression* subplan_expr = dynamic_cast<SubplanExpression*>(expr.get());
		if (subplan_expr->subplan->type() == Plan::SELECT) {
			SelectPlan* select_plan = dynamic_cast<SelectPlan*>(subplan_expr->subplan.get());
			ret = select_plan->get_select_rows() > 0;
		}
		break;
	}
	case Expression::Unary:
	{
		UnaryExpression* unary_expr = dynamic_cast<UnaryExpression*>(expr.get());
		switch (unary_expr->op.get_type())
		{
		case ExprStmt::OP_MINUS:
		{
			//not support yet
		}
		case ExprStmt::OP_NOT:
		{
			//not support yet
		}
		case ExprStmt::OP_EXISTS:
		{
			//not support yet
		}
		case ExprStmt::OP_NOT_EXISTS:
		{
			//not support yet
		}
		case ExprStmt::OP_IS_NULL:
		{
			ret = cal_const_or_column_null_selectivity(unary_expr->expr);
			break;
		}
		case ExprStmt::OP_IS_NOT_NULL:
		{
			ret = 1.0 - cal_const_or_column_null_selectivity(unary_expr->expr);
			break;
		}
		}
	}
	case Expression::Binary:
	{
		BinaryExpression* bin_expr = dynamic_cast<BinaryExpression*>(expr.get());
		switch (bin_expr->op.get_type())
		{
		case ExprStmt::OP_ADD:
		case ExprStmt::OP_SUB:
		case ExprStmt::OP_MUL:
		case ExprStmt::OP_DIV:
		{
			double ret1 = 1.0 - cal_const_or_column_null_selectivity(bin_expr->first_expr);
			double ret2 = 1.0 - cal_const_or_column_null_selectivity(bin_expr->second_expr);
			ret = ret1 * ret2;
			break;
		}
		case ExprStmt::OP_EQ:
		{
			ret = cal_equal_selectivity(bin_expr->first_expr, bin_expr->second_expr, scale_ndv);
			break;
		}
		case ExprStmt::OP_NE:
		{
			ret = 1.0 - cal_equal_selectivity(bin_expr->first_expr, bin_expr->second_expr, scale_ndv);
			break;
		}
		case ExprStmt::OP_GT:
		case ExprStmt::OP_GE:
		{
			ret = cal_greater_selectivity(bin_expr->first_expr, bin_expr->second_expr);
			break;
		}
		case ExprStmt::OP_LT:
		case ExprStmt::OP_LE:
		{
			ret = cal_less_selectivity(bin_expr->first_expr, bin_expr->second_expr);
			break;
		}
		case ExprStmt::OP_LIKE:
		{
			ret = 1.0 / 3;
			break;
		}
		case ExprStmt::OP_NOT_LIKE:
		{
			ret = 2.0 / 3;
			break;
		}
		}
		break;
	}
	case Expression::Ternary:
	{
		TernaryExpression* ternary_expr = dynamic_cast<TernaryExpression*>(expr.get());
		switch (ternary_expr->op.get_type())
		{
		case ExprStmt::OP_BETWEEN:
		{
			ret = cal_between_selectivity(ternary_expr->first_expr, ternary_expr->second_expr, ternary_expr->third_expr);
			break;
		}
		case ExprStmt::OP_NOT_BETWEEN:
		{
			ret = 1.0 - cal_between_selectivity(ternary_expr->first_expr, ternary_expr->second_expr, ternary_expr->third_expr);
			break;
		}
		}
		break;
	}
	}
	return ret;
}
