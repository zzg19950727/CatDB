#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using CatDB::Common::ColumnDesc;
		using CatDB::Common::Row_s;
		using CatDB::Common::Object_s;
		DECLARE(Expression);
		DECLARE(UnaryExpression);
		DECLARE(AggregateExpression);
		DECLARE(BinaryExpression);
		DECLARE(TernaryExpression);

		class Operation
		{
		public:
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
				OP_NOT,
				OP_SUM,
				OP_COUNT,
				OP_AVG,
				OP_MIN,
				OP_MAX
			};
		private:
			Operation() = delete;
		protected:
			Operation(OperationType type);
		public:
			~Operation();
			void set_type(OperationType type);
			OperationType get_type()const;
			Object_s calc(Object_s& obj);
			Object_s calc(Object_s& first_obj, Object_s& second_obj);
			Object_s calc(Object_s& first_obj, Object_s& second_obj, Object_s& third_obj);
		private:
			Object_s do_add(Object_s& first_obj, Object_s& second_obj);
			Object_s do_sub(Object_s& first_obj, Object_s& second_obj);
			Object_s do_mul(Object_s& first_obj, Object_s& second_obj);
			Object_s do_div(Object_s& first_obj, Object_s& second_obj);
			Object_s do_equal(Object_s& first_obj, Object_s& second_obj);
			Object_s do_greater(Object_s& first_obj, Object_s& second_obj);
			Object_s do_less(Object_s& first_obj, Object_s& second_obj);
			Object_s do_between(Object_s& first_obj, Object_s& second_obj, Object_s& third_obj);
			Object_s do_is(Object_s& first_obj, Object_s& second_obj);
			Object_s do_is_not(Object_s& first_obj, Object_s& second_obj);
			Object_s do_in(Object_s& first_obj, Object_s& second_obj);
			Object_s do_not_in(Object_s& first_obj, Object_s& second_obj);
			Object_s do_exists(Object_s& first_obj);
			Object_s do_not_exists(Object_s& first_obj);
			Object_s do_and(Object_s& first_obj, Object_s& second_obj);
			Object_s do_or(Object_s& first_obj, Object_s& second_obj);
			Object_s do_not(Object_s& first_obj);

		private:
			OperationType type;
			friend class UnaryExpression;
			friend class AggregateExpression;
			friend class BinaryExpression;
			friend class TernaryExpression;
		};

		class Expression
		{
		protected:
			Expression();
		public:
			enum ExprType {
				Const = 0,
				Column,
				SubplanFilter,
				Unary,
				Binary,
				Ternary,
				Aggregate
			};
			virtual ~Expression();
			virtual Object_s get_result(const Row_s& row) = 0;
			virtual ExprType get_type()const = 0;
			virtual void reset(const Row_s& row) = 0;
		private:
			DISALLOW_COPY_AND_ASSIGN(Expression)
		};

		class ConstExpression : public Expression
		{
		private:
			ConstExpression() = delete;
			ConstExpression(const Object_s& object);
		public:
			static Expression_s make_const_expression(const Object_s& object);
			u32 set_object(Object_s& object);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
		private:
			Object_s const_object;
		};

		class ColumnExpression : public Expression
		{
		private:
			ColumnExpression() = delete;
			ColumnExpression(const ColumnDesc& desc);
		public:
			static Expression_s make_column_expression(const ColumnDesc& desc);
			u32 set_column_desc(ColumnDesc& desc);
			const ColumnDesc& get_column_desc()const;
			u32 set_alias_table_id(u32 table_id);
			u32 get_alias_table_id()const;
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
		private:
			Common::ColumnDesc col_desc;
			u32 alias_table_id;
		};

		class SubplanFilterExpression : public Expression
		{
		public:
			SubplanFilterExpression() = delete;
			SubplanFilterExpression(const ColumnDesc& desc);
		public:
			static Expression_s make_subplan_filter_expression(const ColumnDesc& desc);
			u32 set_column_desc(ColumnDesc& desc);
			const ColumnDesc& get_column_desc()const;
			u32 set_alias_table_id(u32 table_id);
			u32 get_alias_table_id()const;
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
		private:
			Object_s result;
			Common::ColumnDesc col_desc;
			u32 alias_table_id;
		};

		class UnaryExpression :public Expression
		{
		private:
			UnaryExpression() = delete;
		protected:
			UnaryExpression(const Expression_s& expr,
				const Operation& op);
		public:
			~UnaryExpression();
			static Expression_s make_unary_expression(const Expression_s& expr, Operation::OperationType op);
			virtual Object_s get_result(const Row_s& row);
			virtual ExprType get_type()const;
			void reset(const Row_s& row);
		public:
			Expression_s expr;
			Operation op;
		};
		//注意：目前还没有支持聚合函数内含distinct，聚合函数嵌套
		class AggregateExpression : public Expression
		{
		private:
			AggregateExpression() = delete;
			AggregateExpression(const Expression_s& expr,const Operation& op);
		public:
			~AggregateExpression();
			static Expression_s make_aggregate_expression(const Expression_s& expr, Operation::OperationType op);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
			u32 add_row(const Row_s& row);
			void reset();
		private:
			u32 sum(const Row_s& row);
			u32 count(const Row_s& row);
			u32 avg(const Row_s& row);
			u32 max(const Row_s& row);
			u32 min(const Row_s& row);

			Expression_s expr;
			Operation op;
			Object_s result;
			u32 row_count;
		};

		class BinaryExpression : public Expression
		{
		private:
			BinaryExpression() = delete;
			BinaryExpression(const Expression_s& first_expr,
				const Expression_s& second_expr,
				const Operation& op);
		public:
			~BinaryExpression();
			static Expression_s make_binary_expression(const Expression_s& first_expr,
				const Expression_s& second_expr,
				Operation::OperationType op);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
		public:
			Expression_s first_expr;
			Expression_s second_expr;
			Operation op;
		};

		class TernaryExpression : public Expression
		{
		private:
			TernaryExpression() = delete;
			TernaryExpression(const Expression_s& first_expr,
				const Expression_s& second_expr,
				const Expression_s& third_expr,
				const Operation& op);
		public:
			~TernaryExpression();
			static Expression_s make_ternary_expression(const Expression_s& first_expr,
				const Expression_s& second_expr,
				const Expression_s& third_expr,
				Operation::OperationType op);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
		public:
			Expression_s first_expr;
			Expression_s second_expr;
			Expression_s third_expr;
			Operation op;
		};
	}
}

#endif	//EXPRESSION_H
