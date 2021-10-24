#include "transform_post_process.h"
#include "expr_stmt.h"
#include "dml_stmt.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Transform;
using namespace CatDB::Parser;

TransformPostProcess::TransformPostProcess()
    :TransformRule(POST_ORDER)
{

}

TransformPostProcess::~TransformPostProcess()
{

}

bool TransformPostProcess::need_rewrite(DMLStmt_s stmt) const
{
    return true;
}

u32 TransformPostProcess::transform_one_stmt(DMLStmt_s &stmt)
{
    u32 ret = SUCCESS;
    bool trans_happend = false;
    CHECK(finial_table_hint(stmt, trans_happend));
    LOG_TRACE("succeed to final table hint", K(trans_happend));
    return ret;
}

u32 TransformPostProcess::finial_table_hint(DMLStmt_s &stmt, bool &happened)
{
    u32 ret = SUCCESS;
    CHECK(stmt->update_table_hint());
    return ret;
}