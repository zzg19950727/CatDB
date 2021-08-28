#ifndef PHY_EXPRESSION_H
#define PHY_EXPRESSION_H
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
		DECLARE(PhyOperator);
		DECLARE(Expression);
		DECLARE(SetExpression);
		DECLARE(AggregateExpression);
		DECLARE(OpExpression);
		DECLARE(SubplanExpression);

		class Operation
		{
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
			Object_s do_cast(Object_s& first_obj, Object_s& second_obj);

		private:
			OperationType type;
			friend class AggregateExpression;
			friend class OpExpression;
		};

		class Expression
		{
		protected:
			Expression();
		public:
			enum ExprType {
				Const = 0,
				ExecParam,
				Column,
				SetExpr,
				Subplan,
				OpExpr,
				Aggregate
			};
			virtual ~Expression();
			virtual Object_s get_result(const Row_s& row) = 0;
			virtual ExprType get_type()const = 0;

		private:
			DISALLOW_COPY_AND_ASSIGN(Expression)
		};

		class ConstExpression : public Expression
		{
		protected:
			ConstExpression() {}
			ConstExpression(const Object_s& object);
		public:
			static Expression_s make_const_expression(const Object_s& object);
			u32 set_object(Object_s& object);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
		private:
			Object_s const_object;
		};

		class ExecParamExpression : public ConstExpression
		{
		private:
			ExecParamExpression() = delete;
			ExecParamExpression(const Expression_s& ref_expr);
		public:
			static Expression_s make_exec_param_expression(const Expression_s& ref_expr);
			u32 set_value(const Row_s& row);
			ExprType get_type()const;
		private:
			Expression_s ref_expr;
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
		public:
			Common::ColumnDesc col_desc;
		};

		class SetExpression : public Expression
		{
		private:
			SetExpression() = delete;
			SetExpression(u32 idx);
		public:
			static Expression_s make_set_expression(u32 idx);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
		public:
			u32 index;
		};

		class OpExpression : public Expression
		{
		private:
			OpExpression() = delete;
			OpExpression(const Operation& op);
		public:
			~OpExpression();
			static Expression_s make_op_expression(OperationType op);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void add_param_expr(Expression_s& expr);

		public:
			Vector<Expression_s> param_exprs;
			Operation op;
		};

		//注意：目前还没有支持聚合函数聚合函数嵌套
		class AggregateExpression : public Expression
		{
		private:
			AggregateExpression() = delete;
			AggregateExpression(const Expression_s& expr, AggrType op, bool is_distinct);
		public:
			~AggregateExpression();
			static Expression_s make_aggregate_expression(const Expression_s& expr, AggrType op, bool is_distinct);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
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

		class SubplanExpression : public Expression
		{
		public:
			SubplanExpression() = delete;
			SubplanExpression(PhyOperator_s& subplan);
		public:
			static Expression_s make_subplan_expression(PhyOperator_s& subplan);
			Object_s get_result(const Row_s& row);
			ExprType get_type()const;
			void reset(const Row_s& row);
			
		public:
			Object_s result;
			PhyOperator_s subplan;
			Vector<Expression_s> exec_params;
		};
	}
}

#endif	//PHY_EXPRESSION_H
