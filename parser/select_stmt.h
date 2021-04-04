#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include "dml_stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);

		class SelectStmt : public DMLStmt
		{
		private:
			SelectStmt();
		public:
			~SelectStmt();
			StmtType stmt_type()const;
			static Stmt_s make_select_stmt();
			KV_STRING(
				K(is_explain),
				K(select_expr_list),
				K(is_distinct),
				K(from_stmts),
				K(where_stmt),
				K(group_columns),
				K(having_stmt),
				K(order_columns),
				K(asc_desc),
				K(limit_stmt)
			);

		public:
			Stmt_s select_expr_list;	//select语句块
			Stmt_s hint_list;			//
			bool is_distinct;			//输出集是否去重？
			Stmt_s group_columns;		//groupby 列
			Stmt_s having_stmt;			//having语句块
			Stmt_s order_columns;		//order by列
			bool asc_desc;				//升序还是降序,true为asc，false为desc，默认asc
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
			KV_STRING(
				K(limit_offset),
				K(limit_size)
			);

		public:
			u32 limit_offset;			//limit行偏移
			u32 limit_size;				//limit行数
		};

		class HintStmt : public Stmt
		{
		public:
			HintStmt();
		public:
			enum HintType{Parallel=0};
			union HintBody
			{
				u32 dop;
			};
			~HintStmt();
			StmtType stmt_type()const override;
			static Stmt_s make_hint_stmt(HintType type, const HintBody& hint);

		private:
			HintType type;
			HintBody hint;
		};
	}
}

#endif	//SELECT_STMT_H