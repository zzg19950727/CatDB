#include "transform_pre_process.h"
#include "sql_engine.h"
#include "expr_stmt.h"
#include "expr_utils.h"
#include "dml_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformPreProcess::TransformPreProcess()
    :TransformRule(POST_ORDER)
{

}

TransformPreProcess::~TransformPreProcess()
{

}

bool TransformPreProcess::need_rewrite(DMLStmt_s stmt) const
{
    return true;
}

u32 TransformPreProcess::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    bool happened = false;
    CHECK(simplify_calculable_expr(stmt, happened));
    if (happened) {
        set_transform_happened();
        LOG_TRACE("succeed to simplify calculable expr", K(ret));
    }
    return ret;
}

u32 TransformPreProcess::simplify_calculable_expr(DMLStmt_s &stmt, bool &happened)
{
    u32 ret = SUCCESS;
    ExprStmt_s old_expr;
    Vector<ExprStmt_s> all_exprs;
    Vector<ExprStmt_s> old_exprs;
    Vector<ExprStmt_s> new_exprs;
    CHECK(stmt->get_stmt_exprs(all_exprs));
    for (u32 i = 0; i < all_exprs.size(); ++i) {
        old_expr = all_exprs[i];
        CHECK(inner_simplify_calculable_expr(all_exprs[i], happened));
        if (old_expr != all_exprs[i]) {
            old_exprs.push_back(old_expr);
            new_exprs.push_back(all_exprs[i]);
        }
    }
    if (!old_exprs.empty()) {
        CHECK(stmt->replace_stmt_exprs(old_exprs, new_exprs));
    }
    return ret;
}

u32 TransformPreProcess::inner_simplify_calculable_expr(ExprStmt_s &expr, bool &happened)
{
    u32 ret = SUCCESS;
    bool is_calculable = false;
    CHECK(ExprUtils::is_calculable_expr(expr, is_calculable));
    if (is_calculable) {
        Object_s obj_result;
        bool bool_result = false;
        CHECK(Sql::SqlEngine::handle_const_expr(expr, obj_result, bool_result));
        if (expr->res_type.is_bool()) {
            CHECK(ExprUtils::make_bool_expr(expr, bool_result));
        } else if (!obj_result) {
            ret = ERR_UNEXPECTED;
            LOG_ERR("failed to calc const expr", K(expr), K(obj_result));
        } else {
            ExprStmt_s new_expr = ConstStmt::make_const_stmt(obj_result);
            new_expr->alias_name = expr->alias_name;
            new_expr->res_type = expr->res_type;
            expr = new_expr;
        }
        happened = true;
    } else {
        for (u32 i = 0; i < expr->params.size(); ++i) {
            CHECK(inner_simplify_calculable_expr(expr->params[i], happened));
        }
    }
    return ret;
}