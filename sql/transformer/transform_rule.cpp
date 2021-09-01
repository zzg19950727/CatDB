#include "transform_rule.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "dml_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformRule::TransformRule(bool pre_order)
    :pre_order(pre_order),
    happened(false)
{

}

TransformRule::~TransformRule()
{

}

u32 TransformRule::transform(DMLStmt_s stmt)
{
    u32 ret = SUCCESS;
    if (pre_order) {
        CHECK(transform_pre_order(stmt));
    } else {
        CHECK(transform_post_order(stmt));
    }
    return ret;
}

u32 TransformRule::transform_one_stmt(DMLStmt_s &stmt)
{
    return SUCCESS;
}

u32 TransformRule::transform_pre_order(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    CHECK(transform_self(stmt));
    CHECK(transform_chidren(stmt));
    return ret;
}

u32 TransformRule::transform_post_order(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    CHECK(transform_chidren(stmt));
    CHECK(transform_self(stmt));
    return ret;
}

u32 TransformRule::transform_self(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    CHECK(transform_one_stmt(stmt));
    CHECK(stmt->formalize());
    return ret;
}

u32 TransformRule::transform_chidren(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    if (Stmt::SetOperation == stmt->stmt_type()) {
        SetStmt_s set_stmt = stmt;
        CHECK(transform(set_stmt->left_query));
        CHECK(transform(set_stmt->right_query));
    }
    Vector<TableStmt_s> table_items;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        TableStmt_s table = stmt->from_stmts[i];
        if (table->is_joined_table()) {
            JoinedTableStmt_s joined_table = table;
            CHECK(joined_table->get_table_items(table_items));
        } else if (table->is_view_table()) {
            table_items.push_back(table);
        }
    }
    for (u32 i = 0; i < table_items.size(); ++i) {
        if (table_items[i]->is_view_table()) {
            ViewTableStmt_s view_table = table_items[i];
            CHECK(transform(view_table->ref_query));
        }
    }
    Vector<SubQueryStmt_s> &subquery_exprs = stmt->get_subquery_exprs();
    for (u32 i = 0; i < subquery_exprs.size(); ++i) {
        CHECK(transform(subquery_exprs[i]->query_stmt));
    }
    return ret;
}