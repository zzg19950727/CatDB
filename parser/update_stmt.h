#ifndef UPDATE_STMT_H
#define UPDATE_STMT_H

#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);

		class UpdateStmt : public DMLStmt
		{
		private:
			UpdateStmt();
		public:
			~UpdateStmt();
			StmtType stmt_type()const;
			static Stmt_s make_update_stmt();
			KV_STRING(
				K(is_explain),
				K(table),
				K(where_stmt),
				K(update_asgn_stmt)
			);

		public:
			Stmt_s table;
			Stmt_s update_asgn_stmt;
		};
	}
}

#endif	//UPDATE_STMT_H
