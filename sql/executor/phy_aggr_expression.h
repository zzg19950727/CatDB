#ifndef PHY_AGGR_EXPRESSION_H
#define PHY_AGGR_EXPRESSION_H
#include "phy_expression.h"
#include "hash_table.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Row_s;
		using Common::Object_s;
		DECLARE(AggregateExpression);
		DECLARE(ExecCtx);
        DECLARE(HashTable);

        DECLARE(AggrExprCtx);
        class AggrExprCtx {
		public:
			AggrExprCtx()
			:row_count(0)
			{}
			~AggrExprCtx() = default;
            static AggrExprCtx_s make_aggr_ctx();
            u32 init(ExecCtx_s &exec_ctx, AggregateExpression_s &aggr_expr);
            u32 reset();
			u64 row_count;
			Object_s value;
			HashTable_s hash_table;
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
			u32 get_result(ExecCtx_s &ctx) override;
            u32 get_result(ExecCtx_s &ctx, AggrExprCtx_s &aggr_ctx);
			ExprType get_type()const override;
			u32 add_row(ExecCtx_s &ctx, AggrExprCtx_s &aggr_ctx);
            
		private:
			u32 sum(Object_s &result, AggrExprCtx_s &aggr_ctx);
			u32 count(Object_s &result, AggrExprCtx_s &aggr_ctx);
			u32 max(Object_s &result, AggrExprCtx_s &aggr_ctx);
			u32 min(Object_s &result, AggrExprCtx_s &aggr_ctx);

			AggrType op;
		public:
			Expression_s expr;
			bool is_distinct;
		};

		class AggregateExpCalculator {
		public:
			AggregateExpCalculator(const Vector<Expression_s>& agg_funcs);
			~AggregateExpCalculator() = default;
		protected:
			u32 reset_agg_func();
			u32 init_agg_func(ExecCtx_s &exec_ctx);
			u32 add_row_to_agg_func(ExecCtx_s &exec_ctx, Row_s& row);
			u32 calc_aggr_func(ExecCtx_s &exec_ctx, Row_s& row);

			Vector<AggregateExpression_s> agg_funcs;
			Vector<AggrExprCtx_s> aggr_ctxs;
		};
	}
}

#endif	//PHY_AGGR_EXPRESSION_H
