#include "group_by_checker.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "error.h"
using namespace CatDB::Parser;
using namespace CatDB::Common;

GroupByChecker::GroupByChecker(SelectStmt_s &stmt)
	:stmt(stmt)
{
}

GroupByChecker::~GroupByChecker()
{

}

u32 GroupByChecker::check_exprs(Vector<ExprStmt_s> &exprs, bool &in_group_by)
{
	u32 ret = SUCCESS;
	in_group_by = true;
	for (u32 i = 0; in_group_by && i < exprs.size(); ++i) {
		CHECK(check_expr(exprs[i], in_group_by));
	}
	return ret;
}

u32 GroupByChecker::check_expr(ExprStmt_s &expr_stmt, bool &in_group_by)
{
    u32 ret = SUCCESS;
    in_group_by = false;
    MY_ASSERT(expr_stmt);
	switch (expr_stmt->expr_type())
	{
	case ExprStmt::Const:
	{
        in_group_by = true;
		break;
	}
	case ExprStmt::Column:
	{
		ColumnStmt_s column_stmt = expr_stmt;
		if (!stmt->table_ids.has_member(column_stmt->table_id)) {
			in_group_by = true;
		} else {
			in_group_by = check_expr_in_group_by(expr_stmt);
		}
		break;
	}
	case ExprStmt::SubQuery:
	{
        SubQueryStmt_s query = expr_stmt;
        in_group_by = true;
        for (u32 i = 0; in_group_by && i < query->get_params().size(); ++i) {
            in_group_by = check_expr_in_group_by(query->get_params()[i]);
        }
		break;
	}
	case ExprStmt::List:
	{
		ListStmt_s list = expr_stmt;
        in_group_by = true;
		for (u32 i = 0; in_group_by && i < list->size(); ++i) {
			CHECK(check_expr(list->at(i), in_group_by));
		}
		break;
	}
	case ExprStmt::Aggregate:
	{
		in_group_by = true;
		break;
	}
	case ExprStmt::OperationExpr:
	{
        in_group_by = check_expr_in_group_by(expr_stmt);
        if (!in_group_by) {
            CHECK(check_exprs(expr_stmt->params, in_group_by));
        }
		break;
	}
	default:
		LOG_ERR("unknown expr stmt");
		ret = ERROR_LEX_STMT;
	}
	return ret;
}

bool GroupByChecker::check_expr_in_group_by(ExprStmt_s expr)
{
    if (stmt->group_exprs.empty()) {
        return false;
    } else {
        bool find = false;
        for (u32 i = 0; !find && i < stmt->group_exprs.size(); ++i) {
            if (expr->same_as(stmt->group_exprs[i])) {
                find = true;
            }
        }
        return find;
    }
}
