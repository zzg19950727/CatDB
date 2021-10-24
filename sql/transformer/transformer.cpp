#include "transformer.h"
#include "transform_rule.h"
#include "transform_pre_process.h"
#include "transform_post_process.h"
#include "expr_stmt.h"
#include "dml_stmt.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

static const u32 MAX_TRANSFORM_COUNT = 10;

u32 Transformer::transform(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    bool happened = false;
    TRANSFORM(TransformPreProcess, stmt, happened);
    for (u32 i = 0; i < MAX_TRANSFORM_COUNT; ++i) {
        happened = false;
        //TRANSFORM(TransformJASubquery, stmt, happened);
        //TRANSFORM(TransformNSubquery, stmt, happened);
        //TRANSFORM(TransformASubquery, stmt, happened);
        //TRANSFORM(TransformJSubquery, stmt, happened);
        if (!happened) {
            break;
        } else {
            LOG_TRACE("success to transform once", K(i), K(stmt));
        }
    }
    TRANSFORM(TransformPostProcess, stmt, happened);
    return ret;
}