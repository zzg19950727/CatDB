#ifndef SQL_REWRITE_H
#define SQL_REWRITE_H
#include "expr_stmt.h"
#include "type.h"
namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(TableStmt);
		DECLARE(ColumnStmt);
	}
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		DECLARE(Plan);
		DECLARE(Filter);
		DECLARE(Expression);
		DECLARE(DeletePlan);
		DECLARE(UpdatePlan);
		DECLARE(SelectPlan);
		DECLARE(SqlRewriter);
		using Common::Row_s;
		using Parser::Stmt_s;
		using Sql::Filter_s;
		using Sql::Expression_s;
		using Sql::DeletePlan;
		using Sql::UpdatePlan;
		using Sql::SelectPlan;
		using Parser::ExprStmt;
		using Parser::TableStmt;
		using Parser::ColumnStmt;

		class SqlRewriter
		{
		public:
			SqlRewriter(SelectPlan* parent_query, Expression_s& subquery_expr);
			~SqlRewriter();
			static SqlRewriter_s make_sql_rewriter(SelectPlan* parent_query,
				Expression_s& subquery_expr,
				const Expression_s& lhs, 
				ExprStmt::OperationType op_type);
			u32 rewrite_for_select(Expression_s& expr);
		private:
			//为select plan改写N型的IN、EXISTS子查询，用semi join改写
			u32 rewrite_for_select_N_semi(Expression_s& expr);
			//为select plan改写N型的NOT IN、NOT EXISTS子查询，用anti join改写
			u32 rewrite_for_select_N_anti(Expression_s& expr);
			//为select plan改写J型的IN、EXISTS子查询，用semi join改写
			u32 rewrite_for_select_J_semi(Expression_s& expr);
			//为select plan改写J型的NOT IN、NOT EXISTS子查询，用anti join改写
			u32 rewrite_for_select_J_anti(Expression_s& expr);
			//为select plan改写JA型的查询，用semi join改写
			u32 rewrite_for_select_JA_semi(Expression_s& expr);


			u32 rewrite_select_list(u32 new_table_id);
			u32 rewrite_expr(Expression_s& expr, u32 new_table_id);
			bool is_column_eq_expr(const Expression_s& expr);
			u32 rewrite_group_by_for_JA(u32 new_table_id);

		private:
			Expression_s subquery_expr;
			Plan_s subquery;
			SelectPlan* subquery_plan;
			SelectPlan* parent_query_plan;
			Expression_s lhs;
			ExprStmt::OperationType op_type;
			String subquery_alias;
			static u32 tmp_table_id;
			bool is_correlated;
		};
	}
}

#endif	//SQL_REWRITE_H