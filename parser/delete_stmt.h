#ifndef DELETE_STMT_H
#define DELETE_STMT_H

#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);

		class DeleteStmt : public DMLStmt
		{
		private:
			DeleteStmt();
		public:
			~DeleteStmt();
			StmtType stmt_type()const;
			static Stmt_s make_delete_stmt();
		};
	}
}

#endif	//DELETE_STMT_H