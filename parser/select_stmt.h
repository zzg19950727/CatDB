#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class SelectStmt : public Stmt
		{
		private:
			SelectStmt();
		public:
			~SelectStmt();
			StmtType stmt_type()const;
			static Stmt_s make_select_stmt();
		public:
			Stmt_s select_expr_list;	//select语句块
			bool is_distinct;				//输出集是否去重？
			Stmt_s from_stmts;		//from语句块
			Stmt_s where_stmt;				//where语句块
			Stmt_s group_columns;	//groupby 列
			Stmt_s having_stmt;				//having语句块
			Stmt_s order_columns;	//order by列
			bool asc_desc;					//升序还是降序,true为asc，false为desc，默认asc
			Stmt_s limit_stmt;
		};

		class LimitStmt : public Stmt
		{
		private:
			LimitStmt();
		public:
			~LimitStmt();
			StmtType stmt_type()const;
			static Stmt_s make_limit_stmt(u32 size, u32 offset = 0);
		private:
			u32 limit_offset;			//limit行偏移
			u32 limit_size;				//limit行数
		};
	}
}

#endif	//SELECT_STMT_H