﻿#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include "dml_stmt.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);
		DECLARE(SelectStmt);
		DECLARE(ColumnStmt);
		DECLARE(AggrStmt);
		DECLARE(ExprStmt);
		DECLARE(LimitStmt);
		DECLARE(HintStmt);

		class LimitStmt
		{
		private:
			LimitStmt();
		public:
			~LimitStmt();
			static LimitStmt_s make_limit_stmt(u32 size, u32 offset = 0);
			u32 formalize();
			KV_STRING(
				K(limit_offset),
				K(limit_size)
			);

		public:
			u32 limit_offset;			//limit行偏移
			u32 limit_size;				//limit行数
		};

		class SelectStmt : public DMLStmt
		{
		protected:
			SelectStmt();
		public:
			virtual ~SelectStmt();
			virtual StmtType stmt_type()const override;
			static Stmt_s make_select_stmt();
			virtual u32 formalize() override;
			virtual u32 deep_copy(SelectStmt_s &stmt, u32 flag)const;
			bool is_scalar_group_by() const;
			bool has_group_by() const;
			bool has_order_by() const;
			bool has_limit() const;
			bool has_distinct() const;
			bool has_window_func() const;
			Vector<ExprStmt_s> &get_aggr_exprs()	{ return aggr_exprs; }
			Vector<ExprStmt_s> &get_win_func_exprs() { return win_func_exprs; }
		protected:
			virtual u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;
			virtual u32 inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   		 		 const Vector<ExprStmt_s> &new_exprs)override;
		public:
			VIRTUAL_KV_STRING_OVERRIDE(
				KV(stmt_type, StmtTypeString[stmt_type()]),
				K(is_explain),
				K(stmt_hint),
				K(is_distinct),
				K(select_expr_list),
				K(from_stmts),
				K(where_stmt),
				K(group_exprs),
				K(having_stmt),
				K(order_exprs),
				K(limit_stmt)
			);

		public:
			bool is_distinct;			//输出集是否去重
			Vector<ExprStmt_s> select_expr_list;	//select语句块
			Vector<ExprStmt_s> group_exprs;		//groupby 列
			Vector<ExprStmt_s> having_stmt;			//having语句块
			Vector<ExprStmt_s> order_exprs;		//order by列
			LimitStmt_s limit_stmt;
		};

		DECLARE(SetStmt);
		class SetStmt : public SelectStmt
		{
		private:
			SetStmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op);
		public:
			~SetStmt();
			StmtType stmt_type()const override;
			static Stmt_s make_set_stmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op);
			u32 formalize() override;
			u32 deep_copy(SelectStmt_s &stmt, u32 flag)const override;
		private:
			u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;
			u32 inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   		 const Vector<ExprStmt_s> &new_exprs)override;

			KV_STRING_OVERRIDE(
				KV(stmt_type, StmtTypeString[stmt_type()]),
				KV(set_op, SetOpTypeString[set_op]),
				K(left_query),
				K(right_query),
				K(select_expr_list)
			);

		public:
			SelectStmt_s left_query;
			SelectStmt_s right_query;
			SetOpType set_op;
		};
	}
}

#endif	//SELECT_STMT_H