#ifndef DML_STMT_H
#define DML_STMT_H

#include "stmt.h"
#include "type.h"
#include "log.h"

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
			virtual KV_STRING(
				K(from_stmts),
				K(where_stmt)
			);

		public:
			Stmt_s from_stmts;			//from语句块
			Stmt_s where_stmt;			//where语句块
		};
	}
}

#endif	//DML_STMT_H