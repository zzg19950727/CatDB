#ifndef INSERT_STMT_H
#define INSERT_STMT_H

#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);
		DECLARE(BasicTableStmt);
		DECLARE(SelectStmt);
		DECLARE(InsertStmt);

		class InsertStmt : public DMLStmt
		{
		private:
			InsertStmt();
		public:
			~InsertStmt();
			StmtType stmt_type()const override;
			static Stmt_s make_insert_stmt();
			u32 formalize();
		protected:
			u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;
			u32 inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   		 const Vector<ExprStmt_s> &new_exprs)override;
		public:
			DECLARE_KV_STRING_OVERRIDE;

		public:
			BasicTableStmt_s table;
			Vector<ExprStmt_s> value_list;
			SelectStmt_s query_values;
		};
	}
}

#endif	//INSERT_STMT_H
