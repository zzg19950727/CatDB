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

u32 TransformPostProcess::transform_one_stmt(DMLStmt_s &stmt)
{
    return SUCCESS;
}