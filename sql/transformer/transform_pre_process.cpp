#include "transform_pre_process.h"
#include "expr_stmt.h"
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

u32 TransformPreProcess::transform_one_stmt(DMLStmt_s &stmt)
{
    return SUCCESS;
}