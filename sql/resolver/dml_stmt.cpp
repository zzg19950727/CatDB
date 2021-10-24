#include "dml_stmt.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "select_stmt.h"

using namespace CatDB::Parser;

HintStmt_s HintStmt::make_hint_stmt(HintType type)
{
    HintStmt_s hint;
    switch (type)
    {
        case QB_NAME:
        case NO_REWRITE:
            hint = HintStmt_s(new HintStmt());
            break;
        case JOIN:
            hint = HintStmt_s(new JoinHintStmt());
            break;
        case LEADING:
            hint = HintStmt_s(new LeadingHintStmt());
            break;
    }
    if (hint) {
        hint->type = type;
    }
    return hint;
}

String HintStmt::get_type_name() const
{
    switch (type)
    {
        case QB_NAME:
            return "QB_NAME";
        case NO_REWRITE:
            return "NO_REWRITE";
        case JOIN:
            return "JOIN";
        case LEADING:
            return "LEADING";
    }
    return "UNKNOWN";
}

String HintStmt::print_outline()
{
    String ret = "";
    if (QB_NAME == type) {
        ret = "QB_NAME(" + qb_name + ")";
    } else if (NO_REWRITE == type) {
        ret = "NO_REWRITE(@" + qb_name + ")";
    }
    return ret;
}

String JoinHintStmt::print_outline()
{
    String ret;
    if (join_algo == HASH_JOIN) {
        ret = "USE_HASH";
    } else {
        ret = "USE_NL";
    }
    ret += "(@" + qb_name + " ";
    for (u32 i = 0; i < table_names.size(); ++i) {
        if (i > 0) {
            ret += ", ";
        }
        ret += table_names[i];
    }
    ret += ")";
    return ret;
}

String LeadingHintStmt::print_outline()
{
    String ret = "LEADING";
    ret += "(@" + qb_name;
    if (tables) {
        String list = tables->to_kv_string();
        list[0] = ' ';
        ret += list;
    } else {
        ret += ")";
    }
    return ret;
}

LeadingTable_s LeadingTable::make_leading_table()
{
    return LeadingTable_s(new LeadingTable());
}

String LeadingTable::to_kv_string()const
{
    String ret;
    if (is_base_table) {
        ret = table_name;
    } else {
        ret = "(";
        for (u32 i = 0; i < table_list.size(); ++i) {
            if (i > 0) {
                ret += ", ";
            }
            ret += table_list[i]->to_kv_string();
        }
        ret += ")";
    }
    return ret;
}

bool Hint::has_qb_name() const
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::QB_NAME == all_hints[i]->get_hint_type()) {
            return true;
        }
    }
    return false;
}

String Hint::generate_qb_name(u32 stmt_id)
{
    String qb_name = "SEL" + std::to_string(stmt_id);
    HintStmt_s hint = HintStmt::make_hint_stmt(HintStmt::QB_NAME);
    hint->set_qb_name(qb_name);
    all_hints.push_back(hint);
    return qb_name;
}

String Hint::get_qb_name() const
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::QB_NAME == all_hints[i]->get_hint_type()) {
            return all_hints[i]->get_qb_name();
        }
    }
    return "";
}

String Hint::print_outline()
{
    String ret;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::QB_NAME != all_hints[i]->get_hint_type()) {
            ret += "\t" + all_hints[i]->print_outline() + "\n";
        }
    }
    return ret;
}

bool Hint::enable_no_rewrite() const
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::NO_REWRITE == all_hints[i]->get_hint_type()) {
            return true;
        }
    }
    return false;
}

void Hint::get_join_hints(Vector<JoinHintStmt_s> &join_hints)
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::JOIN == all_hints[i]->get_hint_type()) {
            join_hints.push_back(all_hints[i]);
        }
    }
}

bool Hint::has_leading_hint()
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::LEADING == all_hints[i]->get_hint_type()) {
            return true;
        }
    }
    return false;
}

LeadingHintStmt_s Hint::get_leading_hint()
{
    LeadingHintStmt_s hint;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::LEADING == all_hints[i]->get_hint_type()) {
            hint = all_hints[i];
            return hint;
        }
    }
}

DMLStmt::DMLStmt()
    :is_explain(false)
{

}

DMLStmt::~DMLStmt()
{

}

u32 DMLStmt::formalize()
{
    u32 ret = SUCCESS;
    table_ids.clear();
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->formalize());
        table_ids.add_members(from_stmts[i]->table_ids);
    }
    for (u32 i = 0; i < where_stmt.size(); ++i) {
        CHECK(where_stmt[i]->formalize());
    }
    collect_special_exprs();
    return ret;
}

u32 DMLStmt::get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
    u32 ret = SUCCESS;
    CHECK(inner_get_stmt_exprs(exprs));
    return ret;
}

u32 DMLStmt::inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs)
{
    u32 ret = SUCCESS;
    append(exprs, where_stmt);
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        CHECK(from_stmts[i]->get_table_exprs(exprs));
    }
    return ret;
}

u32 DMLStmt::collect_special_exprs()
{
    u32 ret = SUCCESS;
    column_exprs.clear();
    aggr_exprs.clear();
    subquery_exprs.clear();
    Vector<ExprStmt_s> all_exprs;
    CHECK(get_stmt_exprs(all_exprs));
    for (u32 i = 0; i < all_exprs.size(); ++i) {
        CHECK(collect_special_exprs(all_exprs[i]));
    }
    return ret;
}

u32 DMLStmt::collect_special_exprs(ExprStmt_s& expr)
{
    u32 ret = SUCCESS;
    MY_ASSERT(expr);
    if (ExprStmt::Column == expr->expr_type()) {
        column_exprs.push_back(expr);
    } else if (ExprStmt::Aggregate == expr->expr_type()) {
        aggr_exprs.push_back(expr);
    } else if (ExprStmt::SubQuery == expr->expr_type()) {
        subquery_exprs.push_back(expr);
    }
    for (u32 i = 0; i < expr->get_params().size(); ++i) {
        CHECK(collect_special_exprs(expr->get_params()[i]));
    }
    return ret;
}

u32 DMLStmt::get_table_items(Vector<TableStmt_s> &tables)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        if (!from_stmts[i]->is_joined_table()) {
            tables.push_back(from_stmts[i]);
        } else {
            JoinedTableStmt_s joined_table = from_stmts[i];
            CHECK(joined_table->get_table_items(tables));
        }
    }
    return ret;
}

u32 DMLStmt::get_column_exprs(u32 table_id, Vector<ExprStmt_s> &columns)
{
    u32 ret = SUCCESS;
    for (u32 i = 0; i < column_exprs.size(); ++i) {
        if (column_exprs[i]->table_ids.has_member(table_id)) {
			if (!ExprUtils::find_equal_expr(columns, column_exprs[i])) {
            	columns.push_back(column_exprs[i]);
			}
        }
    }
    return ret;
}

u32 DMLStmt::get_child_stmts(Vector<SelectStmt_s> &child_stms)
{
    u32 ret = SUCCESS;
    if (Stmt::SetOperation == stmt_type()) {
        SetStmt *set_stmt = dynamic_cast<SetStmt*>(this);
        child_stms.push_back(set_stmt->left_query);
        child_stms.push_back(set_stmt->right_query);
    }
    Vector<TableStmt_s> table_items;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        TableStmt_s table = from_stmts[i];
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
            child_stms.push_back(view_table->ref_query);
        }
    }
    Vector<SubQueryStmt_s> &subquery_exprs = get_subquery_exprs();
    for (u32 i = 0; i < subquery_exprs.size(); ++i) {
        child_stms.push_back(subquery_exprs[i]->query_stmt);
    }
    return ret;
}


u32 DMLStmt::update_table_hint()
{
    u32 ret = SUCCESS;
    Vector<HintStmt_s> new_hints;
    for (u32 i = 0; i < stmt_hint.all_hints.size(); ++i) {
        if (HintStmt::LEADING == stmt_hint.all_hints[i]->get_hint_type()) {
            LeadingHintStmt_s leading_hint = stmt_hint.all_hints[i];
            bool res = true;
            if (!leading_hint->is_ordered) {
                res = update_leading_table_hint(leading_hint->tables);
            } else {
                change_ordered_to_leading(leading_hint);
            }
            if (res) {
                new_hints.push_back(stmt_hint.all_hints[i]);
            }
        } else if (HintStmt::JOIN == stmt_hint.all_hints[i]->get_hint_type()) {
            if (update_join_hint(stmt_hint.all_hints[i])) {
                new_hints.push_back(stmt_hint.all_hints[i]);
            }
        } else {
            new_hints.push_back(stmt_hint.all_hints[i]);
        }
    }
    stmt_hint.all_hints = new_hints;
    return ret;
}

bool DMLStmt::update_join_hint(JoinHintStmt_s join_hint)
{
    bool ret = true;
    u32 id = INVALID_ID;
    for (u32 i = 0; i < join_hint->table_names.size(); ++i) {
        ret = find_table_id(join_hint->table_names[i], id);
        if (!ret) {
            return ret;
        }
        join_hint->table_ids.add_member(id);
    }
    return ret;
}

void DMLStmt::change_ordered_to_leading(LeadingHintStmt_s &leading_hint)
{
    LeadingTable_s table = LeadingTable::make_leading_table();
    table->is_base_table = false;
    for (u32 i = 0; i < from_stmts.size(); ++i) {
        LeadingTable_s leading_table;
        change_table_item_to_leading(from_stmts[i], leading_table);
        table->table_list.push_back(leading_table);
    }
    leading_hint->is_ordered = false;
    leading_hint->tables = table;
}

void DMLStmt::change_table_item_to_leading(TableStmt_s &table_item, LeadingTable_s &leading_table)
{
    if (table_item->is_joined_table()) {
        leading_table = LeadingTable::make_leading_table();
        leading_table->is_base_table = false;
        LeadingTable_s left_table, right_table;
        JoinedTableStmt_s joined_table = table_item;
        change_table_item_to_leading(joined_table->left_table, left_table);
        change_table_item_to_leading(joined_table->right_table, right_table);
        leading_table->table_list.push_back(left_table);
        leading_table->table_list.push_back(right_table);
    } else {
        leading_table = LeadingTable::make_leading_table();
        leading_table->is_base_table = true;
        leading_table->table_name = table_item->alias_name;
        leading_table->table_id = table_item->table_id;
    }
}

bool DMLStmt::update_leading_table_hint(LeadingTable_s &leading_hint)
{
    bool ret = true;
    if (leading_hint->is_base_table) {
        ret = find_table_id(leading_hint->table_name, 
                            leading_hint->table_id);
    } else {
        for (u32 i = 0; i < leading_hint->table_list.size(); ++i) {
            ret = update_leading_table_hint(leading_hint->table_list[i]);
            if (!ret) {
                break;
            }
        }
    }
    return ret;
}

bool DMLStmt::find_table_id(const String &table_name, u32 &table_id)
{
    bool find = false;
    Vector<TableStmt_s> table_items;
    get_table_items(table_items);
    for (u32 i = 0; i < table_items.size(); ++i) {
        if (table_items[i]->alias_name == table_name) {
            if (find) {
                return false;
            } else {
                table_id = table_items[i]->table_id;
                find = true;
            }
        }
    }
    return find;
}