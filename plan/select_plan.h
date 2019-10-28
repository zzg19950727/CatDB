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
			void set_alias_table_id(u32 id);
			u32 get_alias_table_id()const;
		private:
			/*第一个pair指定join的两张表，第二个pair指定join condition和join equal condition
			  join condition包含join equal condition*/
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
			//解析select list中的*列
			u32 resolve_all_column_in_select_list(const Stmt_s& stmt);
			//解析count聚合函数内的*表达式
			u32 resolve_all_column_in_count_agg(const Stmt_s& stmt, Expression_s& expr);
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
			u32 search_jon_info(const JoinableTables& join_tables, JoinConditions*& join_cond);
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
			//解析group列
			u32 resolve_group_stmt(const Stmt_s& group_stmt);
			//解析having子句
			u32 resolve_having_stmt(const Stmt_s& having_stmt);
			//解析limit子句
			u32 resolve_limit_stmt(const Stmt_s& limit_stmt);
			//解析sort列
			u32 resolve_sort_stmt(const Stmt_s& sort_stmt);
			u32 resolve_select_list(const Stmt_s& select_list);
			u32 add_access_column(TableStmt* table, const ColumnDesc& col_desc);
			u32 make_access_row_desc();
			u32 make_join_plan(PhyOperator_s& op);
			u32 make_table_scan(TableStmt* table, PhyOperator_s& op);
			u32 make_group_pan(PhyOperator_s& op);
			u32 make_sort_plan(PhyOperator_s& op);
			u32 make_query_plan(PhyOperator_s& op);
			u32 make_limit_and_distinct_plan(PhyOperator_s& op);
		private:
			//select list语句块中出现过的聚合函数
			Vector<Expression_s> aggr_exprs;
			//select list语句块中每一个输出表达式，聚合函数预处理过，详见select_list解析函数
			Vector<Expression_s> select_list;
			//from list的表
			Vector<TableStmt*> table_list;
			/*不能用于table scan和join的filter，比如or条件连接的双表谓词*/
			Expression_s filter_after_join;
			//每张表的过滤谓词
			HashMap<TableStmt*, Expression_s> table_filters;
			//每张表需要访问的列
			HashMap<TableStmt*, Vector<ColumnDesc> > table_access_column;
			HashMap<TableStmt*, RowDesc > table_access_row_desc;
			//两表连接条件信息
			HashMap<JoinableTables, JoinConditions> join_info;
			//聚合列
			Vector<Expression_s> group_cols;
			//having过滤谓词表达式
			Expression_s having_filter;
			//排序列
			Vector<Expression_s> sort_cols;
			//limit相关
			u32 limit_offset, limit_size;
			//当前在解析select_list还是having语句块，用于区分聚合函数的解析处理
			u32 resolve_select_list_or_having;
			//查询生成的临时表ID
			u32 alias_table_id;
			bool is_distinct;
			bool asc;
			bool have_limit;
		};
	}
}

#endif	//SELECT_PLAN_H