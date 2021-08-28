#ifndef STMT_H
#define STMT_H
#include "error.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class Stmt
		{
		public:
			enum StmtType {
				Select = 0,
				Update,
				Insert,
				Delete,
				SetOperation,
				DoCMD
			};
			Stmt();
			virtual ~Stmt();
			virtual StmtType stmt_type()const = 0;
			virtual u32 formalize() = 0;
			bool is_select_stmt() const { return stmt_type() == Select; }
			VIRTUAL_KV_STRING(
				K("")
			);

		private:
			DISALLOW_COPY_AND_ASSIGN(Stmt);
		};
	}
}
#endif	//STMT_H