#ifndef PHY_EXPRESSION_H
#define PHY_EXPRESSION_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Object_s;
		DECLARE(PhyOperator);
		DECLARE(Expression);
		DECLARE(ColumnExpression);
		DECLARE(SetExpression);
		DECLARE(ExecParamExpression);
		DECLARE(ListExpression);
		DECLARE(OpExpression);
		DECLARE(SubplanExpression);
		DECLARE(ExecCtx);

		u32 expr_filter(Vector<Expression_s> &filters, ExecCtx_s &ctx);
		class Expression
		{
		protected:
			Expression();
		public:
			virtual ~Expression();
			virtual u32 get_result(ExecCtx_s &ctx) = 0;
			virtual ExprType get_type()const = 0;
		private:
			DISALLOW_COPY_AND_ASSIGN(Expression)
		};

		class ConstExpression : public Expression
		{
		protected:
			ConstExpression() {}
			ConstExpression(const Object_s& object);
			virtual ~ConstExpression()=default;
		public:
			static Expression_s make_const_expression(const Object_s& object);
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
		private:
			Object_s const_object;
		};

		class ExecParamExpression : public Expression
		{
		private:
			ExecParamExpression() = delete;
			ExecParamExpression(u32 param_index);
		public:
			static Expression_s make_exec_param_expression(u32 param_index);
			u32 set_value(ExecCtx_s &ctx, Object_s &value);
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
		private:
			u32 param_index;
		};

		class ColumnExpression : public Expression
		{
		private:
			ColumnExpression() = delete;
			ColumnExpression(u32 op_id, u32 column_id);
		public:
			static Expression_s make_column_expression(u32 op_id, u32 column_id);
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
		public:
			u32 op_id;
			u32 column_id;
		};

		class SetExpression : public Expression
		{
		private:
			SetExpression() = delete;
			SetExpression(u32 idx);
		public:
			static Expression_s make_set_expression(u32 idx);
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
		public:
			u32 index;
		};

		class ListExpression : public Expression
		{
		private:
			ListExpression();
		public:
			~ListExpression();
			static Expression_s make_list_expression();
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
			void add_param_expr(Expression_s& expr);

		public:
			Vector<Expression_s> param_exprs;	
		};

		class OpExpression : public Expression
		{
		protected:
			OpExpression() = delete;
			OpExpression(OperationType op);
		public:
			~OpExpression();
			static Expression_s make_op_expression(OperationType op);
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
			void add_param_expr(Expression_s& expr);

		public:
			Vector<Expression_s> param_exprs;
			OperationType op_type;
		};

		class SubplanExpression : public Expression
		{
		public:
			SubplanExpression() = delete;
			SubplanExpression(u32 subplan_id);
		public:
			static Expression_s make_subplan_expression(u32 subplan_id);
			u32 get_result(ExecCtx_s &ctx) override;
			ExprType get_type()const override;
			u32 set_exec_param(ExecCtx_s &ctx);
			u32 get_next_result(ExecCtx_s &ctx, Object_s &res);
			
		public:
			u32 subplan_id;
			Vector<std::pair<ExecParamExpression_s, Expression_s>> exec_params;
		};

		DECLARE(CastExpression);
		class CastExpression : public OpExpression
		{
		private:
			CastExpression() = delete;
			CastExpression(OperationType op);
		public:
			~CastExpression();
			static Expression_s make_cast_expression(OperationType op);
			void set_dst_type(const Common::DataType& type);
			u32 get_result(ExecCtx_s &ctx) override;
		private:
			Common::DataType dst_type;
		};
	}
}

#endif	//PHY_EXPRESSION_H
