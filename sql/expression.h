#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "expr_stmt.h"
#include "hash_table.h"
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using Common::ColumnDesc;
		using Common::Row_s;
		using Common::Object_s;
		using Parser::TableStmt;
		DECLARE(Plan);
		DECLARE(Expression);
		DECLARE(UnaryExpression);
		DECLARE(AggregateExpression);
		DECLARE(BinaryExpression);
		DECLARE(TernaryExpression);

		class Operation
		{
		public:
			typedef Parser::ExprStmt::OperationType OperationType;
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
			Object_s do_anti_equal(Object_s& first_obj, Object_s& second_obj);
			Object_s do_not_equal(Object_s& first_obj, Object_s& second_obj);
			Object_s do_greater(Object_s& first_obj, Object_s& second_obj);
			Object_s do_GE(Object_s& first_obj, Object_s& second_obj);
			Object_s do_less(Object_s& first_obj, Object_s& second_obj);
			Object_s do_LE(Object_s& first_obj, Object_s& second_obj);
			Object_s do_between(Object_s& first_obj, Object_s& second_obj, Object_s& third_obj);
			Object_s do_not_between(Object_s& first_obj, Object_s& second_obj, Object_s& third_obj);
			Object_s do_is_null(Object_s& first_obj);
			Object_s do_is_not_null(Object_s& first_obj);
			Object_s do_in(Object_s& first_obj, Object_s& second_obj);
			Object_s do_not_in(Object_s& first_obj, Object_s& second_obj);
			Object_s do_exists(Object_s& first_obj);
			Object_s do_not_exists(Object_s& first_obj);
			Object_s do_and(Object_s& first_obj, Object_s& second_obj);
			Object_s do_or(Object_s& first_obj, Object_s& second_obj);
			Object_s do_not(Object_s& first_obj);
			Object_s do_minus(Object_s& first_obj);
			Object_s do_like(Object_s& first_obj, Object_s& second_obj);
			Object_s do_not_like(Object_s& first_obj, Object_s& second_obj);

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
				Subplan,
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
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
		public:
			Common::ColumnDesc col_desc;
			TableStmt* table;
			Object_s result;
		};

		class SubplanExpression : public Expression
		{
		public:
			SubplanExpression() = delete;
			SubplanExpression(const Plan_s& subplan);
		public:
			static Expression_s make_subplan_expression(const Plan_s& subplan, bool correlated, const String& alias="");
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
			
		public:
			Object_s result;
			Plan_s subplan;
			String alias_name;
			bool is_correlated;
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
		public:
			typedef Parser::AggrStmt::AggrType	AggrType;
		private:
			AggregateExpression() = delete;
			AggregateExpression(const Expression_s& expr, AggrType op, bool is_distinct);
		public:
			~AggregateExpression();
			static Expression_s make_aggregate_expression(const Expression_s& expr, AggrType op, bool is_distinct);
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

			AggrType op;
			Object_s result;
			u32 row_count;
			Common::HashTable hash_table;
		public:
			Expression_s expr;
			bool is_distinct;
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
