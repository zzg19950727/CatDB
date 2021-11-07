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
			virtual StmtType stmt_type() const = 0;
			virtual u32 formalize();
			void increase_ref_count() { ++ref_count; }
			void decrease_ref_count() { --ref_count; }
			
			u32 get_stmt_exprs(Vector<ExprStmt_s> &exprs);
			u32 replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   const Vector<ExprStmt_s> &new_exprs);
			u32 collect_special_exprs();
			u32 get_column_exprs(u32 table_id, Vector<ExprStmt_s> &columns);
			u32 get_table_items(Vector<TableStmt_s> &tables);
			Vector<ExprStmt_s> &get_column_exprs() { return column_exprs; }
			Vector<SubQueryStmt_s> &get_subquery_exprs() { return subquery_exprs; }
			u32 get_child_stmts(Vector<SelectStmt_s> &child_stms);
			u32 update_table_hint();
			u32 reset_stmt_id(u32 stmt_id);

		private:
			bool update_join_hint(JoinHintStmt_s join_hint);
			void change_ordered_to_leading(LeadingHintStmt_s &leading_hint);
			void change_table_item_to_leading(TableStmt_s &table_item, LeadingTable_s &leading_table);
			bool update_leading_table_hint(LeadingTable_s &leading_hint);
			bool find_table_id(const String &table_name, u32 &table_id);

		protected:
			u32 collect_special_exprs(ExprStmt_s& expr);
			virtual u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs);
			virtual u32 inner_replace_stmt_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                   				 const Vector<ExprStmt_s> &new_exprs);
			u32 inner_deep_copy(DMLStmt_s stmt, u32 flag)const;

		public:
			VIRTUAL_KV_STRING(
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
			Vector<SubQueryStmt_s> subquery_exprs;
		};
	}
}

#endif	//DML_STMT_H