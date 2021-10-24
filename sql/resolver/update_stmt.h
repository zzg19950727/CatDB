#ifndef UPDATE_STMT_H
#define UPDATE_STMT_H

#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);
		DECLARE(ExprStmt);
		DECLARE(ColumnStmt);
		DECLARE(BasicTableStmt);
		DECLARE(UpdateStmt);

		class UpdateStmt : public DMLStmt
		{
		private:
			UpdateStmt();
		public:
			~UpdateStmt();
			StmtType stmt_type()const;
			static Stmt_s make_update_stmt();
			u32 formalize();
		protected:
			u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;
		public:
			KV_STRING(
				KV(stmt_type, N(UPDATE)),
				K(is_explain),
				K(stmt_hint),
				K(table),
				K(update_assign_stmt),
				K(from_stmts),
				K(where_stmt)
			);

		public:
			BasicTableStmt_s table;
			Vector<ExprStmt_s> update_assign_stmt;
			Vector<ColumnStmt_s> column_exprs;
			Vector<ExprStmt_s> value_exprs;
			ColumnStmt_s row_id_col;
		};
	}
}

#endif	//UPDATE_STMT_H
