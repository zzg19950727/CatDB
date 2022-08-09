#include "stmt_compare.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "error.h"

using namespace CatDB::Parser;

void StmtCompareMap::reset()
{
    table_id_map.clear();
    table_map.clear();
    where_map.clear();
    group_map.clear();
    having_map.clear();
    select_map.clear();
    relation = UNCOMPARE;
}

ExprCompareCtx::ExprCompareCtx()
{

}

ExprCompareCtx::~ExprCompareCtx()
{

}

u32 ExprCompareCtx::compare_column(ColumnStmt *lhs, 
                                    ColumnStmt *rhs, 
                                    bool &is_equal)
{
    u32 ret = SUCCESS;
    is_equal = false;
    return ret;
}

u32 ExprCompareCtx::compare_query_ref(SubQueryStmt *lhs, 
                                    SubQueryStmt *rhs, 
                                    bool &is_equal)
{
    u32 ret = SUCCESS;
    is_equal = false;
    return ret;
}

u32 StmtCompareCtx::compare_column(ColumnStmt *lhs, 
                                    ColumnStmt *rhs, 
                                    bool &is_equal)
{
    u32 ret = SUCCESS;
    is_equal = false;
    if (!lhs || !rhs) {
        return ret;
    }
    if (lhs->column_id != rhs->column_id) {
        return ret;
    }
    for (u32 i = 0; !is_equal && i < compare_map.table_map.size(); ++i) {
        if ((compare_map.table_id_map[i].first == lhs->table_id &&
            compare_map.table_id_map[i].second == rhs->table_id) ||
            (compare_map.table_id_map[i].first == rhs->table_id &&
            compare_map.table_id_map[i].second == lhs->table_id)) {
            is_equal = true;
        }
    }
    return ret;
}

u32 StmtCompareCtx::compare_query_ref(SubQueryStmt *lhs, 
                                    SubQueryStmt *rhs, 
                                    bool &is_equal)
{
    u32 ret = SUCCESS;
    is_equal = false;
    if (!lhs || !rhs) {
        return ret;
    }
    StmtCompareMap cmp_map;
    CHECK(StmtCompare::compare_select_stmt(lhs->query_stmt, 
                                           rhs->query_stmt, 
                                           cmp_map));
    is_equal = cmp_map.relation == StmtCompareMap::EQUAL;
    return ret;
}

u32 StmtCompare::compare_stmt(DMLStmt_s &lhs, 
                            DMLStmt_s &rhs, 
                            StmtCompareMap &cmp_map)
{
    u32 ret = SUCCESS;
    if (lhs->is_select_stmt() && rhs->is_select_stmt()) {
        SelectStmt_s l_sel = lhs;
        SelectStmt_s r_sel = rhs;
        CHECK(compare_select_stmt(l_sel, r_sel, cmp_map));
    }
    return ret;
}

u32 StmtCompare::compare_select_stmt(SelectStmt_s &lhs, 
                                    SelectStmt_s &rhs, 
                                    StmtCompareMap &cmp_map)
{
    u32 ret = SUCCESS;
    if (lhs->is_set_stmt() && rhs->is_set_stmt()) {
        SetStmt_s lhs_set = lhs;
        SetStmt_s rhs_set = rhs;
        CHECK(compare_set_stmt(lhs_set, rhs_set, cmp_map));
    } else if (!lhs->is_set_stmt() && !rhs->is_set_stmt()) {
        CHECK(compare_normal_select_stmt(lhs, rhs, cmp_map));
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
    }
    LOG_TRACE("succeed to compare stmt", K(cmp_map));
    return ret;
}

u32 StmtCompare::compare_set_stmt(SetStmt_s &lhs, 
                                  SetStmt_s &rhs, 
                                  StmtCompareMap &cmp_map)
{
    u32 ret = SUCCESS;
    if (lhs->set_op != rhs->set_op || lhs->has_limit() || rhs->has_limit()) {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
    } else if (EXCEPT == lhs->set_op && EXCEPT == rhs->set_op) {
        StmtCompareMap l_cmp_map, r_cmp_map;
        CHECK(StmtCompare::compare_select_stmt(lhs->left_query, 
                                                rhs->left_query, 
                                                l_cmp_map));
        CHECK(StmtCompare::compare_select_stmt(lhs->right_query, 
                                                rhs->right_query, 
                                                r_cmp_map));
        if (StmtCompareMap::EQUAL == l_cmp_map.relation && 
            StmtCompareMap::EQUAL == r_cmp_map.relation) {
            cmp_map.relation = StmtCompareMap::EQUAL;
        } else if ((StmtCompareMap::EQUAL == l_cmp_map.relation ||
                    StmtCompareMap::LEFT_SUBSET == l_cmp_map.relation) &&
                   (StmtCompareMap::EQUAL == r_cmp_map.relation ||
                    StmtCompareMap::LEFT_SUBSET == r_cmp_map.relation))  {
            cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
        } else if ((StmtCompareMap::EQUAL == l_cmp_map.relation ||
                    StmtCompareMap::RIGHT_SUBSET == l_cmp_map.relation) &&
                   (StmtCompareMap::EQUAL == r_cmp_map.relation ||
                    StmtCompareMap::RIGHT_SUBSET == r_cmp_map.relation))  {
            cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
        } else {
            cmp_map.relation = StmtCompareMap::UNCOMPARE;
        }
    } else {
        StmtCompareMap l_cmp_map, r_cmp_map;
        CHECK(StmtCompare::compare_select_stmt(lhs->left_query, 
                                                rhs->left_query, 
                                                l_cmp_map));
        CHECK(StmtCompare::compare_select_stmt(lhs->right_query, 
                                                rhs->right_query, 
                                                r_cmp_map));
        if (StmtCompareMap::EQUAL == l_cmp_map.relation && 
            StmtCompareMap::EQUAL == r_cmp_map.relation) {
            cmp_map.relation = StmtCompareMap::EQUAL;
        } else if (StmtCompareMap::EQUAL == l_cmp_map.relation &&
                   StmtCompareMap::LEFT_SUBSET == r_cmp_map.relation)  {
            cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
        } else if (StmtCompareMap::EQUAL == l_cmp_map.relation &&
                   StmtCompareMap::RIGHT_SUBSET == r_cmp_map.relation)  {
            cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
        } else if (StmtCompareMap::LEFT_SUBSET == l_cmp_map.relation &&
                   StmtCompareMap::EQUAL == r_cmp_map.relation)  {
            cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
        } else if (StmtCompareMap::RIGHT_SUBSET == l_cmp_map.relation &&
                   StmtCompareMap::EQUAL == r_cmp_map.relation)  {
            cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
        } else {
            cmp_map.relation = StmtCompareMap::UNCOMPARE;
            return ret;
        }
        l_cmp_map.reset();
        r_cmp_map.reset();
        CHECK(StmtCompare::compare_select_stmt(lhs->left_query, 
                                                rhs->right_query, 
                                                l_cmp_map));
        CHECK(StmtCompare::compare_select_stmt(lhs->right_query, 
                                                rhs->left_query, 
                                                r_cmp_map));
        if (StmtCompareMap::EQUAL == l_cmp_map.relation && 
            StmtCompareMap::EQUAL == r_cmp_map.relation) {
            cmp_map.relation = StmtCompareMap::EQUAL;
        } else if ((StmtCompareMap::EQUAL == l_cmp_map.relation ||
                    StmtCompareMap::LEFT_SUBSET == l_cmp_map.relation) &&
                   (StmtCompareMap::EQUAL == r_cmp_map.relation ||
                    StmtCompareMap::LEFT_SUBSET == r_cmp_map.relation))  {
            cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
        } else if ((StmtCompareMap::EQUAL == l_cmp_map.relation ||
                    StmtCompareMap::RIGHT_SUBSET == l_cmp_map.relation) &&
                   (StmtCompareMap::EQUAL == r_cmp_map.relation ||
                    StmtCompareMap::RIGHT_SUBSET == r_cmp_map.relation))  {
            cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
        } else {
            cmp_map.relation = StmtCompareMap::UNCOMPARE;
            return ret;
        }
    }
    for (u32 i = 0; i < lhs->left_query->select_expr_list.size(); ++i) {
        cmp_map.select_map.push_back(i);
    }
    return ret;
}

u32 StmtCompare::compare_normal_select_stmt(SelectStmt_s &lhs, 
                                            SelectStmt_s &rhs, 
                                            StmtCompareMap &cmp_map)
{
    u32 ret = SUCCESS;
    bool is_equal = false;
    u32 match_count = 0;
    StmtCompareCtx ctx(cmp_map);
    //check from tables
    CHECK(compare_table_items(lhs->from_stmts,
                              rhs->from_stmts,
                              match_count,
                              ctx));
    if (match_count == lhs->from_stmts.size() &&
        match_count == rhs->from_stmts.size()) {
        cmp_map.relation = StmtCompareMap::EQUAL;
    } else if (match_count == lhs->from_stmts.size()) {
        cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
    } else if (match_count == rhs->from_stmts.size()) {
        cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    //check where condition
    CHECK(compare_exprs(lhs->where_stmt,
                        rhs->where_stmt,
                        cmp_map.where_map, 
                        match_count,
                        ctx));
    if (match_count == lhs->where_stmt.size() &&
        match_count == rhs->where_stmt.size()) {
        //do nothing
    } else if (match_count == lhs->where_stmt.size() &&
               (StmtCompareMap::RIGHT_SUBSET == cmp_map.relation || 
                StmtCompareMap::EQUAL == cmp_map.relation)) {
        cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
    } else if (match_count == rhs->where_stmt.size() &&
               (StmtCompareMap::LEFT_SUBSET == cmp_map.relation || 
                StmtCompareMap::EQUAL == cmp_map.relation)) {
        cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    //check group exprs
    CHECK(compare_exprs(lhs->group_exprs,
                        rhs->group_exprs,
                        cmp_map.group_map, 
                        match_count,
                        ctx));
    if (match_count == lhs->group_exprs.size() &&
        match_count == rhs->group_exprs.size() &&
        StmtCompareMap::EQUAL == cmp_map.relation) {
        //do nothing
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    //check having conditions
    CHECK(compare_exprs(lhs->having_stmt,
                        rhs->having_stmt,
                        cmp_map.having_map, 
                        match_count,
                        ctx));
    if (match_count == lhs->having_stmt.size() &&
        match_count == rhs->having_stmt.size()) {
        //do nothing
    } else if (match_count == lhs->having_stmt.size() &&
               (StmtCompareMap::RIGHT_SUBSET == cmp_map.relation || 
                StmtCompareMap::EQUAL == cmp_map.relation)) {
        cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
    } else if (match_count == rhs->having_stmt.size() &&
               (StmtCompareMap::LEFT_SUBSET == cmp_map.relation || 
                StmtCompareMap::EQUAL == cmp_map.relation)) {
        cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    //check select exprs
    CHECK(compare_exprs(lhs->select_expr_list,
                        rhs->select_expr_list,
                        cmp_map.select_map, 
                        match_count,
                        ctx));
    if (match_count == lhs->select_expr_list.size() &&
        match_count == rhs->select_expr_list.size()) {
        //do nothing
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    if ((lhs->has_distinct() && rhs->has_distinct()) ||
        (!lhs->has_distinct() && !rhs->has_distinct())) {
        //do nothing
    } else if (lhs->has_distinct() && 
               (StmtCompareMap::LEFT_SUBSET == cmp_map.relation || 
                StmtCompareMap::EQUAL == cmp_map.relation)) {
        cmp_map.relation = StmtCompareMap::LEFT_SUBSET;
    } else if (rhs->has_distinct() && 
               (StmtCompareMap::RIGHT_SUBSET == cmp_map.relation || 
                StmtCompareMap::EQUAL == cmp_map.relation)) {
        cmp_map.relation = StmtCompareMap::RIGHT_SUBSET;
    } else {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    if (lhs->has_limit() || rhs->has_limit()) {
        cmp_map.relation = StmtCompareMap::UNCOMPARE;
        return ret;
    }
    return ret;
}

u32 StmtCompare::compare_table_items(Vector<TableStmt_s> &lhs, 
                                    Vector<TableStmt_s> &rhs,
                                    u32 &match_count,
                                    StmtCompareCtx &ctx)
{
    u32 ret = SUCCESS;
    match_count = 0;
    bool is_equal = false;
    BitSet used_tables;
    for (u32 i = 0; i < lhs.size(); ++i) {
        is_equal = false;
        for (u32 j = 0; !is_equal && j < rhs.size(); ++j) {
            if (used_tables.has_member(j)) {
                continue;
            }
            CHECK(compare_table_item(lhs[i], 
                                     rhs[j], 
                                     is_equal, 
                                     ctx));
            if (is_equal) {
                ++match_count;
                ctx.compare_map.table_map.push_back(j);
                used_tables.add_member(j);
            }
        }
        if (!is_equal) {
            ctx.compare_map.table_map.push_back(INVALID_IDX);
        }
    }
    return ret;
}

u32 StmtCompare::compare_table_item(TableStmt_s &lhs, 
                                    TableStmt_s &rhs,
                                    bool &is_equal,
                                    StmtCompareCtx &ctx)
{
    u32 ret = SUCCESS;
    is_equal = false;
    if (lhs->is_dual_table() && rhs->is_dual_table()) {
        is_equal = true;
        StmtCompareMap::TableIdPair pair(lhs->table_id, rhs->table_id);
        ctx.compare_map.table_id_map.push_back(pair);
    } else if (lhs->is_basic_and_not_dual_table() && 
               rhs->is_basic_and_not_dual_table()) {
        BasicTableStmt_s lhs_basic = lhs;
        BasicTableStmt_s rhs_basic = rhs;
        is_equal = lhs_basic->ref_table_id == rhs_basic->ref_table_id;
        if (is_equal) {
            StmtCompareMap::TableIdPair pair(lhs->table_id, rhs->table_id);
            ctx.compare_map.table_id_map.push_back(pair);
        }
    } else if (lhs->is_view_table() && rhs->is_view_table()) {
        StmtCompareMap cmp_map;
        ViewTableStmt_s lhs_view = lhs;
        ViewTableStmt_s rhs_view = rhs;
        CHECK(StmtCompare::compare_select_stmt(lhs_view->ref_query, 
                                               rhs_view->ref_query, 
                                               cmp_map));
        is_equal = cmp_map.relation == StmtCompareMap::EQUAL;
        if (is_equal) {
            StmtCompareMap::TableIdPair pair(lhs->table_id, rhs->table_id);
            ctx.compare_map.table_id_map.push_back(pair);
        }
    } else if (lhs->is_joined_table() && rhs->is_joined_table()) {
        JoinedTableStmt_s lhs_joined_table = lhs;
        JoinedTableStmt_s rhs_joined_table = rhs;
        if (lhs_joined_table->join_type == rhs_joined_table->join_type) {
            bool is_left_equal = false;
            bool is_right_equal = false;
            CHECK(compare_table_item(lhs_joined_table->left_table, 
                                     rhs_joined_table->left_table, 
                                     is_left_equal, 
                                     ctx));
            CHECK(compare_table_item(lhs_joined_table->right_table, 
                                     rhs_joined_table->right_table, 
                                     is_right_equal, 
                                     ctx));
            if (is_left_equal && is_right_equal) {
                is_equal = true;
            } else if (Inner == lhs_joined_table->join_type || FullOuter == lhs_joined_table->join_type) {
                CHECK(compare_table_item(lhs_joined_table->left_table, 
                                        rhs_joined_table->right_table, 
                                        is_left_equal, 
                                        ctx));
                 CHECK(compare_table_item(lhs_joined_table->right_table, 
                                        rhs_joined_table->left_table, 
                                        is_right_equal, 
                                        ctx));
                if (is_left_equal && is_right_equal) {
                    is_equal = true;
                }
            }
        }
        if (is_equal) {
            Vector<int> exprs_map;
            u32 match_count = 0;
            CHECK(compare_exprs(lhs_joined_table->join_condition, 
                                rhs_joined_table->join_condition, 
                                exprs_map, 
                                match_count, 
                                ctx));
            if (match_count != lhs_joined_table->join_condition.size() || 
                match_count != rhs_joined_table->join_condition.size()) {
                is_equal = false;
            }
        }
    }
    if (is_equal) {
        Vector<int> exprs_map;
        u32 match_count = 0;
        CHECK(compare_exprs(lhs->table_filter, 
                            rhs->table_filter, 
                            exprs_map, 
                            match_count, 
                            ctx));
        if (match_count != lhs->table_filter.size() || 
            match_count != rhs->table_filter.size()) {
            is_equal = false;
        }
    }
    return ret;
}

u32 StmtCompare::compare_exprs(Vector<ExprStmt_s> &lhs, 
                                Vector<ExprStmt_s> &rhs, 
                                Vector<int> &exprs_map,
                                u32 &match_count,
                                StmtCompareCtx &ctx)
{
    u32 ret = SUCCESS;
    match_count = 0;
    bool is_equal = false;
    BitSet used_exprs;
    for (u32 i = 0; i < lhs.size(); ++i) {
        is_equal = false;
        for (u32 j = 0; !is_equal && j < rhs.size(); ++j) {
            if (used_exprs.has_member(j)) {
                continue;
            }
            is_equal = lhs[i]->same_as(rhs[j], &ctx);
            if (is_equal) {
                ++match_count;
                exprs_map.push_back(j);
                used_exprs.add_member(j);
            }
        }
        if (!is_equal) {
            exprs_map.push_back(INVALID_IDX);
        }
    }
    return ret;
}