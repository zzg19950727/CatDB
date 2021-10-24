#include "insert_resolver.h"
#include "schema_checker.h"
#include "select_stmt.h"
#include "insert_stmt.h"
#include "expr_stmt.h"
#include "error.h"
using namespace CatDB::Parser;
using namespace CatDB::Common;
using namespace CatDB::Sql;

InsertResolver::InsertResolver(InsertStmt_s &stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx)
    :DMLResolver(stmt, query_ctx, resolve_ctx),
    insert_stmt(stmt)
{
    
}

InsertResolver::~InsertResolver()
{

}

u32 InsertResolver::check_insert_value(BasicTableStmt_s &insert_table, Vector<ExprStmt_s> &value_list)
{
    u32 ret = SUCCESS;
    SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	RowDesc row_desc;
	CHECK(checker->get_row_desc(insert_table->ref_table_id, row_desc));
	for (u32 i = 0; i < value_list.size(); ++i) {
        CHECK(value_list[i]->formalize());
        MY_ASSERT(ExprStmt::List == value_list[i]->expr_type());
		CHECK(resolve_row(value_list[i]->params, row_desc));
	}
	return ret;
}

u32 InsertResolver::check_insert_value(BasicTableStmt_s &insert_table, SelectStmt_s &select_value)
{
    u32 ret = SUCCESS;
    SchemaChecker_s checker = SchemaChecker::make_schema_checker();
	MY_ASSERT(checker);
	RowDesc row_desc;
	CHECK(checker->get_row_desc(insert_table->ref_table_id, row_desc));
    CHECK(select_value->formalize());
    CHECK(resolve_row(select_value->select_expr_list, row_desc));
	return ret;
}

u32 InsertResolver::resolve_row(Vector<ExprStmt_s>& list, const RowDesc& row_desc)
{
    u32 ret = SUCCESS;
    ColumnDesc col_desc;
    u32 column_id = INVALID_ID;
    u32 table_id = INVALID_ID;
    MY_ASSERT(row_desc.get_column_num() == list.size());
	for (u32 i = 0; i < list.size(); ++i) {
		CHECK(row_desc.get_column_desc(i, col_desc));
        col_desc.get_tid_cid(table_id, column_id);
        MY_ASSERT(column_id >= 0, column_id < list.size());
        ExprStmt_s new_value = OpExprStmt::make_op_expr_stmt(OP_CAST);
        Object_s const_value = Object::make_null_object();
        CHECK(cast_to(col_desc.get_data_type(), const_value));
        ExprStmt_s const_stmt = ConstStmt::make_const_stmt(const_value);
		new_value->add_param(list[column_id]);
        new_value->add_param(const_stmt);
        list[column_id] = new_value;
	}
	return SUCCESS;
}

u32 InsertResolver::resolve_stmt()
{
    u32 ret = SUCCESS;
    CHECK(resolve_stmt_hint());
    MY_ASSERT(insert_stmt->table);
    MY_ASSERT(insert_stmt->table->is_basic_table());
    BasicTableStmt_s &insert_table_item = insert_stmt->table;
    CHECK(resolve_basic_table_item(insert_table_item));
    MY_ASSERT(!insert_table_item->is_dual_table());
    if (insert_stmt->value_list.size() > 0) {
        CHECK(resolve_exprs(insert_stmt->value_list, resolve_ctx));
        CHECK(check_insert_value(insert_table_item, insert_stmt->value_list));
    } else if (insert_stmt->query_values && insert_stmt->query_values->is_select_stmt()) {
        CHECK(DMLResolver::resolve_stmt(insert_stmt->query_values, query_ctx, resolve_ctx));
        CHECK(check_insert_value(insert_table_item, insert_stmt->query_values));
    } else {
        ret = ERROR_LEX_STMT;
    }
    return ret;
}