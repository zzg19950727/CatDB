#ifndef DELETE_STMT_H
#define DELETE_STMT_H

#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(ColumnStmt);
		DECLARE(BasicTableStmt);
		DECLARE(DeleteStmt);

		class DeleteStmt : public DMLStmt
		{
		private:
			DeleteStmt();
		public:
			~DeleteStmt();
			StmtType stmt_type()const;
			static Stmt_s make_delete_stmt();
			u32 formalize();
		protected:
			u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;
		public:
			KV_STRING(
				KV(stmt_type, N(DELETE)),
				K(is_explain),
				K(stmt_hint),
				K(table),
				K(from_stmts),
				K(where_stmt)
			);
			BasicTableStmt_s table;
			ColumnStmt_s row_id_col;
		};
	}
}

#endif	//DELETE_STMT_H
