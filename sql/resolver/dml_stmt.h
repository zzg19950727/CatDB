#ifndef DML_STMT_H
#define DML_STMT_H
#include "bit_set.h"
#include "stmt.h"

namespace CatDB {
	namespace Parser {
		DECLARE(TableStmt);
		DECLARE(ExprStmt);
		DECLARE(ColumnStmt);
		DECLARE(AggrStmt);
		DECLARE(SubQueryStmt);
		DECLARE(DMLStmt);

		class DMLStmt : public Stmt
		{
		public:
			DMLStmt();
		public:
			virtual ~DMLStmt();
			virtual StmtType stmt_type() const = 0;
			virtual u32 formalize();
			u32 get_stmt_exprs(Vector<ExprStmt_s> &exprs);
			u32 collect_special_exprs();
			u32 get_column_exprs(u32 table_id, Vector<ExprStmt_s> &columns);
			u32 get_table_items(Vector<TableStmt_s> &tables);
			Vector<ExprStmt_s> &get_column_exprs() { return column_exprs; }
			Vector<SubQueryStmt_s> &get_subquery_exprs() { return subquery_exprs; }

		protected:
			u32 collect_special_exprs(ExprStmt_s& expr);
			virtual u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs);

		public:
			virtual KV_STRING(
				K(is_explain),
				K(from_stmts),
				K(where_stmt)
			);

		public:
			Vector<TableStmt_s> from_stmts;			//from语句块
			Vector<ExprStmt_s> where_stmt;			//where语句块
			BitSet table_ids;
			bool is_explain;

			Vector<ExprStmt_s> column_exprs;
			Vector<ExprStmt_s> aggr_exprs;
			Vector<SubQueryStmt_s> subquery_exprs;
		};
	}
}

#endif	//DML_STMT_H