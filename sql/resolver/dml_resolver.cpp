#include "schema_checker.h"
#include "schema_guard.h"
#include "select_resolver.h"
#include "update_resolver.h"
#include "delete_resolver.h"
#include "insert_resolver.h"
#include "select_stmt.h"
#include "update_stmt.h"
#include "delete_stmt.h"
#include "insert_stmt.h"
#include "dml_resolver.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "query_ctx.h"
#include "stmt.h"
#include "object.h"
#include "error.h"
using namespace CatDB::Parser;
using namespace CatDB::Common;
using namespace CatDB::Sql;

DMLResolver::DMLResolver(DMLStmt_s stmt, QueryCtx_s &query_ctx, ResolveCtx &resolve_ctx)
    :stmt(stmt),
    query_ctx(query_ctx),
    resolve_ctx(resolve_ctx),
    can_use_aggr_func(false)
{
    resolve_ctx.cur_tables.clear();
    stmt->stmt_id = query_ctx->generate_stmt_id();
}

DMLResolver::~DMLResolver()
{

}

u32 DMLResolver::resolve_stmt(Stmt_s stmt, QueryCtx_s &query_ctx, ResolveCtx &resolve_ctx)
{
    u32 ret = SUCCESS;
    MY_ASSERT(stmt);
    switch (stmt->stmt_type()) {
        case Stmt::Select:
        {
            SelectStmt_s select_stmt = stmt;
            SelectResolver resolver(select_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Stmt::Update:
        {
            UpdateStmt_s update_stmt = stmt;
            UpdateResolver resolver(update_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Stmt::Delete:
        {
            DeleteStmt_s delete_stmt = stmt;
            DeleteResolver resolver(delete_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Stmt::Insert:
        {
            InsertStmt_s insert_stmt = stmt;
            InsertResolver resolver(insert_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Stmt::SetOperation:
        {
            SetStmt_s set_stmt = stmt;
            SetResolver resolver(set_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        default:
            LOG_ERR("unknown stmt type", K(stmt));
            ret = ERR_UNEXPECTED;
    }
    return ret;
}

u32 DMLResolver::resolve_from_stmt()
{
    u32 ret = SUCCESS;
    MY_ASSERT(stmt);
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        CHECK(resolve_table_item(stmt->from_stmts[i]));
    }
	return ret;
}

u32 DMLResolver::resolve_where_stmt()
{
    u32 ret = SUCCESS;
    MY_ASSERT(stmt);
    if (stmt->where_stmt.size() == 1) {
        ExprStmt_s expr = stmt->where_stmt[0];
        stmt->where_stmt.clear();
        CHECK(flatten_exprs(expr, stmt->where_stmt));
        CHECK(resolve_exprs(stmt->where_stmt, resolve_ctx));
    } else if (stmt->where_stmt.size() > 1) {
        ret = ERR_UNEXPECTED;
    }
    return ret;
}

u32 DMLResolver::resolve_table_item(TableStmt_s& table)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table);
    if (table->is_basic_table()) {
        CHECK(resolve_basic_table_item(table));
    } else if (table->is_joined_table()) {
        CHECK(resolve_joined_table_item(table));
    } else if (table->is_view_table()) {
        CHECK(resolve_view_table_item(table));
    } else {
        LOG_ERR("unknow table type", K(table));
        ret = ERROR_LEX_STMT;
    }
    return ret;
}

u32 DMLResolver::resolve_basic_table_item(BasicTableStmt_s table_stmt)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    if (find_table_name(table_stmt->alias_name)) {
        String err_msg = "table " + table_stmt->alias_name + " exists";
        query_ctx->set_error_msg(err_msg);
        LOG_ERR("same alias table in from list", K(table_stmt));
        ret = NOT_UNIQUE_TABLE;
    } else if (table_stmt->is_dual) {
        table_stmt->table_id = query_ctx->generate_table_id();
        resolve_ctx.cur_tables.push_back(table_stmt);
    } else {
        SchemaChecker_s checker = SchemaChecker::make_schema_checker();
        ret=(checker->get_table_id(table_stmt->database,
                                    table_stmt->table_name,
                                    table_stmt->ref_table_id));
        if (FAIL(ret)) {
            String err_msg = "table " + table_stmt->alias_name + " not exists";
            query_ctx->set_error_msg(err_msg);
            return ret;
        }
        table_stmt->table_id = query_ctx->generate_table_id();
        resolve_ctx.cur_tables.push_back(table_stmt);
    }
    return ret;
}

u32 DMLResolver::resolve_joined_table_item(JoinedTableStmt_s table_stmt)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    CHECK(resolve_table_item(table_stmt->left_table));
    CHECK(resolve_table_item(table_stmt->right_table));
    if (table_stmt->join_condition.size() == 1) {
        ExprStmt_s expr = table_stmt->join_condition[0];
        table_stmt->join_condition.clear();
        CHECK(flatten_exprs(expr, table_stmt->join_condition));
        ResolveCtx temp_ctx;
        CHECK(table_stmt->get_table_items(temp_ctx.cur_tables));
        append(temp_ctx.parent_tables, resolve_ctx.parent_tables);
        CHECK(resolve_exprs(table_stmt->join_condition, temp_ctx));
        append(resolve_ctx.columns_from_parent, temp_ctx.columns_from_parent);
    } else if (table_stmt->join_condition.size() > 1) {
        ret = ERR_UNEXPECTED;
    }
    return ret;
}

u32 DMLResolver::resolve_view_table_item(ViewTableStmt_s table_stmt)
{
    u32 ret = SUCCESS;
    MY_ASSERT(table_stmt);
    if (find_table_name(table_stmt->alias_name)) {
        String err_msg = "table " + table_stmt->alias_name + " exists";
        query_ctx->set_error_msg(err_msg);
        LOG_ERR("same alias table in from list", K(table_stmt));
        ret = NOT_UNIQUE_TABLE;
    } else {
        table_stmt->table_id = query_ctx->generate_table_id();
        resolve_ctx.cur_tables.push_back(table_stmt);
        ResolveCtx temp_ctx;
        append(temp_ctx.parent_tables, resolve_ctx.parent_tables);
        CHECK(resolve_subquery(table_stmt->ref_query, temp_ctx));
        append(resolve_ctx.columns_from_parent, temp_ctx.columns_from_parent);
    }
    return ret;
}

u32 DMLResolver::flatten_exprs(ExprStmt_s& expr, Vector<ExprStmt_s> &expr_list)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr);
    if (expr->is_and_expr()) {
        for (u32 i = 0; i < expr->params.size(); ++i) {
            CHECK(flatten_exprs(expr->params[i], expr_list));
        }
    } else {
        expr_list.push_back(expr);
    }
    return ret;
}

u32 DMLResolver::resolve_exprs(Vector<ExprStmt_s>& expr_list, ResolveCtx &resolve_ctx)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < expr_list.size(); ++i) {
        CHECK(resolve_expr(expr_list[i], resolve_ctx));
    }
    return ret;
}

u32 DMLResolver::resolve_expr(ExprStmt_s& expr_stmt, ResolveCtx &resolve_ctx)
{
	u32 ret = SUCCESS;;
    MY_ASSERT(expr_stmt);
	switch (expr_stmt->expr_type())
	{
	case CONST:
	{
		break;
	}
	case COLUMN:
	{
		ColumnStmt_s column_stmt = expr_stmt;
		if (column_stmt->is_all_column()) {
			ret = IS_ALL_COLUMN_STMT;
		} else {
			CHECK(resolve_column(expr_stmt, resolve_ctx));
		}
		break;
	}
	case SUBQUERY:
	{
        SubQueryStmt_s query = expr_stmt;
        ResolveCtx temp_ctx;
        append(temp_ctx.parent_tables, resolve_ctx.parent_tables);
        append(temp_ctx.parent_tables, resolve_ctx.cur_tables);
		CHECK(resolve_subquery(query->query_stmt, temp_ctx));
        for (uint i = 0; i <temp_ctx.columns_from_parent.size(); ++i) {
            ExecParamStmt_s& exec_param = temp_ctx.columns_from_parent[i];
            ExprStmt_s& correlated_expr = exec_param->get_ref_expr();
            if (COLUMN == correlated_expr->expr_type()) {
                ColumnStmt_s col = correlated_expr;
                if (find_table_item(col->table_id)) {
                    query->add_corrected_exprs(exec_param);
                } else {
                    query->add_corrected_exprs(exec_param);
                }
            } else {
                ret = OPERATION_NOT_SUPPORT;
            }
        }
		break;
	}
	case EXPR_LIST:
	{
		ListStmt_s list = expr_stmt;
		for (u32 i = 0; SUCC(ret) && i < list->size(); ++i) {
			CHECK(resolve_expr(list->at(i), resolve_ctx));
		}
		break;
	}
	case AGG_EXPR:
	{
		if (!can_use_aggr_func) {
			LOG_ERR("aggregate function can only exist in select list or having stmt");
			ret = ERROR_LEX_STMT;
		} else {
            AggrStmt_s agg_stmt = expr_stmt;
            ExprStmt_s aggr_expr = agg_stmt->get_aggr_expr();
		    ret = resolve_expr(aggr_expr, resolve_ctx);
            if (ret == IS_ALL_COLUMN_STMT) {
                if (COUNT == agg_stmt->aggr_func && 
                    !agg_stmt->distinct) {
                    agg_stmt->set_aggr_expr(ConstStmt::make_const_stmt(Number::make_object(1, -1)));
                    ret = SUCCESS;
                } else {
                    ret = HAVE_ALL_COLUMN_STMT;
                }
            }
            aggr_funcs.push_back(agg_stmt);
        }
		break;
	}
	case OP_EXPR:
	{
        for (u32 i = 0; i < expr_stmt->params.size(); ++i) {
            ret = resolve_expr(expr_stmt->params[i], resolve_ctx);
            if (IS_ALL_COLUMN_STMT == ret) {
                ret = HAVE_ALL_COLUMN_STMT;
            } else if (FAIL(ret)) {
                return ret;
            }
        }
		break;
	}
	default:
		LOG_ERR("unknown expr stmt");
		ret = ERROR_LEX_STMT;
	}
	return ret;
}

u32 DMLResolver::resolve_column(ExprStmt_s &expr_stmt, ResolveCtx &resolve_ctx)
{
    u32 ret = SUCCESS;
    bool find = false;
    u32 find_cnt = 0;
    MY_ASSERT(expr_stmt, COLUMN == expr_stmt->expr_type());
    ColumnStmt_s column = expr_stmt;
    if (!column->table.empty()) {
        for (u32 i = 0; !find && i < resolve_ctx.cur_tables.size(); ++i) {
            if (column->table == resolve_ctx.cur_tables[i]->alias_name) {
                ret = resolve_column(resolve_ctx.cur_tables[i], column);
                if (ret == SUCCESS) {
                    find = true;
                } else {
                    ret = SUCCESS;
                }
            }
        }
        for (u32 i = 0; !find && i < resolve_ctx.parent_tables.size(); ++i) {
            if (column->table == resolve_ctx.parent_tables[i]->alias_name) {
                ret = resolve_column(resolve_ctx.parent_tables[i], column);
                if (ret == SUCCESS) {
                    find = true;
                    ExecParamStmt_s exec_param = ExecParamStmt::make_exec_param_stmt(column);
                    resolve_ctx.columns_from_parent.push_back(exec_param);
                    expr_stmt = exec_param;
                } else {
                    ret = SUCCESS;
                }
            }
        }
    } else {
        for (u32 i = 0; i < resolve_ctx.cur_tables.size(); ++i) {
            ret = resolve_column(resolve_ctx.cur_tables[i], column);
            if (ret == SUCCESS) {
                ++find_cnt;
                column->table = resolve_ctx.cur_tables[i]->alias_name;
            } else {
                ret = SUCCESS;
            }
        }
        MY_ASSERT(find_cnt <= 1);
        if (find_cnt == 1) {
            find = true;
        }
        for (u32 i = 0; !find && i < resolve_ctx.parent_tables.size(); ++i) {
            ret = resolve_column(resolve_ctx.parent_tables[i], column);
            if (ret == SUCCESS) {
                find = true;
                column->table = resolve_ctx.parent_tables[i]->alias_name;
                ExecParamStmt_s exec_param = ExecParamStmt::make_exec_param_stmt(column);
                resolve_ctx.columns_from_parent.push_back(exec_param);
                expr_stmt = exec_param;
            } else {
                ret = SUCCESS;
            }
        }
    }
    MY_ASSERT(find);
    return ret;
}

u32 DMLResolver::resolve_column(TableStmt_s &table, ColumnStmt_s &column)
{
    if (table->is_dual_table()) {
        LOG_ERR("column not in dual table", K(column));
        return ERROR_LEX_STMT;
    } else if (table->is_view_table()) {
        return resolve_column_from_view(table, column);
    } else if (table->is_basic_table()) {
        return resolve_column_from_basic_table(table, column);
    } else {
        LOG_ERR("unknown table type", K(table));
        return ERROR_LEX_STMT;
    }
}

u32 DMLResolver::resolve_column_from_basic_table(BasicTableStmt_s table, ColumnStmt_s &column)
{
    u32 ret = SUCCESS;
    SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
    if (column->is_row_id) {
        column->table_id = table->table_id;
        column->column_id = ROWID_COLUMN_ID;
    } else {
        ColumnDesc col_desc;
        CHECK(checker->get_column_desc(table->ref_table_id,
                                        column->column,
                                        col_desc));
        u32 tid, cid;
        col_desc.get_tid_cid(tid, cid);
        column->table_id = table->table_id;
        column->column_id = cid;
    }
    return ret;
}

u32 DMLResolver::resolve_column_from_view(ViewTableStmt_s table, ColumnStmt_s &column)
{
    u32 ret = SUCCESS;
    SelectStmt_s &query = table->ref_query;
    MY_ASSERT(query);
    bool find = false;
    for (u32 i = 0; !find && i < query->select_expr_list.size(); ++i) {
        if (column->column == query->select_expr_list[i]->alias_name) {
            find = true;
            column->table_id = table->table_id;
            column->column_id = i;
        }
    }
    return ret;
}

u32 DMLResolver::resolve_subquery(SelectStmt_s& query_stmt, ResolveCtx &ctx)
{
    u32 ret = SUCCESS;
    ctx.all_hints = resolve_ctx.all_hints;
    CHECK(resolve_stmt(query_stmt, query_ctx, ctx));
    return ret;
}

bool DMLResolver::find_table_name(const String& table_name)
{
    for (u32 i = 0; i < resolve_ctx.cur_tables.size(); ++i) {
        if (resolve_ctx.cur_tables[i]->alias_name == table_name) {
            return true;
        }
    }
    return false;
}

bool DMLResolver::find_table_item(u32 table_id)
{
    for (u32 i = 0; i < resolve_ctx.cur_tables.size(); ++i) {
        if (resolve_ctx.cur_tables[i]->table_id == table_id) {
            return true;
        }
    }
    return false;
}

u32 DMLResolver::resolve_stmt_hint()
{
    u32 ret = SUCCESS;
    String qb_name = stmt->stmt_hint.generate_qb_name(stmt->stmt_id);
    if (stmt->stmt_hint.has_qb_name()) {
        HintStmt_s qb_name_hint = stmt->stmt_hint.get_hint(HintStmt::QB_NAME);
        qb_name = qb_name_hint->get_qb_name();
    }
    Vector<HintStmt_s> all_hints = stmt->stmt_hint.all_hints;
    stmt->stmt_hint.all_hints.clear();
    //选出当前stmt的hint
    for (u32 i = 0; i < resolve_ctx.all_hints.size(); ++i) {
        if (qb_name == resolve_ctx.all_hints[i]->get_qb_name() ||
            resolve_ctx.all_hints[i]->is_global_hint()) {
            stmt->stmt_hint.add_hint(resolve_ctx.all_hints[i]);
        }
    }
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (!all_hints[i]->has_qb_name() ||
            qb_name == all_hints[i]->get_qb_name() ||
            all_hints[i]->is_global_hint()) {
            stmt->stmt_hint.add_hint(all_hints[i]);
        }
        if ((all_hints[i]->has_qb_name() &&
            qb_name != all_hints[i]->get_qb_name()) ||
            all_hints[i]->is_global_hint()) {
            resolve_ctx.all_hints.push_back(all_hints[i]);
        }
    }
    CHECK(stmt->stmt_hint.formalize());
    return ret;
}