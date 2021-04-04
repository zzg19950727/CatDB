#ifndef EXPR_STMT_H
#define EXPR_STMT_H
#include "stmt.h"
#include "type.h"
#include "log.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		DECLARE(Plan);
		DECLARE(Expression);
	}
	namespace Parser {
		using Sql::Plan_s;
		using Common::Object_s;
		using Sql::Expression_s;
		DECLARE(Stmt);
		//表达式语句
		class ExprStmt : public Stmt
		{
		public:
			enum ExprType {
				Const = 0,
				Column,
				Table,
				Aggregate,
				Query,
				List,
				Unary,
				Binary,
				Ternary
			};
			enum OperationType {
				OP_INVALID = 0,
				OP_ADD,
				OP_SUB,
				OP_MUL,
				OP_DIV,
				OP_EQ,
				OP_ANTI_EQ,
				OP_NE,
				OP_GE,
				OP_GT,
				OP_LE,
				OP_LT,
				OP_BETWEEN,
				OP_NOT_BETWEEN,
				OP_IS_NULL,
				OP_IS_NOT_NULL,
				OP_IN,
				OP_NOT_IN,
				OP_EXISTS,
				OP_NOT_EXISTS,
				OP_AND,
				OP_OR,
				OP_NOT,
				OP_UNION,
				OP_UNION_ALL,
				OP_INTERSECT,
				OP_EXCEPT,
				OP_MINUS,
				OP_LIKE,
				OP_NOT_LIKE
			};
			ExprStmt();
			~ExprStmt();
			StmtType stmt_type()const;
			virtual ExprType expr_stmt_type()const = 0;
			virtual String to_string()const = 0;
			static String op_string(OperationType op_type);
			virtual KV_STRING(
				K(alias_name)
			);

		public:
			String alias_name;	//表达式的别名
		};
		//单个常量表达式
		class ConstStmt : public ExprStmt
		{
		private:
			ConstStmt();
		public:
			~ConstStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_const_stmt(const Object_s& value);
			String to_string()const;
			KV_STRING(
				K(value)
			);

		public:
			Object_s value;
		};
		/*单列表达式
		column为*时表示表的所有列，
		table、column同时为*时表示from列表所有表的所有列
		*/
		class ColumnStmt : public ExprStmt
		{
		private:
			ColumnStmt();
		public:
			~ColumnStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_column_stmt(const String& table, const String& column);
			static Stmt_s make_all_column_stmt();
			bool is_all_column()const;
			String to_string()const;
			KV_STRING(
				K(table),
				K(column)
			);

		public:
			String table;	//所属表
			String column;	//列名
		};
		//一张表的描述语句
		class TableStmt : public ExprStmt
		{
		private:
			TableStmt();
		public:
			~TableStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_table_stmt(const String& database, const String& table_name);
			static Stmt_s make_table_stmt(const String& table_name);
			String to_string()const;
			KV_STRING(
				K(database),
				K(table_name),
				K(table_id),
				K(is_tmp_table)
			);

		public:
			String database;
			String table_name;
			//如果是子查询，则临时表由查询生成
			Plan_s subplan;
			//如果是由子查询改写的临时表，则标记连接类型
			u32 join_type;
			//如果是由子查询改写的临时表，则标记子查询的操作符
			u32 op_type;
			//如果是anti join，则保存anti join expr
			Expression_s anti_cond;
			//预测返回的行数
			double select_rows;
			//表的行数
			double row_count;
			u32 table_id;
			bool is_tmp_table;
		};
		//子查询语句
		class QueryStmt : public ExprStmt
		{
		private:
			QueryStmt();
		public:
			~QueryStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_query_stmt();
			String to_string()const;
			KV_STRING(
				K(query_stmt)
			);

		public:
			Stmt_s query_stmt;
		};
		//表达式列表
		class ListStmt : public ExprStmt
		{
		private:
			ListStmt();
		public:
			~ListStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_list_stmt();
			String to_string()const;
			KV_STRING(
				K(stmt_list)
			);

		public:
			Vector<Stmt_s> stmt_list;
		};
		//聚合函数语句
		class AggrStmt : public ExprStmt
		{
		private:
			AggrStmt();
		public:
			enum AggrType {
				SUM = 0,
				AVG,
				COUNT,
				MIN,
				MAX
			};
			~AggrStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_aggr_stmt();
			String to_string()const;
			String aggr_func_name()const;
			KV_STRING(
				K(aggr_func_name()),
				K(aggr_expr),
				K(distinct)
			);

		public:
			Stmt_s aggr_expr;	//聚合函数内的表达式
			AggrType aggr_func;		//聚合函数类型
			bool distinct;
		};
		//一元表达式语句
		class UnaryExprStmt :public ExprStmt
		{
		private:
			UnaryExprStmt();
		public:
			~UnaryExprStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_unary_stmt();
			String to_string()const;
			KV_STRING(
				K(op_string(op_type)),
				K(expr_stmt)
			);

		public:
			Stmt_s expr_stmt;
			OperationType op_type;
		};
		//二元表达式语句
		class BinaryExprStmt :public ExprStmt
		{
		private:
			BinaryExprStmt();
		public:
			~BinaryExprStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_binary_stmt();
			String to_string()const;
			KV_STRING(
				K(op_string(op_type)),
				K(first_expr_stmt),
				K(second_expr_stmt)
			);

		public:
			Stmt_s first_expr_stmt;
			Stmt_s second_expr_stmt;
			OperationType op_type;
		};
		//三元表达式语句
		class TernaryExprStmt :public ExprStmt
		{
		private:
			TernaryExprStmt();
		public:
			~TernaryExprStmt();
			ExprType expr_stmt_type()const;
			static Stmt_s make_ternary_stmt();
			String to_string()const;
			KV_STRING(
				K(op_string(op_type)),
				K(first_expr_stmt),
				K(second_expr_stmt),
				K(third_expr_stmt)
			);

		public:
			Stmt_s first_expr_stmt;
			Stmt_s second_expr_stmt;
			Stmt_s third_expr_stmt;
			OperationType op_type;
		};
	}
}

#endif	//EXPR_STMT_H