#ifndef PHY_WIN_EXPRESSION_H
#define PHY_WIN_EXPRESSION_H
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
		DECLARE(WindowFuncExpression);
		DECLARE(ExecCtx);
        DECLARE(HashTable);

        DECLARE(WinFuncExprCtx);
        class WinFuncExprCtx {
		public:
			WinFuncExprCtx()
			:row_count(0),
			rank(1)
			{}
			~WinFuncExprCtx() = default;
            static WinFuncExprCtx_s make_win_ctx();
            u32 init(ExecCtx_s &exec_ctx, WindowFuncExpression_s &win_func_expr);
            u32 reset();
			u64 row_count;
			u64 rank;
			Object_s value;
			Vector<Object_s> order_by_values;
			HashTable_s hash_table;
		};

		class WindowFuncExpression : public Expression
		{
		private:
			WindowFuncExpression() = delete;
			WindowFuncExpression(const Expression_s& expr, WinType op, bool is_distinct);
		public:
			~WindowFuncExpression();
			static Expression_s make_window_func_expression(const Expression_s& expr, WinType op, bool is_distinct);
			u32 get_result(ExecCtx_s &ctx) override;
            u32 get_result(ExecCtx_s &ctx, WinFuncExprCtx_s &win_ctx);
			ExprType get_type()const override;
			u32 add_row(ExecCtx_s &ctx, WinFuncExprCtx_s &win_ctx);
            
		private:
			u32 calc_order_by_exprs(ExecCtx_s &ctx, Vector<Object_s> &order_by_values);
			u32 compare_order_by_values(const Vector<Object_s> &l_values, 
										const Vector<Object_s> &r_values,
										bool &is_equal);
			u32 sum(Object_s &result, WinFuncExprCtx_s &win_ctx);
			u32 count(Object_s &result, WinFuncExprCtx_s &win_ctx);
			u32 max(Object_s &result, WinFuncExprCtx_s &win_ctx);
			u32 min(Object_s &result, WinFuncExprCtx_s &win_ctx);

		public:
			Vector<Expression_s> order_by_exprs;
			Expression_s expr;
			WinType op;
			bool is_distinct;
		};

		class WindowFuncCalculator {
		public:
			WindowFuncCalculator(const Vector<Expression_s>& win_funcs);
			~WindowFuncCalculator() = default;
		protected:
			u32 reset_win_func();
			u32 init_win_func(ExecCtx_s &exec_ctx);
			u32 add_row_to_win_func(ExecCtx_s &exec_ctx, Row_s& row);
			u32 calc_win_func(ExecCtx_s &exec_ctx, Row_s& row);

			Vector<WindowFuncExpression_s> win_funcs;
			Vector<WinFuncExprCtx_s> win_ctxs;
		};
	}
}

#endif	//PHY_WIN_EXPRESSION_H
