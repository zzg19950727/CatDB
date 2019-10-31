#ifndef DROP_STMT_H
#define DROP_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class DropTableStmt : public Stmt
		{
		private:
			DropTableStmt();
		public:
			~DropTableStmt();
			StmtType stmt_type()const;
			static Stmt_s make_drop_table_stmt();
		public:
			Stmt_s table;
		};

		class DropDatabaseStmt : public Stmt
		{
		public:
			DropDatabaseStmt();
		public:
			~DropDatabaseStmt();
			StmtType stmt_type()const;
			static Stmt_s make_drop_database_stmt();
		public:
			String database;
		};
	}
}

#endif	//DROP_STMT_H