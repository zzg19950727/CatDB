#ifndef STMT_H
#define STMT_H
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class Stmt
		{
		public:
			enum StmtType {
				Expr = 0,
				Select,
				Limit,
				Update,
				Insert,
				Delete,
				ColumnDefine,
				CreateTable,
				DropTable
			};
			Stmt();
			virtual ~Stmt();
			virtual StmtType stmt_type()const = 0;
		private:
			DISALLOW_COPY_AND_ASSIGN(Stmt);
		};
	}
}
#endif	//STMT_H