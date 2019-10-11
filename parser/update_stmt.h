#ifndef UPDATE_STMT_H
#define UPDATE_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class UpdateStmt : public Stmt
		{
		private:
			UpdateStmt();
		public:
			~UpdateStmt();
			StmtType stmt_type()const;
			static Stmt_s make_update_stmt();
		public:
			Stmt_s table;
			Stmt_s update_asgn_stmt;
			Stmt_s where_stmt;
		};
	}
}

#endif	//UPDATE_STMT_H