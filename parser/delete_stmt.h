#ifndef DELETE_STMT_H
#define DELETE_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class DeleteStmt : public Stmt
		{
		private:
			DeleteStmt();
		public:
			~DeleteStmt();
			StmtType stmt_type()const;
			static Stmt_s make_delete_stmt();
		public:
			Stmt_s table;
			Stmt_s where_stmt;
		};
	}
}

#endif	//DELETE_STMT_H