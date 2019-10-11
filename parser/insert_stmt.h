#ifndef INSERT_STMT_H
#define INSERT_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class InsertStmt : public Stmt
		{
		private:
			InsertStmt();
		public:
			~InsertStmt();
			StmtType stmt_type()const;
			static Stmt_s make_insert_stmt();
		public:
			Stmt_s table;
			Stmt_s values;
		};
	}
}

#endif	//INSERT_STMT_H