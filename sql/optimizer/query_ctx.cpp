#include "query_ctx.h"
#include "error.h"

using namespace CatDB::Sql;

QueryCtx_s QueryCtx::make_query_ctx()
{
    return QueryCtx_s(new QueryCtx());
}

void QueryCtx::reset() 
{
    killed = false; 
    //reset_table_id
    cur_table_id = 1;
    //reset_stmt_id
    cur_stmt_id = 1; 
    //reset_sample_size
    sample_size = 1.0;
    //reset_view_id
    cur_view_id = 1;
    affected_rows = 0;
    err_msg.clear(); 
}

u32 QueryCtx::check_query_status() 
{ 
    if (killed) 
        return QUERY_KILLED; 
    else 
        return SUCCESS; 
}