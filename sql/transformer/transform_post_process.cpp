#include "transform_post_process.h"
#include "query_ctx.h"
#include "hint_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformPostProcess::TransformPostProcess()
    :TransformRule(POST_ORDER)
{

}

TransformPostProcess::~TransformPostProcess()
{

}

bool TransformPostProcess::need_rewrite(DMLStmt_s stmt) const
{
    return true;
}

u32 TransformPostProcess::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    bool happened = false;
    CHECK(update_table_hint(stmt, happened));
    if (happened) {
        set_transform_happened();
    }
    LOG_TRACE("succeed to update table hint", K(happened));
    return ret;
}

u32 TransformPostProcess::update_table_hint(DMLStmt_s &stmt, bool &happened)
{
    u32 ret = SUCCESS;
    CHECK(update_join_hint(stmt, happened));
    CHECK(update_leading_table_hint(stmt, happened));
    return ret;
}

u32 TransformPostProcess::update_join_hint(DMLStmt_s &stmt, bool &happened)
{
    u32 ret = SUCCESS;
    u32 id = INVALID_ID;
    Vector<JoinHintStmt_s> join_hints;
    ctx->query_ctx->query_hint.get_join_hints(stmt->get_qb_name(), join_hints);
    for (u32 i = 0; i < join_hints.size(); ++i) {
        for (u32 j = 0; j < join_hints[i]->table_names.size(); ++j) {
            if (stmt->find_table_id(join_hints[i]->table_names[j], id)) {
                join_hints[i]->table_ids.add_member(id);
            } else {
                join_hints[i]->set_invalid(true);
                break;
            }
        }
        happened = true;
    }
    return ret;
}

u32 TransformPostProcess::update_leading_table_hint(DMLStmt_s &stmt, bool &happened)
{
    u32 ret = SUCCESS;
    QueryHint &query_hint = ctx->query_ctx->query_hint;
    if (!query_hint.has_leading_hint(stmt->get_qb_name())) {
        return ret;
    }
    LeadingHintStmt_s leading_hint = query_hint.get_leading_hint(stmt->get_qb_name());
    if (leading_hint->is_ordered) {
        CHECK(change_ordered_to_leading(stmt, leading_hint));
    } else {
        if(!inner_update_leading_table_hint(stmt, leading_hint->tables)) {
            leading_hint->set_invalid(true);   
        }
    }
    happened = true;
    return ret;
}

bool TransformPostProcess::inner_update_leading_table_hint(DMLStmt_s &stmt, LeadingTable_s &leading_hint)
{
    bool ret = true;
    if (leading_hint->is_base_table) {
        ret = stmt->find_table_id(leading_hint->table_name, 
                                  leading_hint->table_id);
    } else {
        for (u32 i = 0; ret && i < leading_hint->table_list.size(); ++i) {
            ret = inner_update_leading_table_hint(stmt, leading_hint->table_list[i]);
        }
    }
    return ret;
}

u32 TransformPostProcess::change_ordered_to_leading(DMLStmt_s &stmt, LeadingHintStmt_s &leading_hint)
{
    u32 ret = SUCCESS;
    LeadingTable_s table = LeadingTable::make_leading_table();
    table->is_base_table = false;
    for (u32 i = 0; i < stmt->from_stmts.size(); ++i) {
        LeadingTable_s leading_table;
        CHECK(change_table_item_to_leading(stmt->from_stmts[i], leading_table));
        table->table_list.push_back(leading_table);
    }
    leading_hint->is_ordered = false;
    leading_hint->tables = table;
    return ret;
}

u32 TransformPostProcess::change_table_item_to_leading(TableStmt_s &table_item, LeadingTable_s &leading_table)
{
    u32 ret = SUCCESS;
    if (table_item->is_joined_table()) {
        leading_table = LeadingTable::make_leading_table();
        leading_table->is_base_table = false;
        LeadingTable_s left_table, right_table;
        JoinedTableStmt_s joined_table = table_item;
        CHECK(change_table_item_to_leading(joined_table->left_table, left_table));
        CHECK(change_table_item_to_leading(joined_table->right_table, right_table));
        leading_table->table_list.push_back(left_table);
        leading_table->table_list.push_back(right_table);
    } else {
        leading_table = LeadingTable::make_leading_table();
        leading_table->is_base_table = true;
        leading_table->table_name = table_item->alias_name;
        leading_table->table_id = table_item->table_id;
    }
    return ret;
}
