#ifndef STMT_H
#define STMT_H
#include "type.h"
#include "log.h"

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
				Hint,
				Update,
				Insert,
				Delete,
				ColumnDefine,
				CMDParam,
				DoCMD
			};
			Stmt();
			virtual ~Stmt();
			virtual StmtType stmt_type()const = 0;
			bool is_explain;

			virtual KV_STRING(
				K(is_explain)
			);

		private:
			DISALLOW_COPY_AND_ASSIGN(Stmt);
		};
	}
}
#endif	//STMT_H