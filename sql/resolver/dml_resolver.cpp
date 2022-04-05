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
#include "obj_number.h"
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
        case Select:
        {
            SelectStmt_s select_stmt = stmt;
            SelectResolver resolver(select_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Update:
        {
            UpdateStmt_s update_stmt = stmt;
            UpdateResolver resolver(update_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Delete:
        {
            DeleteStmt_s delete_stmt = stmt;
            DeleteResolver resolver(delete_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case Insert:
        {
            InsertStmt_s insert_stmt = stmt;
            InsertResolver resolver(insert_stmt, query_ctx, resolve_ctx);
            CHECK(resolver.resolve_stmt());
            break;
        }
        case SetOperation:
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
            ExecParamHelper& helper = temp_ctx.columns_from_parent[i];
            ExprStmt_s& ref_expr = helper.ref_expr;
            if (COLUMN == ref_expr->expr_type()) {
                ColumnStmt_s col = ref_expr;
                if (find_table_item(col->table_id)) {
                    query->add_related_exprs(ref_expr, helper.exec_param);
                } else {
                    resolve_ctx.columns_from_parent.push_back(helper);
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
                    ExprStmt_s aggr_expr = ConstStmt::make_const_stmt(Number::make_object((longlong)1));
                    agg_stmt->set_aggr_expr(aggr_expr);
                    ret = SUCCESS;
                } else {
                    ret = HAVE_ALL_COLUMN_STMT;
                }
            }
            if (SUCC(ret) && AVG == agg_stmt->aggr_func) {
                AggrStmt_s sum_expr = AggrStmt::make_aggr_stmt();
                sum_expr->set_aggr_expr(aggr_expr);
                sum_expr->distinct = agg_stmt->distinct;
                sum_expr->aggr_func = SUM;
                AggrStmt_s count_expr = AggrStmt::make_aggr_stmt();
                count_expr->set_aggr_expr(aggr_expr);
                count_expr->distinct = agg_stmt->distinct;
                count_expr->aggr_func = COUNT;
                OpExprStmt_s op_expr = OpExprStmt::make_op_expr_stmt(OP_DIV);
                op_expr->params.push_back(sum_expr);
                op_expr->params.push_back(count_expr);
                op_expr->alias_name = expr_stmt->alias_name;
                expr_stmt = op_expr;
                aggr_funcs.push_back(sum_expr);
                aggr_funcs.push_back(count_expr);
            } else {
                aggr_funcs.push_back(agg_stmt);
            }
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
                    u32 param_index = query_ctx->generate_param_index();
                    ExecParamStmt_s exec_param = ExecParamStmt::make_exec_param_stmt(param_index);
                    exec_param->res_type = column->res_type;
                    ExecParamHelper helper;
                    helper.exec_param = exec_param;
                    helper.ref_expr = column;
                    resolve_ctx.columns_from_parent.push_back(helper);
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
                u32 param_index = query_ctx->generate_param_index();
                ExecParamStmt_s exec_param = ExecParamStmt::make_exec_param_stmt(param_index);
                ExecParamHelper helper;
                helper.exec_param = exec_param;
                helper.ref_expr = column;
                resolve_ctx.columns_from_parent.push_back(helper);
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
    u32 ret = SUCCESS;
    if (table->is_dual_table()) {
        LOG_ERR("column not in dual table", K(column));
        ret = ERROR_LEX_STMT;
    } else if (table->is_view_table()) {
        if (FAIL(resolve_column_from_view(table, column))) {
            String err_msg = "column " + column->table + "." + column->column + " not exists";
            query_ctx->set_error_msg(err_msg);
        }
    } else if (table->is_basic_table()) {
        if (FAIL(resolve_column_from_basic_table(table, column))) {
            String err_msg = "column " + column->table + "." + column->column + " not exists";
            query_ctx->set_error_msg(err_msg);
        }
    } else {
        LOG_ERR("unknown table type", K(table));
        ret = ERROR_LEX_STMT;
    }
    return ret;
}

u32 DMLResolver::resolve_column_from_basic_table(BasicTableStmt_s table, ColumnStmt_s &column)
{
    u32 ret = SUCCESS;
    SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
    if (column->is_row_id) {
        column->table_id = table->table_id;
        column->column_id = ROWID_COLUMN_ID;
        column->res_type = DataType::default_int_type();
    } else {
        ColumnDesc col_desc;
        CHECK(checker->get_column_desc(table->ref_table_id,
                                        column->column,
                                        col_desc));
        u32 cid = col_desc.get_cid();
        column->table_id = table->table_id;
        column->column_id = cid;
        column->res_type = col_desc.get_data_type();
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
            CHECK(query->select_expr_list[i]->formalize());
            find = true;
            column->table_id = table->table_id;
            column->column_id = i;
            column->res_type = query->select_expr_list[i]->res_type;
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
    Vector<HintStmt_s> &all_hints = stmt->stmt_hint.all_hints;
    if (resolve_ctx.has_outline) {
        all_hints.clear();
        return ret;
    }
    if (stmt->stmt_hint.is_outline) {
        resolve_ctx.all_hints.clear();
        resolve_ctx.has_outline = true;
    }
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (!all_hints[i]->has_qb_name() &&
            !all_hints[i]->is_global_hint()) {
            all_hints[i]->set_qb_name(qb_name);
        }
        resolve_ctx.all_hints.push_back(all_hints[i]);
    }
    all_hints.clear();
    return ret;
}