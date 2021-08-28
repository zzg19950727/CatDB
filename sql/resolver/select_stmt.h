#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include "dml_stmt.h"
#include "error.h"

namespace CatDB {
	namespace Parser {
		DECLARE(DMLStmt);
		DECLARE(SelectStmt);
		DECLARE(ColumnStmt);
		DECLARE(AggrStmt);
		DECLARE(ExprStmt);
		DECLARE(OrderStmt);
		DECLARE(LimitStmt);
		DECLARE(HintStmt);

		class OrderStmt
		{
		private:
			OrderStmt();
		public:
			~OrderStmt();
			static OrderStmt_s make_order_stmt(const ExprStmt_s& order_expr, bool asc = true);
			u32 formalize();
			KV_STRING(
				K(order_expr),
				K(asc)
			);

		public:
			ExprStmt_s order_expr;
			bool asc;//升序还是降序,true为asc，false为desc，默认asc
		};

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

		class HintStmt
		{
		private:
			HintStmt();
		public:
			enum HintType{Parallel=0};
			union HintBody
			{
				u32 dop;
			};
			~HintStmt();
			static HintStmt_s make_hint_stmt(HintType type, const HintBody& hint);
			u32 formalize();

		private:
			HintType type;
			HintBody hint;
		};

		class SelectStmt : public DMLStmt
		{
		protected:
			SelectStmt();
		public:
			~SelectStmt();
			StmtType stmt_type()const;
			static Stmt_s make_select_stmt();
			virtual u32 formalize();
			bool is_scalar_group_by() const;
			bool has_group_by() const;
			Vector<ExprStmt_s> &get_aggr_exprs()	{ return aggr_exprs; }
		protected:
			virtual u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;
		public:
			KV_STRING(
				KV(stmt_type, N(SELECT)),
				K(is_explain),
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
			HintStmt_s hint_list;			//
			bool is_distinct;			//输出集是否去重
			Vector<ExprStmt_s> select_expr_list;	//select语句块
			Vector<ExprStmt_s> group_exprs;		//groupby 列
			Vector<ExprStmt_s> having_stmt;			//having语句块
			Vector<OrderStmt_s> order_exprs;		//order by列
			LimitStmt_s limit_stmt;
		};

		DECLARE(SetStmt);
		class SetStmt : public SelectStmt
		{
		private:
			SetStmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op);
		public:
			~SetStmt();
			StmtType stmt_type()const;
			static Stmt_s make_set_stmt(const Stmt_s& left_query, const Stmt_s& right_query, SetOpType set_op);
			u32 formalize();
			u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs) override;

			KV_STRING(
				KV(stmt_type, N(SET)),
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