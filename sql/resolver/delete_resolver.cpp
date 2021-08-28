#include "delete_resolver.h"
#include "delete_stmt.h"
#include "expr_stmt.h"
#include "error.h"
using namespace CatDB::Parser;
using namespace CatDB::Common;
using namespace CatDB::Sql;

DeleteResolver::DeleteResolver(DeleteStmt_s &stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx)
    :DMLResolver(stmt, query_ctx, resolve_ctx),
    delete_stmt(stmt)
{
    
}

DeleteResolver::~DeleteResolver()
{

}
u32 DeleteResolver::resolve_stmt()
{
    u32 ret = SUCCESS;
    CHECK(resolve_from_stmt());
    CHECK(resolve_where_stmt());
    MY_ASSERT(delete_stmt->table);
    BasicTableStmt_s &delete_table = delete_stmt->table;
    const String& table_name = delete_table->table_name;
    bool find = false;
    for (u32 i = 0; !find && i < resolve_ctx.cur_tables.size(); ++i) {
        if (resolve_ctx.cur_tables[i]->alias_name == table_name
            && resolve_ctx.cur_tables[i]->is_basic_and_not_dual_table()) {
            find = true;
            delete_stmt->row_id_col = ColumnStmt::make_column_stmt(table_name, "ROWID");
            delete_stmt->row_id_col->table_id = resolve_ctx.cur_tables[i]->table_id;
            delete_stmt->row_id_col->column_id = ROWID_COLUMN_ID;
            delete_stmt->table = resolve_ctx.cur_tables[i];
        }
    }
    MY_ASSERT(find);
    return ret;
}