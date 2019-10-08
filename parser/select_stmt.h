#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(ExprStmt);

		class SelectStmt : public Stmt
		{
		public:
			SelectStmt();
			~SelectStmt();
			StmtType stmt_type()const;
		public:
			Vector<ExprStmt_s> select_stmts;	//selectÓï¾ä¿é
			Vector<ExprStmt_s> from_stmts;		//fromÓï¾ä¿é
			ExprStmt_s where_stmt;				//whereÓï¾ä¿é
			Vector<ExprStmt_s> group_columns;	//groupby ÁĞ
			ExprStmt_s having_stmt;				//havingÓï¾ä¿é
			Vector<ExprStmt_s> order_columns;	//order byÁĞ
			ExprStmt_s order_stmt;				//ÉıĞò»¹ÊÇ½µĞò
			ExprStmt_s limit_offset;			//limitĞĞÆ«ÒÆ
			ExprStmt_s limit_size;				//limitĞĞÊı
		};
	}
}

#endif	//SELECT_STMT_H