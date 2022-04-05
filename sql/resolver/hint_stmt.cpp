#include "hint_stmt.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Parser;

HintStmt_s HintStmt::make_hint_stmt(HintType type, bool is_enable)
{
    HintStmt_s hint;
    switch (type)
    {
        case NO_REWRITE:
            hint = HintStmt_s(new NoRewriteHintStmt());
            break;
        case UNNEST:
            hint = HintStmt_s(new UnnestHintStmt(is_enable));
            break;
        case JOIN:
            hint = HintStmt_s(new JoinHintStmt(is_enable));
            break;
        case LEADING:
            hint = HintStmt_s(new LeadingHintStmt());
            break;
        case PARALLEL:
            hint = HintStmt_s(new ParallelHintStmt());
            break;
        default:
            ;
    }
    return hint;
}

String HintStmt::print_outline() const
{
    String ret = HintTypeString[type];
    ret += "(@" + qb_name + ")";
    return ret;
}

u32 HintStmt::deep_copy(HintStmt_s &hint) const
{
    u32 ret = SUCCESS;
    hint = make_hint_stmt(type);
    hint->qb_name = qb_name;
    return ret;
}

bool HintStmt::is_base_equal(const HintStmt_s &other) const
{
    return type == other->type && 
           qb_name == other->qb_name;
}

bool HintStmt::is_equal(const HintStmt_s &other) const
{
    return is_base_equal(other) &&
           is_enable_ == other->is_enable_;
}

bool HintStmt::is_excluse(const HintStmt_s &other) const
{
    return is_base_equal(other) &&
           is_enable_ != other->is_enable_;
}

String JoinHintStmt::print_outline() const
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

u32 JoinHintStmt::deep_copy(HintStmt_s &hint) const
{
    u32 ret = SUCCESS;
    CHECK(HintStmt::deep_copy(hint));
    JoinHintStmt_s join_hint = hint;
    join_hint->table_names = table_names;
	join_hint->table_ids = table_ids;
	join_hint->join_algo = join_algo;
    return ret;
}

bool JoinHintStmt::is_base_equal(const HintStmt_s &other) const
{
    bool ret = HintStmt::is_base_equal(other);
    if (ret) {
        JoinHintStmt_s join_hint = other;
        Vector<String> common_table;
        intersect(table_names, join_hint->table_names, common_table);
        return common_table.size() == table_names.size() &&
                common_table.size() == join_hint->table_names.size();
    } else {
        return false;
    }
}

bool JoinHintStmt::is_equal(const HintStmt_s &other) const
{
    bool ret = is_base_equal(other);
    if (ret) {
        JoinHintStmt_s join_hint = other;
        return join_algo == join_hint->join_algo &&
                is_enable_ == join_hint->is_enable_;
    } else {
        return false;
    }
}

bool JoinHintStmt::is_excluse(const HintStmt_s &other) const
{
    bool ret = is_base_equal(other);
    if (ret) {
        JoinHintStmt_s join_hint = other;
        if (!is_enable_ && !join_hint->is_enable_) {
            return false;
        } else {
            return join_algo != join_hint->join_algo ||
                is_enable_ != join_hint->is_enable_;
        }
    } else {
        return false;
    }
}

String LeadingHintStmt::print_outline() const
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


u32 LeadingHintStmt::deep_copy(HintStmt_s &hint) const
{
    u32 ret = SUCCESS;
    CHECK(HintStmt::deep_copy(hint));
    LeadingHintStmt_s leading_hint = hint;
    leading_hint->is_ordered = is_ordered;
    if (tables) {
        CHECK(tables->deep_copy(leading_hint->tables));
    }
    return ret;
}

bool LeadingHintStmt::is_base_equal(const HintStmt_s &other) const
{
    bool ret = HintStmt::is_base_equal(other);
    if (!ret) {
        return ret;
    } else {
        LeadingHintStmt_s leading_hint = other;
        if (is_ordered && leading_hint->is_ordered) {
            return true;
        } else if (!is_ordered && !leading_hint->is_ordered) {
            return tables->is_base_equal(leading_hint->tables);
        } else {
            return false;
        }
    }
}

bool LeadingHintStmt::is_excluse(const HintStmt_s &other) const
{
    return !is_equal(other);
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

u32 LeadingTable::deep_copy(LeadingTable_s &table) const
{
    u32 ret = SUCCESS;
    table = make_leading_table();
    table->table_name = table_name;
	table->table_id = table_id;
	table->is_base_table = is_base_table;
    LeadingTable_s copy_table;
    for (u32 i = 0; i < table_list.size(); ++i) {
        CHECK(table_list[i]->deep_copy(copy_table));
        table->table_list.push_back(copy_table);
    }
    return ret;
}

bool LeadingTable::is_base_equal(const LeadingTable_s &other) const
{
    if (is_base_table && other->is_base_table) {
        return table_name == other->table_name;
    } else if (!is_base_table && !other->is_base_table) {
        if (table_list.size() != other->table_list.size()) {
            return false;
        } else {
            for (u32 i = 0; i < table_list.size(); ++i) {
                if (!table_list[i]->is_base_equal(other->table_list[i])) {
                    return false;
                }
            }
            return true;
        }
    } else {
        return false;
    }
}

String ParallelHintStmt::print_outline() const
{
    String ret = "PARALLEL(";
    ret += std::to_string(parallel) + ")";
    return ret;
}

u32 ParallelHintStmt::deep_copy(HintStmt_s &hint) const
{
    u32 ret = SUCCESS;
    CHECK(HintStmt::deep_copy(hint));
    ParallelHintStmt_s dop_hint = hint;
    dop_hint->parallel = parallel;
    return ret;
}

StmtHintManager_s StmtHintManager::make_stmt_hint_manager()
{
    return StmtHintManager_s(new StmtHintManager);
}

void StmtHintManager::get_all_hints(Vector<HintStmt_s> &all_hints)const
{
    for (auto iter1 = hint_map.cbegin(); iter1 != hint_map.cend(); ++iter1) {
        for (auto iter2 = iter1->second.cbegin(); iter2 != iter1->second.cend(); ++iter2) {
            append(all_hints, iter2->second);
        }
    }
}

u32 StmtHintManager::add_hint(HintStmt_s &hint)
{
    u32 ret = SUCCESS;
    const String &qb_name = hint->get_qb_name();
    auto iter1 = hint_map.find(qb_name);
    if (iter1 == hint_map.cend()) {
        HintArray hints;
        hints.push_back(hint);
        HintTypeMap type_map;
        type_map[hint->get_hint_type()] = hints;
        hint_map[qb_name] = type_map;
        return ret;
    }
    auto iter2 = iter1->second.find(hint->get_hint_type());
    if (iter2 == iter1->second.cend()) {
        HintArray hints;
        hints.push_back(hint);
        iter1->second[hint->get_hint_type()] = hints;
        return ret;
    }
    for (u32 i = 0; i < iter2->second.size(); ++i) {
        if (hint->is_equal(iter2->second[i])) {
            hint->set_invalid(true);
        } else if (hint->is_excluse(iter2->second[i])) {
            hint->set_invalid(true);
            iter2->second[i]->set_invalid(true);
        }
    }
    iter2->second.push_back(hint);
    return ret;
}

void StmtHintManager::get_hint_status(const String &qb_name, HintType type, HintStatus &status)
{
    Vector<HintStmt_s> hints;
    find_hints(qb_name, type, hints);
    if (hints.empty()) {
        status = HintManager::NOT_SET_HINT;
        return;
    }
    HintStmt_s &hint = hints[0];
    if (hint->is_enable()) {
        status = HintManager::FORCE_ENABLE;
    } else {
        status = HintManager::FORCE_DISABLE;
    }
}

void StmtHintManager::find_hints(const String &qb_name, HintType type, Vector<HintStmt_s> &hints)const
{
    auto iter1 = hint_map.find(qb_name);
    if (iter1 == hint_map.cend()) {
        return;
    }
    auto iter2 = iter1->second.find(type);
    if (iter2 == iter1->second.cend()) {
        return;
    }
    for (u32 i = 0; i < iter2->second.size(); ++i) {
        if (!iter2->second[i]->is_invalid()) {
            hints.push_back(iter2->second[i]);
        }
    }
}

u32 StmtHintManager::copy_hints(const String &src_qb_name, const String &dst_qb_name)
{
    u32 ret = SUCCESS;
    auto iter1 = hint_map.find(dst_qb_name);
    if (iter1 != hint_map.end()) {
        return ret;
    }
    iter1 = hint_map.find(src_qb_name);
    if (iter1 != hint_map.end()) {
        return ret;
    }
    HintStmt_s hint;
    for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2) {
        for (u32 i = 0; i < iter2->second.size(); ++i) {
            CHECK(iter2->second[i]->deep_copy(hint));
            hint->set_qb_name(dst_qb_name);
            CHECK(add_hint(hint));
        }
    }
    return ret;
}

OutlineHintManager_s OutlineHintManager::make_outline_hint_manager()
{
    return OutlineHintManager_s(new OutlineHintManager);
}

void OutlineHintManager::get_all_hints(Vector<HintStmt_s> &all_hints)const
{
    append(all_hints, outline_hints);
}

u32 OutlineHintManager::add_hint(HintStmt_s &hint)
{
    u32 ret = SUCCESS;
    if (hint->is_enable()) {
        outline_hints.push_back(hint);
    }
    return ret;
}

void OutlineHintManager::get_hint_status(const String &qb_name, HintType type, HintStatus &status)
{
    if (index < 0 || index >= outline_hints.size()) {
        status = HintManager::FORCE_DISABLE;
        return;
    }
    HintStmt_s &hint = outline_hints[index];
    if (qb_name == hint->get_qb_name() && type == hint->get_hint_type()) {
        status = HintManager::FORCE_ENABLE;
        hint->set_used(true);
        ++index;
    } else {
        status = HintManager::FORCE_DISABLE;
    }
}

void OutlineHintManager::find_hints(const String &qb_name, HintType type, Vector<HintStmt_s> &hints)const
{
    if (index < 0 || index >= outline_hints.size()) {
        return;
    }
    const HintStmt_s &hint = outline_hints[index];
    if (qb_name == hint->get_qb_name() && type == hint->get_hint_type()) {
        hints.push_back(hint);
    }
}

u32 OutlineHintManager::copy_hints(const String &src_qb_name, const String &dst_qb_name)
{
    return OPERATION_NOT_SUPPORT;
}

void QueryHint::reset()
{
    global_hints.reset();
    optimizer_hints.reset();
    transformer_hints.reset();
    generate_hints.reset();
}

u32 QueryHint::init(Vector<HintStmt_s> &all_hints, bool is_outline)
{
    u32 ret = SUCCESS;
    reset();
    if (is_outline) {
		transformer_hints = OutlineHintManager::make_outline_hint_manager();
    } else {
        transformer_hints = StmtHintManager::make_stmt_hint_manager();
    }
    global_hints = StmtHintManager::make_stmt_hint_manager();
	optimizer_hints = StmtHintManager::make_stmt_hint_manager();
	generate_hints = StmtHintManager::make_stmt_hint_manager();
    for (u32 i = 0; i < all_hints.size(); ++i) {
        if (all_hints[i]->is_global_hint()) {
            CHECK(global_hints->add_hint(all_hints[i]));
        } else if (all_hints[i]->is_transform_hint()) {
            CHECK(transformer_hints->add_hint(all_hints[i]));
        } else {
            CHECK(optimizer_hints->add_hint(all_hints[i]));
        }
    }
    return ret;
}

String QueryHint::print_outline() const
{
    String ret;
    ret += "\tBEGIN_OUTLINE_DATA\n";
    Vector<HintStmt_s> all_hints;
    generate_hints->get_all_hints(all_hints);
    for (u32 i = 0; i < all_hints.size(); ++i) {
        ret += "\t" + all_hints[i]->print_outline() + "\n";
    }
    ret += "\tEND_OUTLINE_DATA\n";
    return ret;
}

bool QueryHint::enable_no_rewrite(const String &qb_name) const
{
    HintManager::HintStatus status = HintManager::NOT_SET_HINT;
    transformer_hints->get_hint_status(qb_name, NO_REWRITE, status);
    return HintManager::FORCE_DISABLE == status;
}

bool QueryHint::enable_unnest(const String &qb_name) const
{
    HintManager::HintStatus status = HintManager::NOT_SET_HINT;
    transformer_hints->get_hint_status(qb_name, UNNEST, status);
    return HintManager::FORCE_ENABLE == status;
}

bool QueryHint::enable_no_unnest(const String &qb_name) const
{
    HintManager::HintStatus status = HintManager::NOT_SET_HINT;
    transformer_hints->get_hint_status(qb_name, UNNEST, status);
    return HintManager::FORCE_DISABLE == status;
}

void QueryHint::get_join_hints(const String &qb_name, Vector<JoinHintStmt_s> &join_hints)
{
    Vector<HintStmt_s> hints;
    optimizer_hints->find_hints(qb_name, JOIN, hints);
    append(join_hints, hints);
}

LeadingHintStmt_s QueryHint::get_leading_hint(const String &qb_name)
{
    Vector<HintStmt_s> hints;
    LeadingHintStmt_s hint;
    optimizer_hints->find_hints(qb_name, LEADING, hints);
    if (!hints.empty()) {
        hint = hints[0];
    }
    return hint;
}

bool QueryHint::has_leading_hint(const String &qb_name)
{
    HintManager::HintStatus status = HintManager::NOT_SET_HINT;
    optimizer_hints->get_hint_status(qb_name, LEADING, status);
    return HintManager::FORCE_ENABLE == status;
}

u32 QueryHint::generate_transform_outline(const String &qb_name, HintType type)
{
    u32 ret = SUCCESS;
    HintStmt_s hint = HintStmt::make_hint_stmt(type);
    hint->set_qb_name(qb_name);
    CHECK(generate_hints->add_hint(hint));
    return ret;
}
u32 QueryHint::generate_join_outline(const String &qb_name, const Vector<String> &table_names, JoinAlgo join_algo)
{
    u32 ret = SUCCESS;
    HintStmt_s hint = HintStmt::make_hint_stmt(JOIN);
    hint->set_qb_name(qb_name);
    JoinHintStmt_s join_hint = hint;
    join_hint->table_names = table_names;
    join_hint->join_algo = join_algo;
    CHECK(generate_hints->add_hint(hint));
    return ret;
}

u32 QueryHint::generate_leading_outline(const String &qb_name, const LeadingTable_s &tables)
{
    u32 ret = SUCCESS;
    HintStmt_s hint = HintStmt::make_hint_stmt(LEADING);
    hint->set_qb_name(qb_name);
    LeadingHintStmt_s leading_hint = hint;
    leading_hint->tables = tables;
    CHECK(generate_hints->add_hint(hint));
    return ret;
}

String Hint::generate_qb_name(u32 stmt_id)
{
    qb_name = String("SEL") + std::to_string(stmt_id);
    return qb_name;
}