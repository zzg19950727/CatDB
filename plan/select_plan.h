#ifndef SELECT_PLAN_H
#define SELECT_PLAN_H
#include "type.h"
#include "plan.h"

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
		using Common::Row_s;
		using Parser::Stmt_s;
		using Sql::Filter_s;
		using Sql::Expression_s;
		using Parser::TableStmt;
		using Parser::ColumnStmt;

		class SelectPlan : public Plan
		{
		protected:
			SelectPlan();
		public:
			~SelectPlan();
			static Plan_s make_select_plan(const Stmt_s& lex_insert_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
		private:
			typedef Pair<TableStmt*, TableStmt*> JoinableTables;
			typedef Pair<Expression_s, Expression_s> JoinConditions;
			//解析where子句，拆分成一个一个and连接的stmt
			u32 resolve_where_stmt(const Stmt_s& where_stmt);
			//解析单个stmt，可能是连接谓词，可能是基表过滤谓词，也可能是普通的过滤谓词
			u32 resolve_simple_stmt(const Stmt_s& expr_stmt);
			//当前语句块如果是列描述则返回列描述，否则返回null
			ColumnStmt* resolve_column_stmt(const Stmt_s& stmt);
			//将语句块转换成表达式
			u32 resolve_expr(const Stmt_s& expr_stmt, Expression_s& expr);
			//判断当前谓词块是否是基表过滤谓词
			bool is_table_filter(const Stmt_s& expr_stmt, TableStmt*& table);
			//根据列语句块获取列描述
			u32 resolve_column_desc(ColumnStmt* column_stmt, ColumnDesc& col_desc);
			//从from list中搜索表
			u32 find_table(const String& table_name, TableStmt*& table);
			//从from list中搜索含有指定列的表
			u32 who_have_column(ColumnStmt* column_stmt, TableStmt*& table);
			//从from list中搜索含有指定列的表
			u32 who_have_column(const String& column_name, TableStmt*& table);
			//解析from语句块为table list
			u32 get_ref_tables(const Stmt_s& from_stmt);
			//根据两张表搜素这两张表的连接谓词
			u32 search_jon_info(const JoinableTables& join_tables, JoinConditions& join_cond);
			//添加两张表的连接谓词
			u32 add_join_cond(const JoinableTables& join_tables, const Expression_s& expr);
			//添加两张表的等值连接谓词
			u32 add_join_equal_cond(const JoinableTables& join_tables, const Expression_s& expr);
			//添加基表过滤谓词
			u32 add_table_filter(TableStmt* table, const Expression_s& filter);
			//expr = expr and other
			u32 make_and_expression(Expression_s& expr, const Expression_s& other);
			//选择最优的join order
			u32 choos_best_join_order();

		private:
			Vector<TableStmt*> table_list;
			/*不能用于table scan和join的filter，比如or条件连接的双表谓词*/
			Expression_s filter_after_join;
			HashMap<TableStmt*, Expression_s> table_filters;
			/*第一个pair指定join的两张表，第二个pair指定join condition和join equal condition
			  join condition包含join equal condition*/
			HashMap<JoinableTables, JoinConditions> join_info;
		};
	}
}

#endif	//SELECT_PLAN_H