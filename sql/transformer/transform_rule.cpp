#include "transform_rule.h"
#include "session_info.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformRule::TransformRule(bool pre_order, HintType control_hint)
    :pre_order(pre_order),
    control_hint(control_hint),
    trans_happened(false)
{

}

TransformRule::~TransformRule()
{

}

bool TransformRule::need_rewrite(DMLStmt_s stmt) const
{
    return !QUERY_CTX->query_hint->enable_no_rewrite(stmt->get_qb_name());
}

void TransformRule::set_transform_ctx(TransformCtx_s &ctx)
{ 
    this->ctx = ctx; 
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
    /**
      * do {
      *     1. check hint disable
      *     2. check validity
      *     4. do transform
      *     5. generate outline
      * } while
      */
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
    if (!need_rewrite(stmt)) {
        return ret;
    }
    CHECK(transform_one_stmt(stmt));
    if (trans_happened) {
        CHECK(stmt->formalize());
    }
    return ret;
}

u32 TransformRule::transform_chidren(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    Vector<SelectStmt_s> child_stmts;
    CHECK(stmt->get_child_stmts(child_stmts));
    for (u32 i = 0; i < child_stmts.size(); ++i) {
        CHECK(transform(child_stmts[i]));
    }
    return ret;
}