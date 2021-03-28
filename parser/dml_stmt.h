#ifndef DML_STMT_H
#define DML_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class DMLStmt : public Stmt
		{
		protected:
			DMLStmt() {};
		public:
			virtual ~DMLStmt() {};
			virtual StmtType stmt_type() const = 0;
		public:
			Stmt_s from_stmts;			//from语句块
			Stmt_s where_stmt;			//where语句块
		};
	}
}

#endif	//DML_STMT_H