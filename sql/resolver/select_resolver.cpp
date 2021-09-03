#include "select_resolver.h"
#include "group_by_checker.h"
#include "schema_checker.h"
#include "schema_guard.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "object.h"
#include "error.h"
using namespace CatDB::Parser;
using namespace CatDB::Common;
using namespace CatDB::Sql;

SelectResolver::SelectResolver(SelectStmt_s &stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx)
    :DMLResolver(stmt, query_ctx, resolve_ctx),
    select_stmt(stmt)
{
    
}

SelectResolver::~SelectResolver()
{

}
u32 SelectResolver::resolve_stmt()
{
    u32 ret = SUCCESS;
    CHECK(resolve_from_stmt());
    CHECK(resolve_where_stmt());
    if (select_stmt->group_exprs.size() > 0) {
        CHECK(resolve_exprs(select_stmt->group_exprs, resolve_ctx));
        if (select_stmt->having_stmt.size() == 1) {
            ExprStmt_s having_expr = select_stmt->having_stmt[0];
            select_stmt->having_stmt.clear();
            CHECK(flatten_exprs(having_expr, select_stmt->having_stmt));
            can_use_aggr_func = true;
            CHECK(resolve_exprs(select_stmt->having_stmt, resolve_ctx));
        }
    }
    can_use_aggr_func = true;
    Vector<ExprStmt_s> order_exprs;
    CHECK(resolve_select_list());
    CHECK(resolve_order_exprs(order_exprs));
    if (!aggr_funcs.empty() || !select_stmt->group_exprs.empty()) {
        CHECK(select_stmt->formalize());
        GroupByChecker group_by_checker(select_stmt);
        bool in_group_by = false;
        CHECK(group_by_checker.check_exprs(select_stmt->select_expr_list, in_group_by));
        MY_ASSERT(in_group_by);
        CHECK(group_by_checker.check_exprs(select_stmt->having_stmt, in_group_by));
        MY_ASSERT(in_group_by);
        CHECK(group_by_checker.check_exprs(order_exprs, in_group_by));
        MY_ASSERT(in_group_by);
    }
    return ret;
}

u32 SelectResolver::resolve_select_list()
{
    u32 ret = SUCCESS;
    Vector<ExprStmt_s> &old_select_exprs = select_stmt->select_expr_list;
    Vector<ExprStmt_s> new_select_exprs;
    for (u32 i = 0; i < old_select_exprs.size(); ++i) {
        ret = resolve_expr(old_select_exprs[i], resolve_ctx);
        if (IS_ALL_COLUMN_STMT == ret) {
            CHECK(resolve_all_column(old_select_exprs[i], new_select_exprs));
        } else if (SUCCESS == ret) {
            new_select_exprs.push_back(old_select_exprs[i]);
        }
    }
    old_select_exprs.clear();
    append(old_select_exprs, new_select_exprs);
    return ret;
}

u32 SelectResolver::resolve_all_column(ExprStmt_s &stmt, Vector<ExprStmt_s> &columns)
{
    u32 ret = SUCCESS;
    MY_ASSERT(stmt, ExprStmt::Column == stmt->expr_type())
	ColumnStmt_s column = stmt;
	if (column->table == "*") {
		for (u32 i = 0; i < resolve_ctx.cur_tables.size(); ++i) {
            CHECK(get_all_column_from_table(resolve_ctx.cur_tables[i], columns));
        }
	}
	else {
        bool find = false;
		for (u32 i = 0; !find && i < resolve_ctx.cur_tables.size(); ++i) {
            if (column->table == resolve_ctx.cur_tables[i]->alias_name) {
                CHECK(get_all_column_from_table(resolve_ctx.cur_tables[i], columns));
                find = true;
            }
        }
        for (u32 i = 0; !find && i < resolve_ctx.parent_tables.size(); ++i) {
            if (column->table == resolve_ctx.parent_tables[i]->alias_name) {
                CHECK(get_all_column_from_table(resolve_ctx.parent_tables[i], columns));
                find = true;
            }
        }
        MY_ASSERT(find);
	}
	return ret;
}

u32 SelectResolver::get_all_column_from_table(TableStmt_s &table, Vector<ExprStmt_s> &columns)
{
    u32 ret = SUCCESS;
    if (table->is_basic_table()) {
        CHECK(get_all_column_from_basic_table(table, columns));
    } else if (table->is_view_table()) {
        CHECK(get_all_column_from_view_table(table, columns));
    } else {
        LOG_ERR("unexpect table item type", K(table));
        ret = ERROR_LEX_STMT;
    }
    return ret;
}

u32 SelectResolver::get_all_column_from_basic_table(BasicTableStmt_s table, Vector<ExprStmt_s> &columns)
{
    u32 ret = SUCCESS;
    if (table->is_dual_table()) {
        columns.push_back(ConstStmt::make_const_stmt(Number::make_object(1, -1)));
    } else {
        SchemaChecker_s checker = SchemaChecker::make_schema_checker();
        MY_ASSERT(checker);
        Vector<ColumnInfo_s> column_infos;
        CHECK(checker->get_all_columns(table->ref_table_id, column_infos));
        std::sort(column_infos.begin(), column_infos.end(), [](const ColumnInfo_s& lhs, const ColumnInfo_s& rhs){ return lhs->column_id < rhs->column_id; } );
        for (u32 i = 0; i < column_infos.size(); ++i) {
            ColumnStmt_s col = ColumnStmt::make_column_stmt(table->alias_name, column_infos[i]->column_name);
            col->table_id = table->table_id;
            col->column_id = column_infos[i]->column_id;
            col->alias_name = column_infos[i]->column_name;
            columns.push_back(col);
        }
    }
    return ret;
}

u32 SelectResolver::get_all_column_from_view_table(ViewTableStmt_s table, Vector<ExprStmt_s> &columns)
{
    u32 ret = SUCCESS;
    SelectStmt_s& query = table->ref_query;
    MY_ASSERT(query);
    for (u32 i = 0; i < query->select_expr_list.size(); ++i) {
        String &alias_name = query->select_expr_list[i]->alias_name;
        ColumnStmt_s col = ColumnStmt::make_column_stmt(table->alias_name, alias_name);
        col->table_id = table->table_id;
        col->column_id = i;
        col->alias_name = alias_name;
        columns.push_back(col);
    }
    return ret;
}

u32 SelectResolver::resolve_order_exprs(Vector<ExprStmt_s> &order_exprs)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < select_stmt->order_exprs.size(); ++i) {
        CHECK(resolve_expr(select_stmt->order_exprs[i]->order_expr, resolve_ctx));
        order_exprs.push_back(select_stmt->order_exprs[i]->order_expr);
    }
    return ret;
}

SetResolver::SetResolver(SetStmt_s &stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx)
    :DMLResolver(stmt, query_ctx, resolve_ctx),
    set_stmt(stmt)
{
    
}

SetResolver::~SetResolver()
{

}
u32 SetResolver::resolve_stmt()
{
    u32 ret = SUCCESS;
    MY_ASSERT(set_stmt->left_query->is_select_stmt());
    MY_ASSERT(set_stmt->right_query->is_select_stmt());
    CHECK(DMLResolver::resolve_stmt(set_stmt->left_query, query_ctx, resolve_ctx));
    CHECK(DMLResolver::resolve_stmt(set_stmt->right_query, query_ctx, resolve_ctx));
    MY_ASSERT(set_stmt->left_query->select_expr_list.size() == set_stmt->right_query->select_expr_list.size());
    for (u32 i = 0; i < set_stmt->left_query->select_expr_list.size(); ++i) {
        ExprStmt_s select_expr = SetExprStmt::make_set_expr(set_stmt->set_op, i);
        select_expr->alias_name = set_stmt->left_query->select_expr_list[i]->alias_name;
        set_stmt->select_expr_list.push_back(select_expr);
    }
    return ret;
}