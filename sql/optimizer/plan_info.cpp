#include "plan_info.h"

using namespace CatDB::Optimizer;
#include <iostream>

String n_string(const String& str, u32 n)
{
    String ret;
    for (u32 i = 0; i < n; ++i) {
        ret += str;
    }
    return ret;
}

void uppercase(String &str)
{
    for (u32 i = 0; i < str.length(); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = 'A' + str[i] - 'a';
        }
    }
}

void PlanInfo::formalize_plan_info(Vector<PlanInfo> &plan_infos)
{
    u32 id_max_len = 2;
    u32 op_max_len = 8;
    u32 name_max_len = 4;
    u32 rows_max_len = 9;
    u32 cost_max_len = 4;
    PlanInfo title;
    title.depth = 0;
    title.id = "ID";
    title.op = "OPERATOR";
    title.name = "NAME";
    title.rows = "ROW COUNT";
    title.cost = "COST";
    Vector<PlanInfo> temp = plan_infos;
    plan_infos.clear();
    plan_infos.push_back(title);
    for (u32 i = 0; i < temp.size(); ++i) {
        PlanInfo &info = temp[i];
        info.id = std::to_string(i);
        info.op = n_string(" ", info.depth) + info.op;
        if (!info.expr_info.empty()) {
            info.expr_info = info.id + " - " + info.expr_info;
        }
        if (info.id.length() > id_max_len) {
            id_max_len = info.id.length();
        }
        if (info.op.length() > op_max_len) {
            op_max_len = info.op.length();
        }
        if (info.name.length() > name_max_len) {
            name_max_len = info.name.length();
        }
        if (info.rows.length() > rows_max_len) {
            rows_max_len = info.rows.length();
        }
        if (info.cost.length() > cost_max_len) {
            cost_max_len = info.cost.length();
        }
        plan_infos.push_back(info);
    }
    for (u32 i = 0; i < plan_infos.size(); ++i) {
        PlanInfo &info = plan_infos[i];
        info.id += n_string(" ", id_max_len - info.id.length());
        info.op += n_string(" ", op_max_len - info.op.length());
        info.name += n_string(" ", name_max_len - info.name.length());
        uppercase(info.name);
        info.rows += n_string(" ", rows_max_len - info.rows.length());
        info.cost += n_string(" ", cost_max_len - info.cost.length());
    }
}

void PlanInfo::print_plan_info(Vector<PlanInfo> &plan_infos, String &explain_info)
{
    static const String BEGIN_TABLE = "+-";
    static const String END_TABLE = "-+";
    static const String BEGIN_LINE = "| ";
    static const String SEPARATOR = " | ";
    static const String END_LINE = " |";
    static const String NEW_LINE = "\n";
    static const String LINE = "-";
    if (plan_infos.size() < 1) {
        return;
    }
    u32 size = plan_infos[0].id.length() +
                SEPARATOR.length() +
                plan_infos[0].op.length() +
                SEPARATOR.length() +
                plan_infos[0].name.length() +
                SEPARATOR.length() +
                plan_infos[0].rows.length() +
                SEPARATOR.length() +
                plan_infos[0].cost.length();
    explain_info += "Query Plan:" + NEW_LINE;
    explain_info += BEGIN_TABLE + n_string(LINE, size) + END_TABLE + NEW_LINE;
    explain_info += BEGIN_LINE +
                    plan_infos[0].id + SEPARATOR +
                    plan_infos[0].op + SEPARATOR +
                    plan_infos[0].name + SEPARATOR +
                    plan_infos[0].rows + SEPARATOR +
                    plan_infos[0].cost + END_LINE + NEW_LINE;
    explain_info += BEGIN_TABLE + n_string(LINE, size) + END_TABLE + NEW_LINE;
    for (u32 i = 1; i < plan_infos.size(); ++i) {
        explain_info += BEGIN_LINE +
                    plan_infos[i].id + SEPARATOR +
                    plan_infos[i].op + SEPARATOR +
                    plan_infos[i].name + SEPARATOR +
                    plan_infos[i].rows + SEPARATOR +
                    plan_infos[i].cost + END_LINE + NEW_LINE;
    }
    explain_info += BEGIN_TABLE + n_string(LINE, size) + END_TABLE + NEW_LINE;
    explain_info += "Outputs & filters:" + NEW_LINE;
    for (u32 i = 1; i < plan_infos.size(); ++i) {
        if (!plan_infos[i].expr_info.empty()) {
            explain_info += plan_infos[i].expr_info;
        }
    }
}
