#ifndef STMT_H
#define STMT_H
#include "type.h"

namespace CatDB {
	namespace Parser {
		class Stmt
		{
		public:
			enum StmtType {
				Expr = 0,
				Select,
				Update,
				Insert,
				Delete
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