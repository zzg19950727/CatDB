#ifndef DROP_TABLE_STMT_H
#define DROP_TABLE_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class DropTableStmt : public Stmt
		{
		public:
			DropTableStmt();
		public:
			~DropTableStmt();
			StmtType stmt_type()const;
			static Stmt_s make_drop_table_stmt();
		public:
			Stmt_s table;
		};
	}
}

#endif	//DROP_TABLE_STMT_H