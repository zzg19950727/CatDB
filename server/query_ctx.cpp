#include "query_ctx.h"
#include "error.h"

using namespace CatDB::Server;
using namespace CatDB::Parser;

QueryCtx::QueryCtx()
{
    reset();
}

QueryCtx_s QueryCtx::make_query_ctx()
{
    return QueryCtx_s(new QueryCtx());
}

void QueryCtx::reset() 
{
    err_msg.clear();
    generate_trace_id();
    //reset_table_id
    cur_table_id = 1;
    //reset_stmt_id
    cur_stmt_id = 1; 
    //reset_view_id
    cur_view_id = 1;
    param_index = 1;
    //reset_sample_value
    sample_value = -1.0;
    affected_rows = 0;
}

void QueryCtx::generate_trace_id()
{
    std::thread::id id = std::this_thread::get_id();
	CatDB::Server::DateTime::TimePoint tp = std::chrono::steady_clock::now();
	CatDB::Server::DateTime::NanoSeconds nano(tp.time_since_epoch());
	Hash<CatDB::Server::DateTime::Rep> func;
	u64 value = func(nano.count());
	static const char *hex_map = "0123456789ABCDEF";
    last_trace_id = trace_id;
	trace_id.clear();
	for (u32 i = 0; i < 16; ++i) {
		trace_id += hex_map[value & 0xf];
		value >>= 4;
	}
}