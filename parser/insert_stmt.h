#ifndef INSERT_STMT_H
#define INSERT_STMT_H

#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);

		class InsertStmt : public DMLStmt
		{
		private:
			InsertStmt();
		public:
			~InsertStmt();
			StmtType stmt_type()const;
			static Stmt_s make_insert_stmt();
			KV_STRING(
				K(is_explain),
				K(table),
				K(where_stmt),
				K(values)
			);

		public:
			Stmt_s table;
			Stmt_s values;
		};
	}
}

#endif	//INSERT_STMT_H
