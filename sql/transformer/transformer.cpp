#include "transformer.h"
#include "transform_rule.h"
#include "transform_pre_process.h"
#include "transform_post_process.h"
#include "transform_full_outer_join.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "dml_stmt.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

static const u32 MAX_TRANSFORM_COUNT = 10;

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
        TRANSFORM(TransformFullOuterJoin, stmt, ctx, happened);
        //TRANSFORM(TransformJASubquery, stmt, ctx, happened);
        //TRANSFORM(TransformNSubquery, stmt, ctx, happened);
        //TRANSFORM(TransformASubquery, stmt, ctx, happened);
        //TRANSFORM(TransformJSubquery, stmt, ctx, happened);
        if (!happened) {
            break;
        } else {
            LOG_ERR("success to transform once", K(i), K(stmt));
        }
    }
    TRANSFORM(TransformPostProcess, stmt, ctx, happened);
    return ret;
}