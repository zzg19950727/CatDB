#ifndef EXPR_STMT_H
#define EXPR_STMT_H
#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Parser {
		using Common::Object_s;
		DECLARE(Stmt);
		DECLARE(ExprStmt);
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
				OP_EQUAL,
				OP_GREATER,
				OP_LESS,
				OP_BETWEEN,
				OP_IS,
				OP_IS_NOT,
				OP_IN,
				OP_NOT_IN,
				OP_EXISTS,
				OP_NOT_EXISTS,
				OP_AND,
				OP_OR,
				OP_NOT
			};
			ExprStmt();
			~ExprStmt();
			StmtType stmt_type()const;
			virtual ExprType expr_stmt_type()const = 0;
		};
		//单个常量表达式
		class ConstStmt : public ExprStmt
		{
		public:
			ConstStmt();
			~ConstStmt();
			ExprType expr_stmt_type()const;
		public:
			Object_s value;
		};
		//单列表达式
		class ColumnStmt : public ExprStmt
		{
		public:
			ColumnStmt();
			~ColumnStmt();
			ExprType expr_stmt_type()const;
		public:
			String table;	//所属表
			String column;	//列名
		};
		//一张表的描述语句
		class TableStmt : public ExprStmt
		{
		public:
			TableStmt();
			~TableStmt();
			ExprType expr_stmt_type()const;
		public:
			String table_name;			//真实表
			String alias_table_name;	//表的别名
		};
		//子查询语句
		class QueryStmt : public ExprStmt
		{
		public:
			QueryStmt();
			~QueryStmt();
			ExprType expr_stmt_type()const;
		public:
			Stmt_s query_stmt;
		};
		//聚合函数语句
		class AggrStmt : public ExprStmt
		{
		public:
			enum AggrType {
				SUM = 0,
				AVG,
				COUNT,
				MIN,
				MAX
			};
			AggrStmt();
			~AggrStmt();
			ExprType expr_stmt_type()const;
		public:
			ExprStmt_s aggr_expr;	//聚合函数内的表达式
			AggrType aggr_func;		//聚合函数类型
		};
		//一元表达式语句
		class UnaryExprStmt :public ExprStmt
		{
		public:
			UnaryExprStmt();
			~UnaryExprStmt();
			ExprType expr_stmt_type()const;
		public:
			ExprStmt_s expr_stmt;
			OperationType op_type;
		};
		//二元表达式语句
		class BinaryExprStmt :public ExprStmt
		{
		public:
			BinaryExprStmt();
			~BinaryExprStmt();
			ExprType expr_stmt_type()const;
		public:
			ExprStmt_s first_expr_stmt;
			ExprStmt_s second_expr_stmt;
			OperationType op_type;
		};
		//三元表达式语句
		class TernaryExprStmt :public ExprStmt
		{
		public:
			TernaryExprStmt();
			~TernaryExprStmt();
			ExprType expr_stmt_type()const;
		public:
			ExprStmt_s first_expr_stmt;
			ExprStmt_s second_expr_stmt;
			ExprStmt_s third_expr_stmt;
			OperationType op_type;
		};
	}
}

#endif	//EXPR_STMT_H