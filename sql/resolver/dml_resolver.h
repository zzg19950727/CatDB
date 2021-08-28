#ifndef DML_RESOLVER_H
#define DML_RESOLVER_H
#include "query_ctx.h"
#include "plan.h"
#include "stmt.h"
#include "type.h"
#include "log.h"

namespace CatDB {
	namespace Parser {
        DECLARE(Stmt);
        DECLARE(DMLStmt);
        DECLARE(SelectStmt);
        DECLARE(ExprStmt);
        DECLARE(AggrStmt);
        DECLARE(ListStmt);
        DECLARE(ColumnStmt);
        DECLARE(ExecParamStmt);
        DECLARE(TableStmt);
        DECLARE(BasicTableStmt);
        DECLARE(JoinedTableStmt);
        DECLARE(ViewTableStmt);
        using Sql::QueryCtx;
        
        struct ResolveCtx {
            Vector<TableStmt_s> parent_tables;
            Vector<TableStmt_s> cur_tables;
            Vector<ExecParamStmt_s> columns_from_parent;
        };
        class DMLResolver
        {
        public:
            DMLResolver(DMLStmt_s stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx);
            ~DMLResolver();
            virtual u32 resolve_stmt() = 0;
            String get_error_msg() const {return err_msg;}
            static u32 resolve_stmt(Stmt_s stmt, QueryCtx &query_ctx, ResolveCtx &resolve_ctx);
        protected:
            u32 resolve_from_stmt();
            u32 resolve_where_stmt();
            u32 resolve_table_item(TableStmt_s& table_stmt);
            u32 resolve_basic_table_item(BasicTableStmt_s table_stmt);
            u32 resolve_joined_table_item(JoinedTableStmt_s table_stmt);
            u32 resolve_view_table_item(ViewTableStmt_s table_stmt);

            u32 flatten_exprs(ExprStmt_s& expr_stmt, Vector<ExprStmt_s> &expr_list);
            u32 resolve_exprs(Vector<ExprStmt_s>& expr_stmt, ResolveCtx &resolve_ctx);
            u32 resolve_expr(ExprStmt_s& expr_stmt, ResolveCtx &resolve_ctx);
            u32 resolve_column(ExprStmt_s &column, ResolveCtx &resolve_ctx);
            u32 resolve_column(TableStmt_s &table, ColumnStmt_s &column);
            u32 resolve_column_from_basic_table(BasicTableStmt_s table, ColumnStmt_s &column);
            u32 resolve_column_from_view(ViewTableStmt_s table, ColumnStmt_s &column);
            u32 resolve_subquery(SelectStmt_s &query_stmt, ResolveCtx &resolve_ctx);
            bool find_table_name(const String& table_name);
            bool find_table_item(u32 table_id);

        protected:
            DMLStmt_s stmt;
            String err_msg;
            ResolveCtx &resolve_ctx;
            QueryCtx &query_ctx;
            Vector<AggrStmt_s> aggr_funcs;
            bool can_use_aggr_func;
        };
    }
}

#endif //DML_RESOLVER_H