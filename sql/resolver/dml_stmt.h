#ifndef DML_STMT_H
#define DML_STMT_H
#include "bit_set.h"
#include "stmt.h"
#include "expr_stmt.h"
#include "table_stmt.h"
#include "hint_stmt.h"

namespace CatDB {
	namespace Parser {
		DECLARE(TableStmt);
		DECLARE(ExprStmt);
		DECLARE(ColumnStmt);
		DECLARE(AggrStmt);
		DECLARE(SubQueryStmt);
		DECLARE(SelectStmt);
		DECLARE(DMLStmt);
		
		class DMLStmt : public Stmt
		{
		public:
			DMLStmt();
		public:
			virtual ~DMLStmt();
			virtual StmtType stmt_type() const override = 0;
			virtual u32 formalize() override;
			void increase_ref_count() { ++ref_count; }
			void decrease_ref_count() { --ref_count; }
			
			u32 get_stmt_exprs(Vector<ExprStmt_s> &exprs);
			u32 replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   const Vector<ExprStmt_s> &new_exprs,
								   const bool recurse_child_stmt = false);
			u32 collect_special_exprs();
			u32 get_column_exprs(u32 table_id, Vector<ExprStmt_s> &columns);
			u32 get_table_items(Vector<TableStmt_s> &tables);
			Vector<ExprStmt_s> &get_column_exprs() { return column_exprs; }
			Vector<SubQueryStmt_s> &get_subquery_exprs() { return subquery_exprs; }
			
			u32 get_child_stmts(Vector<SelectStmt_s> &child_stms);
			u32 reset_stmt_id(u32 stmt_id);
			const String& get_qb_name() const { return stmt_hint.qb_name; }
			bool find_table_id(const String &table_name, u32 &table_id);

		private:

		protected:
			u32 collect_special_exprs(ExprStmt_s& expr);
			virtual u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs);
			virtual u32 inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   				 const Vector<ExprStmt_s> &new_exprs);
			u32 inner_deep_copy(DMLStmt_s stmt, u32 flag)const;

		public:
			VIRTUAL_KV_STRING_OVERRIDE(
				K(is_explain),
				K(stmt_hint),
				K(from_stmts),
				K(where_stmt)
			);

		public:
			Vector<TableStmt_s> from_stmts;			//from语句块
			Vector<ExprStmt_s> where_stmt;			//where语句块
			Hint stmt_hint;
			BitSet table_ids;
			u32 stmt_id;
			u32 ref_count;
			bool is_explain;

			Vector<ExprStmt_s> column_exprs;
			Vector<ExprStmt_s> aggr_exprs;
			Vector<ExprStmt_s> win_func_exprs;
			Vector<SubQueryStmt_s> subquery_exprs;
		};
	}
}

#endif	//DML_STMT_H