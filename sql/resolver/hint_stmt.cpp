#include "hint_stmt.h"
#include "error.h"
#include "log.h"
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
        case PARALLEL:
            hint = HintStmt_s(new ParallelHintStmt());
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
        case PARALLEL:
            return "PARALLEL";
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

u32 HintStmt::deep_copy(HintStmt_s &hint, u32 flag) const
{
    u32 ret = SUCCESS;
    hint = make_hint_stmt(type);
    hint->qb_name = qb_name;
	hint->is_single = is_single;
	hint->is_global = is_global;
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

u32 JoinHintStmt::deep_copy(HintStmt_s &hint, u32 flag) const
{
    u32 ret = SUCCESS;
    hint = make_hint_stmt(type);
    hint->qb_name = qb_name;
	hint->is_single = is_single;
	hint->is_global = is_global;
    JoinHintStmt_s join_hint = hint;
    join_hint->table_names = table_names;
	join_hint->table_ids = table_ids;
	join_hint->join_algo = join_algo;
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


u32 LeadingHintStmt::deep_copy(HintStmt_s &hint, u32 flag) const
{
    u32 ret = SUCCESS;
    hint = make_hint_stmt(type);
    hint->qb_name = qb_name;
	hint->is_single = is_single;
	hint->is_global = is_global;
    LeadingHintStmt_s leading_hint = hint;
    leading_hint->is_ordered = is_ordered;
    if (tables) {
        CHECK(tables->deep_copy(leading_hint->tables, flag));
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

u32 LeadingTable::deep_copy(LeadingTable_s &table, u32 flag) const
{
    u32 ret = SUCCESS;
    table = make_leading_table();
    table->table_name = table_name;
	table->table_id = table_id;
	table->is_base_table = is_base_table;
    LeadingTable_s copy_table;
    for (u32 i = 0; i < table_list.size(); ++i) {
        CHECK(table_list[i]->deep_copy(copy_table, flag));
        table->table_list.push_back(copy_table);
    }
    return ret;
}

String ParallelHintStmt::print_outline()
{
    String ret = "PARALLEL(";
    ret += std::to_string(parallel) + ")";
    return ret;
}

u32 ParallelHintStmt::deep_copy(HintStmt_s &hint, u32 flag) const
{
    u32 ret = SUCCESS;
    hint = make_hint_stmt(type);
    hint->qb_name = qb_name;
	hint->is_single = is_single;
	hint->is_global = is_global;
    ParallelHintStmt_s dop_hint = hint;
    dop_hint->parallel = parallel;
    return ret;
}

bool Hint::has_qb_name() const
{
    return has_hint(HintStmt::QB_NAME);
}

String Hint::generate_qb_name(u32 stmt_id)
{
    qb_name = "SEL" + std::to_string(stmt_id);
    return qb_name;
}

String Hint::print_outline(bool &print_global_hint)
{
    String ret;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (!print_global_hint && all_hints[i]->is_global_hint()) {
            //do not print global hint
        } else {
            ret += "\t" + all_hints[i]->print_outline() + "\n";
        }
    }
    return ret;
}

bool Hint::enable_no_rewrite() const
{
    return has_hint(HintStmt::NO_REWRITE);
}

void Hint::get_join_hints(Vector<JoinHintStmt_s> &join_hints)
{
    Vector<HintStmt_s> hints;
    get_hints(HintStmt::JOIN, hints);
    append(join_hints, hints);
}

bool Hint::has_leading_hint()
{
    return has_hint(HintStmt::LEADING);
}

LeadingHintStmt_s Hint::get_leading_hint()
{
    return get_hint(HintStmt::LEADING);
}

void Hint::add_hint(HintStmt_s hint)
{
    if (HintStmt::QB_NAME != hint->get_hint_type()) {
        hint->set_qb_name(qb_name);
        all_hints.push_back(hint);
    }
}

bool Hint::has_hint(HintStmt::HintType type) const
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (type == all_hints[i]->get_hint_type()) {
            return true;
        }
    }
    return false;
}

void Hint::remove_hint(HintStmt::HintType type)
{
    Vector<HintStmt_s> new_hints;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (type != all_hints[i]->get_hint_type()) {
            new_hints.push_back(all_hints[i]);
        }
    }
    all_hints = new_hints;
}

HintStmt_s Hint::get_hint(HintStmt::HintType type) const
{
    HintStmt_s hint;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (type == all_hints[i]->get_hint_type()) {
            hint = all_hints[i];
            break;
        }
    }
    return hint;
}

void Hint::get_hints(HintStmt::HintType type, Vector<HintStmt_s> &hints) const
{
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (type == all_hints[i]->get_hint_type()) {
            hints.push_back(all_hints[i]);
        }
    }
}


u32 Hint::deep_copy(Hint& hint, u32 flag)const
{
    u32 ret = SUCCESS;
    HintStmt_s copy_hint;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (HintStmt::JOIN == all_hints[i]->get_hint_type() ||
            HintStmt::LEADING == all_hints[i]->get_hint_type()) {
            CHECK(all_hints[i]->deep_copy(copy_hint, flag));
            hint.all_hints.push_back(copy_hint);
        } else {
            hint.all_hints.push_back(all_hints[i]);
        }
    }
    hint.qb_name = qb_name;
    return ret;
}

u32 Hint::formalize()
{
    u32 ret = SUCCESS;
    BitSet hint_map;
    Vector<HintStmt_s> new_hints;
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (!all_hints[i]->is_single_hint()) {
            new_hints.push_back(all_hints[i]);
        } else if (hint_map.has_member( u32(all_hints[i]->get_hint_type()) )) {
            //重复hint
        } else {
            new_hints.push_back(all_hints[i]);
            hint_map.add_member( u32(all_hints[i]->get_hint_type()) );
        }
    }
    return ret;
}

u32 Hint::reset_qb_name(u32 stmt_id)
{
    u32 ret = SUCCESS;
    String qb_name = generate_qb_name(stmt_id);
    for (u32 i = 0; i < all_hints.size(); ++i) {
        all_hints[i]->set_qb_name(qb_name);
    }
    return ret;
}
