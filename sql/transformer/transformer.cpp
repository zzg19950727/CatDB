#include "transformer.h"
#include "transform_rule.h"
#include "transform_pre_process.h"
#include "transform_post_process.h"
#include "transform_full_outer_join.h"
#include "transform_merge_view.h"
#include "transform_unnest_J_N_subquery.h"
#include "transform_unnest_JA_subquery.h"
#include "transform_simplify_subquery.h"
#include "transform_expr_normalize.h"
#include "transform_win_magic.h"
#include "select_stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "session_info.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

static const u32 MAX_TRANSFORM_COUNT = 10;

TransformCtx::TransformCtx()
{
    
}

TransformCtx::~TransformCtx()
{

}

TransformCtx_s TransformCtx::make_transform_ctx()
{
    return TransformCtx_s(new TransformCtx());
}

u32 Transformer::transform(DMLStmt_s &stmt, TransformCtx_s &ctx)
{
    u32 ret = SUCCESS;
    bool happened = false;
    TRANSFORM(TransformPreProcess, stmt, ctx, happened);
    for (u32 i = 0; i < MAX_TRANSFORM_COUNT; ++i) {
        happened = false;
        //must transfrom at first time
        TRANSFORM(TransformFullOuterJoin, stmt, ctx, happened);
        //
        TRANSFORM(TransformExprNormalize, stmt, ctx, happened);
        //
        TRANSFORM(TransformMergeView, stmt, ctx, happened);
        //
        TRANSFORM(TransformSimplifySubquery, stmt, ctx, happened);
        //
        TRANSFORM(TransformWinMagic, stmt, ctx, happened);
        //
        TRANSFORM(TransformUnnestJASubquery, stmt, ctx, happened);
        //
        TRANSFORM(TransformUnnestJNSubquery, stmt, ctx, happened);
        LOG_TRACE("success to transform once", K(i), K(stmt));
        if (!happened) {
            break;
        }
    }
    TRANSFORM(TransformPostProcess, stmt, ctx, happened);
    return ret;
}